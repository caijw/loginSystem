#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <fstream>
#include <map>
#include <grpc++/grpc++.h>

/*mongodb headers begin*/
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
/*mongodb headers end*/
#include "user_id.h"
#include "Database.h"
#include "util.h"
#include "crypto_impl.hpp"

#include "protos/login_system.pb.h"
#include "protos/login_system.grpc.pb.h"

// #include <plog/Log.h> 
#include "loginStreamManager.h"
#include <chrono>
#include <thread>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;

using login_system::helloRequest;
using login_system::helloResponse;
using login_system::registerRequest;
using login_system::registerResponse;
using login_system::loginRequest;
using login_system::loginResponse;
using login_system::verifySTRequest;
using login_system::verifySTResponse;
using login_system::LoginSystem;

using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::type;

#define LOGD std::cout

/*
  secret key between AS and SS,
  hard encoding,this should be maintain carefully.
*/
std::string K_AS_SS = "K_AS_SS";


class LoginSystemServiceImpl final : public LoginSystem::Service {
public:
  Status hello(ServerContext* context, const helloRequest* request,
                  helloResponse* response) override{
    std::string clientHello = request->helloclient();
    LOGD<< "receive client hello: " << clientHello << std::endl;
    response->set_helloserver("from server");
    return Status::OK;
  }

  Status registerAccount(ServerContext* context, const registerRequest* request,
                  registerResponse* response) override {
    LOGD << "registerAccount" << std::endl;
    crypto::CryptoImpl crypto;
    std::string hash = request->hash();
    std::string nickname = request->nickname();
    std::string phone_num = request->phone_num();
    int ret;
    std::string userId;
    ret = user_id::getInstance()->getNewUserId(userId);
    LOGD << "user_id::getInstance ret:" << ret << std::endl;
    if(ret == 0){
      LOGD << "getNewUserId: " << userId << std::endl;
      auto user_info_collection = Database::getInstance()->getCollection("user_info");
      auto doc_builder = bsoncxx::builder::stream::document{};

      bsoncxx::document::view_or_value user_info_doc = doc_builder
        << "user_id" << userId
        << "hash" << hash
        << "phone_num" << phone_num
        << "seq" << 1
        // << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;
      
      bsoncxx::stdx::optional<mongocxx::result::insert_one> insert_result = user_info_collection.insert_one(user_info_doc);

      if(insert_result){
        response->set_ret(0);
        response->set_msg("register success");
        response->set_user_id(userId);
        
      }else{
        response->set_ret(-1);
        response->set_msg("error");
        response->set_user_id("");
      }
    }else{
      response->set_ret(ret);
      response->set_msg("error");
      response->set_user_id("");
    }
    return Status::OK;
  }
  Status loginAccount(ServerContext* context, const loginRequest* request,
                  ServerWriter<loginResponse>* responseWriter) override{

    LoginStreamManager *loginStreamManager = LoginStreamManager::getInstance();

    loginResponse response;
    crypto::CryptoImpl crypto;
    std::string data = request->data();
    std::string user_id = request->user_id();
    LOGD << "login account user_id: " << user_id << std::endl;
    LOGD << "before dec:" << data << std::endl;
    auto user_info_collection = Database::getInstance()->getCollection("user_info");
    auto query = document{}
      << "user_id" << user_id
      << finalize;

    auto find_ret = user_info_collection.find_one(query.view());

    if(find_ret){
      auto find_view = find_ret->view();
      auto iter_hash = find_view.find("hash");
      auto iter_seq = find_view.find("seq");
      std::string hash(iter_hash->get_utf8().value);

      /*decode req data*/
      data = crypto.AESDec(data, hash);
      LOGD << "after dec:" << data << std::endl;
      /*get the user id */
      std::string data_user_id = data.substr(0, 10);
      /*get the timestamp*/
      std::string data_timestamp = data.substr(10, 20);
      /*remove padding x*/

      LOGD << "data_user_id: " << data_user_id << " data_timestamp: " << data_timestamp << std::endl;

      data_user_id = crypto.deStringWithFixedLength(data_user_id, "x");
      data_timestamp = crypto.deStringWithFixedLength(data_timestamp, "x");

      if(user_id != data_user_id){
        response.set_ret(-7);
        response.set_msg("error user id");
        response.set_st("");
        response.set_logout(false);
        responseWriter->Write(response);
       }else{
        int seq = iter_seq->get_int32().value;
        LOGD << "cur seq:" << seq << std::endl;
        std::string tmp_user_id = user_id;
        std::string timestamp = std::to_string(currentTimeSecond());
        std::string nextSeq = std::to_string(seq + 1);

        /*fixed length with padding*/
        tmp_user_id = crypto.stringWithFixedLength(tmp_user_id, 10, "x");
        /*fixed length with padding*/
        timestamp = crypto.stringWithFixedLength(timestamp, 10, "x");
        /*fixed length with padding*/
        nextSeq = crypto.stringWithFixedLength(nextSeq, 5, "x");

        std::string ST = crypto.AESEnc(tmp_user_id + timestamp + nextSeq, K_AS_SS);

        auto query = document{}
          << "user_id" << user_id
          << finalize;
        auto update = document{}
          << "$set"
          << open_document
          << "seq" << (seq + 1)
          << close_document
          << finalize;
        auto update_ret = user_info_collection.update_one(query.view(), update.view());
        if(update_ret && update_ret->modified_count() == 1){
          response.set_ret(0);
          response.set_msg("login success");
          response.set_st(ST);
          response.set_logout(false);
          responseWriter->Write(response);
          /*loginStreamManager update seq*/
          loginStreamManager->update(user_id, seq + 1);
          /*long-long-time stream, ends up until another client logins*/
          while(true){
            int tmpSeq = loginStreamManager->get(user_id);
            if(tmpSeq != (seq + 1)){
              /*end up the stream and ask the client to logout*/
              loginResponse endResponse;
              endResponse.set_ret(-10);
              endResponse.set_msg("invalid login");
              endResponse.set_st("");
              endResponse.set_logout(true);
              break;
            }else{
              /*have a sleep*/
              std::chrono::milliseconds dura(50);
              std::this_thread::sleep_for(dura);
            }
          }


        }else{
          response.set_ret(-4);
          response.set_msg("error, update seq");
          response.set_st("");
          response.set_logout(false);
          responseWriter->Write(response);
        }
      }
    }else{
      LOGD << "not found user id: " << user_id << std::endl;
      response.set_ret(-3);
      response.set_msg("error, account not exist.");
      response.set_st("");
      response.set_logout(false);
      responseWriter->Write(response);
    }
    
    return Status::OK;
  }
  Status verifyST(ServerContext* context, const verifySTRequest* request,
                  verifySTResponse* response) override{
    crypto::CryptoImpl crypto;
    std::string user_id = request->user_id();
    std::string ST = request->st();
    LOGD << "user_id: " << user_id << std::endl;
    ST = crypto.AESDec(ST, K_AS_SS);
    std::string ST_user_id = ST.substr(0, 10);
    std::string ST_timestamp = ST.substr(0, 10);
    std::string ST_seq = ST.substr(0, 5);

    ST_user_id = crypto.deStringWithFixedLength(ST_user_id, "x");

    ST_timestamp = crypto.deStringWithFixedLength(ST_timestamp, "x");

    ST_seq = crypto.deStringWithFixedLength(ST_seq, "x");

    int seq = std::stoi(ST_seq);
    LOGD << "ST_user_id: " << ST_user_id
      << " ST_timestamp: " << ST_timestamp
      << " ST_seq: " << ST_seq << std::endl;
    if(user_id != ST_user_id){
      response->set_ret(-4);
      response->set_msg("error, userId invalid.");
    }else{
      auto user_info_collection = Database::getInstance()->getCollection("user_info");
      auto query = document{}
        << "user_id" << user_id
        << finalize;
      auto find_ret = user_info_collection.find_one(query.view());
      if(find_ret){
        auto find_view = find_ret->view();
        auto iter_seq = find_view.find("seq");
        int cur_seq = iter_seq->get_int32().value;
        LOGD << "cur seq:" << seq << std::endl;
        if(seq != cur_seq){
          response->set_ret(-6);
          response->set_msg("user not yet login.");
        }else{
          response->set_ret(0);
          response->set_msg("ok");
        }
      }else{
        response->set_ret(-5);
        response->set_msg("error, userId invalid.");
      }

    }
    return Status::OK;
  }

private:
  std::map<std::string, ServerWriter<loginResponse>*> loginResponseMap;

};
void RunServer(int argc, char** argv) {
  std::string server_address("0.0.0.0:50051");
  bool useSSL = false;
  for(int i = 1; i < argc; i++){
    if(std::string(argv[i]) == "--useSSL=1"){
      useSSL = true;
    }
  }
  LoginSystemServiceImpl service;
  ServerBuilder builder;

  if(useSSL){
    LOGD << "use SSL" << std::endl;
    std::string key;
    std::string cert;
    std::string root;
    /*https://github.com/grpc/grpc/issues/9538*/ 
    readFile( "server.crt", cert );
    readFile( "server.key", key );
    readFile( "ca.crt", root );
    grpc::SslServerCredentialsOptions::PemKeyCertPair keycert ={key, cert};
    grpc::SslServerCredentialsOptions sslOps;
    sslOps.pem_root_certs = root;
    sslOps.pem_key_cert_pairs.push_back ( keycert );
    builder.AddListeningPort(server_address, grpc::SslServerCredentials( sslOps ));
  }else{
    LOGD << "not use SSL" << std::endl;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

  }

  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<Server> server(builder.BuildAndStart());
  LOGD << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

int main(int argc, char** argv) {

  /*init database*/
  Database::getInstance()->setDb("mongodb://localhost:27017", "login_system");

  LoginStreamManager *loginStreamManager = LoginStreamManager::getInstance();

  RunServer(argc, argv);


  return 0;
}
