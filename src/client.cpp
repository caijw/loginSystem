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
using login_system::LoginSystem;

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
    std::string phone_num;
    std::string password;
    unsigned int timestamp = currentTimeSecond();
    std::cout << "enter your phone number: ";
    std::cin >> phone_num;
    std::cout << "enter your password: ";
    std::cin >> password;

    LOGD << "phone_num: " << phone_num << " password: " << password << " timestamp: " << timestamp;

    request.set_s1("S1");
    request.set_nickname("nickname");
    request.set_timestamp(timestamp);

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
      unsigned int timestamp = response.timestamp();
      if(ret == 0){
        std::cout << "registerAccount success" << std::endl;
        std::cout << "response.userId:" <<  user_id << " response.timestamp:" << timestamp << std::endl;
      }else{
        std::cout << "registerAccount fail" << std::endl;
      }

      return;
    } else {
      std::cout << status.error_code() << ": " << status.error_message() << std::endl;
      return;
    }
  }
  void loginAccount(){

  }
 private:
  std::unique_ptr<LoginSystem::Stub> stub_;
};

void
read ( const std::string& filename, std::string& data )
{
  std::ifstream file ( filename.c_str (), std::ios::in );

  if ( file.is_open () )
  {
    std::stringstream ss;
    ss << file.rdbuf ();

    file.close ();

    data = ss.str ();
  }

  return;
}


int main(int argc, char** argv) {

  plog::init(plog::debug, "../log/client_log.txt");

  std::string cert;
  std::string key;
  std::string root;
  std::string server { "localhost:50051" };

  read ( "ca.crt", root );

  LOGD << getMd5("hello world");

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
