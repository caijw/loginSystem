#pragma once
 
#include "crypto.hpp"

namespace crypto {
    
    class CryptoImpl : public crypto::Crypto {
        
    public:
        
        // Constructor
        CryptoImpl();
        //a hello world for test
        std::string get_hello_world();

        std::string argon2(const std::string & message, int32_t t_cost, int32_t m_cost, int32_t parallelism, const std::string & salt);

        std::string AESEnc(const std::string & message, const std::string & key);

        std::string AESDec(const std::string & message, const std::string & key);

        std::string stringWithFixedLength(const std::string & origin, int32_t length, const std::string & padding);

        std::string deStringWithFixedLength(const std::string & origin, const std::string & padding);
        
    };
    
}