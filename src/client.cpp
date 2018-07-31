#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <fstream>
#include <grpc++/grpc++.h>

#include "crypto_impl.hpp"

#include "protos/login_system.pb.h"
#include "protos/login_system.grpc.pb.h"

#include "util.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;
using login_system::helloRequest;
using login_system::helloResponse;
using login_system::registerRequest;
using login_system::registerResponse;
using login_system::loginRequest;
using login_system::loginResponse;
using login_system::logoutRequest;
using login_system::logoutResponse;
using login_system::LoginSystem;

#define LOGD std::cout 

std::string global_ST;

class Client {
 public:
  Client(int argc, char** argv)
  {

    for(int i = 1; i < argc; i++){
      if(std::string(argv[i]) == "--useSSL=1"){
        useSSL = true;
      }
    }
    std::string server { "localhost:50051" };

    if(useSSL){
      std::cout << "use SSL" << std::endl;
      std::string cert;
      std::string key;
      std::string root;
      readFile( "ca.crt", root );
      grpc::SslCredentialsOptions opts =
      {
        root
      };

      stub_ = LoginSystem::NewStub (
        grpc::CreateChannel (server,
          grpc::SslCredentials ( opts ) ) );
    }else{
      std::cout << "not use SSL" << std::endl;
      stub_ = LoginSystem::NewStub (
        grpc::CreateChannel (server,
          grpc::InsecureChannelCredentials() ) );
    }

  }

  void hello(){
    helloRequest request;
    request.set_helloclient("hello form cpp");
    helloResponse response;
    ClientContext context;
    Status status = stub_->hello(&context, request, &response);
    if (status.ok()) {
      std::cout << "hello response: " << response.helloserver() << std::endl;
      return;
    } else {
      std::cout << status.error_code() << ": " << status.error_message() << std::endl;
      return;
    }
  }

  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  void registerAccount() {
    // Data we are sending to the server.
    crypto::CryptoImpl crypto_util;
    registerRequest request;
    std::string nickname;
    std::string phone_num;
    std::string password;

    std::cout << "enter your nickname: ";
    std::cin >> nickname;
    std::cout << "enter your phone number: ";
    std::cin >> phone_num;
    std::cout << "enter your password: ";
    std::cin >> password;

    int tCost = 100;
    int mCost = 100;
    int parallelism = 100;
    std::string salt = "";
    std::string hash = crypto_util.argon2(password, tCost, mCost, parallelism, salt);


    request.set_hash(hash);
    request.set_salt(salt);
    request.set_nickname(nickname);
    request.set_phone_num(phone_num);

    // Container for the data we expect from the server.
    registerResponse response;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->registerAccount(&context, request, &response);

    // Act upon its status.
    if (status.ok()) {
      int ret = response.ret();
      std::string msg = response.msg();
      std::string user_id = response.user_id();
      if(ret == 0){
        std::cout << "register account success" << std::endl;
        std::cout << "response.userId:" <<  user_id << std::endl;
      }else{
        std::cout << "register account fail" << std::endl;
      }

      return;
    } else {
      std::cout << status.error_code() << ": " << status.error_message() << std::endl;
      return;
    }
  }

  void loginAccount(){
    crypto::CryptoImpl crypto_util;
    loginRequest request;
    std::string user_id;
    std::string password;

    std::cout << "enter your userId: ";
    std::cin >> user_id;
    std::cout << "enter your password: ";
    std::cin >> password;

    int tCost = 100;
    int mCost = 100;
    int parallelism = 100;
    std::string salt = "";
    std::string hash = crypto_util.argon2(password, tCost, mCost, parallelism, salt);

    std::string tmp_user_id = crypto_util.stringWithFixedLength(user_id, 10, "x");
    std::string timestamp = std::to_string(currentTimeSecond());
    timestamp = crypto_util.stringWithFixedLength(timestamp, 10, "x");

    std::string data = crypto_util.AESEnc(tmp_user_id + timestamp, hash);


    request.set_user_id(user_id);
    request.set_data(data);

    loginResponse response;
    ClientContext context;

    std::unique_ptr<ClientReader<loginResponse> > reader(
        stub_->loginAccount(&context, request));
    while (reader->Read(&response)) {

      int ret = response.ret();
      std::string ST = response.st();
      std::string msg = response.msg();
      bool logout = response.logout();

      LOGD << "ret: " << ret << " msg: " << msg << " st: " << ST << " logout: " << logout << std::endl;

      if(logout){
        LOGD << "go logout" << std::endl;
        break;
      }
    }
    Status status = reader->Finish();
    if (status.ok()) {
      std::cout << "loginAccount rpc succeeded." << std::endl;
    } else {
      std::cout << "loginAccount rpc failed." << std::endl;
    }

  }
  void logout(){
    std::string user_id;
    std::cout << "please input the user to logout: ";
    std::cin >> user_id;
    logoutRequest request;
    request.set_user_id(user_id);
    logoutResponse response;
    ClientContext context;
    Status status = stub_->logout(&context, request, &response);
    if (status.ok()) {
      std::cout << "logout Finish. ret: " << response.ret() << " msg: " << response.msg();
      return;
    } else {
      std::cout << status.error_code() << ": " << status.error_message() << std::endl;
      return;
    }

  }
 private:
  std::unique_ptr<LoginSystem::Stub> stub_;
  bool useSSL;
};



int main(int argc, char** argv) {

  Client client (argc, argv);

  int operation = 0;
  std::cout << "input your operation type, 1:[login] 2:[logout] 3:[hello] 0:[exit]: ";
  std::cin >> operation;
  switch(operation){
    case 1:
      std::cout << "login operation" << std::endl;
      client.loginAccount();
      break;
    case 2:
      std::cout << "logout operation" << std::endl;
      client.logout();
      break;
    case 3:
      std::cout << "hello operation" << std::endl;
      client.hello();
      break;
    case 0:
      std::cout << "exit operation" << std::endl;
      exit(0);
      break;
    default:
      std::cout << "unknow operation, exit!" << std::endl;
      exit(0);
      break;
  }




  return 0;
}
