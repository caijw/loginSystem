#include "crypto_impl.hpp"
#include <string>

namespace crypto {
    
    std::shared_ptr<Crypto> Crypto::create() {
        return std::make_shared<CryptoImpl>();
    }
    
    CryptoImpl::CryptoImpl() {
 
    }
    
    std::string CryptoImpl::get_hello_world() {
        std::string myString = "Hello World! ";
        return myString;
        
    }

    std::string CryptoImpl::argon2(const std::string & message, int32_t t_cost, int32_t m_cost, int32_t parallelism, const std::string & salt){
        /*todo: argon2 hash*/
        std::string res = message;
        return res;
    }

    std::string CryptoImpl::AESEnc(const std::string & message, const std::string & key){
        /*todo aes encode*/
        std::string res = key + message;
        return res;
    }

    std::string CryptoImpl::AESDec(const std::string & message, const std::string & key){
        /*todo aes decode*/
        std::string tmp = message.substr(0, key.size());
        std::string res = message.substr(key.size(), message.size());
        if(tmp == key){
            return res;
        }else{
            return "00000000000000000000000000000000000000";
        }
    }

    std::string CryptoImpl::stringWithFixedLength(const std::string & origin, int32_t length, const std::string & padding){
        std::string res = origin;
        for(decltype(res.size()) i = 0; i < (length - origin.size()); ++i){
          res = padding + res;
        }
        return res;
    }

    std::string CryptoImpl::deStringWithFixedLength(const std::string & origin, const std::string & padding){
        size_t tmp = 0;
        tmp = origin.find_first_not_of(padding[0], 0);
        std::string res = origin.substr(tmp, origin.size() - tmp);
        return res;
    }
    
}
