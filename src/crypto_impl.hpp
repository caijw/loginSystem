#pragma once
 
#include "crypto.hpp"
 
namespace crypto {
    
    class CryptoImpl : public crypto::Crypto {
        
    public:
        
        // Constructor
        CryptoImpl();
        
        // Our method that returns a string
        std::string get_hello_world();
        
    };
    
}