//
//  main.cpp
//  login_client_cpp
//
//  Created by jingwei cai on 7/24/18.
//  Copyright © 2018 jingwei cai. All rights reserved.
//

#include <iostream>
#include "hello_world_impl.hpp"

using namespace std;
using namespace helloworld;

int main(int argc, const char * argv[]) {
    HelloWorldImpl hw = HelloWorldImpl();
    string myString = hw.get_hello_world();
    cout << myString << "\n";
    return 0;
}
