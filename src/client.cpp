#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <fstream>
#include <grpcpp/grpcpp.h>
#include "login_system.grpc.pb.h"
#include "util.h"
#include "crypto.h"
#include <plog/Log.h> 
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using login_system::registerRequest;
using login_system::registerResponse;
using login_system::loginRequest;
using login_system::loginResponse;
using login_system::LoginSystem;

std::string global_ST;

class Client {
 public:
  Client(const std::string& root, const std::string& server)
  {
    grpc::SslCredentialsOptions opts =
    {
      root
    };

    stub_ = LoginSystem::NewStub ( grpc::CreateChannel (
      server, grpc::SslCredentials ( opts ) ) );
  }

  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  void registerAccount() {
    // Data we are sending to the server.
    registerRequest request;
    std::string nickname;
    std::string phone_num;
    std::string password;

    std::cout << "enter your phone nickname: ";
    std::cin >> nickname;
    std::cout << "enter your phone number: ";
    std::cin >> phone_num;
    std::cout << "enter your password: ";
    std::cin >> password;

    std::string H1 = md5Enc(password);

    request.set_h1(H1);
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
    loginRequest request;
    std::string user_id;
    std::string password;

    std::cout << "enter your phone userId: ";
    std::cin >> user_id;
    std::cout << "enter your password: ";
    std::cin >> password;

    std::string H1 = md5Enc(password);
    std::string S1 = md5Enc(password + user_id);

    request.set_user_id(user_id);
    request.set_s1(S1);

    // Container for the data we expect from the server.
    loginResponse response;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->loginAccount(&context, request, &response);

    // Act upon its status.
    if (status.ok()) {
      int ret = response.ret();
      std::string msg = response.msg();
      if(ret == 0){
        std::string ST = AESDec(response.st(), S1);
        global_ST = ST;
        std::cout << "login account success" << std::endl;
        std::cout << "response.st:" <<  user_id << std::endl;
      }else{
        std::cout << "login account fail" << std::endl;
      }

      return;
    } else {
      std::cout << status.error_code() << ": " << status.error_message() << std::endl;
      return;
    }
  }
 private:
  std::unique_ptr<LoginSystem::Stub> stub_;
};



int main(int argc, char** argv) {

  plog::init(plog::debug, "../log/client_log.txt");

  std::string cert;
  std::string key;
  std::string root;
  std::string server { "localhost:50051" };

  readFile( "ca.crt", root );

  Client client (root, server);

  

  int operation = 0;
  std::cout << "input your operation type, 1:[register] 2:[login] 0:[exit]: ";
  std::cin >> operation;
  switch(operation){
    case 1:
      std::cout << "register operation" << std::endl;
      client.registerAccount();
      break;
    case 2:
      std::cout << "login operation" << std::endl;
      client.loginAccount();
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
