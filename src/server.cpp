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
#include "login_system.grpc.pb.h"
#include <plog/Log.h> 
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using login_system::registerRequest;
using login_system::registerResponse;
using login_system::LoginSystem;

// Logic and data behind the server's behavior.
class LoginSystemServiceImpl final : public LoginSystem::Service {
  Status registerAccount(ServerContext* context, const registerRequest* request,
                  registerResponse* response) override {
    std::string S1 = request->s1();
    std::string nickname = request->nickname();
    int timestamp = request->timestamp();

    std::string userId = user_id::getInstance()->getNewUserId();
    LOGD << "getNewUserId: " << userId;
    



    response->set_user_id("user_id_1");
    response->set_timestamp(1111);

    return Status::OK;
  }
};
void read ( const std::string& filename, std::string& data ){
  std::ifstream file ( filename.c_str (), std::ios::in );
  if (file.is_open ()){
    std::stringstream ss;
    ss << file.rdbuf ();
    file.close ();
    data = ss.str ();
  }
  return;
}
void RunServer() {
  std::string server_address("0.0.0.0:50051");
  std::string key;
  std::string cert;
  std::string root;
  read ( "server.crt", cert );
  read ( "server.key", key );
  read ( "ca.crt", root );

  LoginSystemServiceImpl service;
  ServerBuilder builder;
  grpc::SslServerCredentialsOptions::PemKeyCertPair keycert ={key, cert};
  grpc::SslServerCredentialsOptions sslOps;
  sslOps.pem_root_certs = root;
  sslOps.pem_key_cert_pairs.push_back ( keycert );
  builder.AddListeningPort(server_address, grpc::SslServerCredentials( sslOps ));
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
  plog::init(plog::debug, "../log/server_log.txt");
  /*init database*/
  Database::getInstance()->setDb("mongodb://localhost:27017", "login_system");

  // bsoncxx::builder::stream::document document{};

  // document << "hello" << "world";

  // collection.insert_one(document.view());
  // auto cursor = collection.find({});

  // for (auto&& doc : cursor) {
  //     std::cout << bsoncxx::to_json(doc) << std::endl;
  // }



  // std::cout<< user_id::getNewUserId() << std::endl;

  RunServer();


  return 0;
}
