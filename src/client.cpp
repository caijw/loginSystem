#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <fstream>
#include <grpcpp/grpcpp.h>

#ifdef BAZEL_BUILD
#include "protos/login_system.grpc.pb.h"
#else
#include "login_system.grpc.pb.h"
#endif

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using login_system::registerRequest;
using login_system::registerResponse;
using login_system::LoginSystem;

class RegisterClient {
 public:
  RegisterClient(const std::string& cert,
                const std::string& key,
                const std::string& root,
                const std::string& server)
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
    request.set_s1("S1");
    request.set_nickname("nickname");
    request.set_timestamp(123);

    // Container for the data we expect from the server.
    registerResponse response;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->registerAccount(&context, request, &response);

    // Act upon its status.
    if (status.ok()) {
      std::cout << "registerAccount" << std::endl;
      std::cout << "response.userId:" << response.user_id() << " response.timestamp:" << response.timestamp() << std::endl;
      return;
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return;
    }
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

  std::string cert;
  std::string key;
  std::string root;
  std::string server { "localhost:50051" };

  read ( "client.crt", cert );
  read ( "client.key", key );
  read ( "ca.crt", root );



  RegisterClient registerClient ( server, key, root, server );

  registerClient.registerAccount();

  int operation = 0;
  std::cout << "input your operation type, 1:[register] 2:[login] 0:[exit]: ";
  std::cin >> operation;
  switch(operation){
    case 1:
      std::cout << "register operation" << std::endl;
      break;
    case 2:
      std::cout << "login operation" << std::endl;
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
