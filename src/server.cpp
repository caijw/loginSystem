#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <fstream>
#include <grpcpp/grpcpp.h>
/*mongodb headers begin*/
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
/*mongodb headers end*/
#include "user_id.h"
#include "Database.h"
#include "util.h"
#include "crypto.h"
#include "login_system.grpc.pb.h"
#include <plog/Log.h> 
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
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

/*
  secret key between AS and SS,
  hard encoding,this should be maintain carefully.
*/
std::string K_AS_SS = "K_AS_SS";


class LoginSystemServiceImpl final : public LoginSystem::Service {

  Status hello(ServerContext* context, const helloRequest* request,
                  helloResponse* response) override{
    std::string clientHello = request->helloclient();
    LOGD<< "receive client hello: " << clientHello;
    response->set_helloserver("from server");
    return Status::OK;
  }
  
  Status registerAccount(ServerContext* context, const registerRequest* request,
                  registerResponse* response) override {
    LOGD << "registerAccount";
    std::string H1 = request->h1();
    std::string nickname = request->nickname();
    std::string phone_num = request->phone_num();
    std::cout << "H1: " << H1;
    int ret;
    std::string userId;
    ret = user_id::getInstance()->getNewUserId(userId);
    LOGD << "user_id::getInstance ret:" << ret;
    if(ret == 0){
      LOGD << "getNewUserId: " << userId;
      std::string S1 = md5Enc(H1 + userId);
      auto user_info_collection = Database::getInstance()->getCollection("user_info");
      auto doc_builder = bsoncxx::builder::stream::document{};

      bsoncxx::document::view_or_value user_info_doc = doc_builder
        << "user_id" << userId
        << "s1" << S1
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
                  loginResponse* response) override{
    std::string data = request->data();
    std::string user_id = request->user_id();
    LOGD << "login account user_id: " << user_id;


    std::string data_user_id = data.substr(0, 10);
    std::string data_timestamp = data.substr(0, 10);

    auto user_info_collection = Database::getInstance()->getCollection("user_info");
    auto query = document{}
      << "user_id" << user_id
      << finalize;
    auto find_ret = user_info_collection.find_one(query.view());
    if(find_ret){
      auto find_view = find_ret->view();
      auto iter_s1 = find_view.find("s1");
      auto iter_seq = find_view.find("seq");
      std::string S1(iter_s1->get_utf8().value);

      /*verify req data*/
      data = AESDec(data, S1);
      size_t tmp = 0;
      tmp = data_user_id.find_first_not_of('x', 0);
      data_user_id = data_user_id.substr(tmp, data_user_id.size() - tmp);
      tmp = data_timestamp.find_first_not_of('x', 0);
      data_timestamp = data_timestamp.substr(tmp, data_timestamp.size() - tmp);
      if(user_id != data_user_id){
        response->set_ret(-7);
        response->set_msg("error");
        response->set_st("");
      }else{
        LOGD << "cur seq:" << iter_seq->get_int32().value;
        int seq = iter_seq->get_int32().value;
        LOGD << "login seq: " << seq;
        std::string tmp_user_id = user_id;
        std::string timestamp = std::to_string(currentTimeSecond());
        std::string nextSeq = std::to_string(seq + 1);
        /*fixed length*/
        for(decltype(user_id.size()) i = 0; i < (10 - user_id.size()); ++i){
          tmp_user_id = "x" + tmp_user_id;
        }
        /*fixed length*/
        for(decltype(timestamp.size()) i = 0; i < (10 - timestamp.size()); ++i){
          timestamp = "x" + timestamp;
        }
        /*fixed length*/
        for(decltype(nextSeq.size()) i = 0; i < (5 - nextSeq.size()); ++i){
          nextSeq = "x" + nextSeq;
        }
        std::string ST = AESEnc(tmp_user_id + timestamp + nextSeq, K_AS_SS);

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
          response->set_ret(0);
          response->set_msg("login success");
          response->set_st(ST);
        }else{
          response->set_ret(-4);
          response->set_msg("error, update seq");
          response->set_st("");
        }
      }



    }else{
      response->set_ret(-3);
      response->set_msg("error, account not exist.");
      response->set_st("");
    }
    return Status::OK;
  }
  Status verifyST(ServerContext* context, const verifySTRequest* request,
                  verifySTResponse* response) override{
    std::string user_id = request->user_id();
    std::string ST = request->st();
    LOGD << "user_id: " << user_id;
    ST = AESDec(ST, K_AS_SS);
    std::string ST_user_id = ST.substr(0, 10);
    std::string ST_timestamp = ST.substr(0, 10);
    std::string ST_seq = ST.substr(0, 5);
    size_t tmp = 0;
    tmp = ST_user_id.find_first_not_of('x', 0);
    ST_user_id = ST_user_id.substr(tmp, ST_user_id.size() - tmp);
    tmp = ST_timestamp.find_first_not_of('x', 0);
    ST_timestamp = ST_timestamp.substr(tmp, ST_timestamp.size() - tmp);
    tmp = ST_seq.find_first_not_of('x', 0);
    ST_seq = ST_seq.substr(tmp, ST_seq.size() - tmp);
    int seq = std::stoi(ST_seq);
    LOGD << "ST_user_id: " << ST_user_id
      << " ST_timestamp: " << ST_timestamp
      << " ST_seq: " << ST_seq;
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
        LOGD << "cur seq:" << seq;
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
    LOGD << "use SSL";
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
    LOGD << "not use SSL";
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
  /*init logger*/
  plog::init(plog::debug, "./server_log.log");
  /*init database*/
  Database::getInstance()->setDb("mongodb://localhost:27017", "login_system");

  RunServer(argc, argv);


  return 0;
}
