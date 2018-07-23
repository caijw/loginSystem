#ifndef CRYPTO_H
#define CRYPTO_H
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <string>

static std::string md5Enc(std::string message){
	/*todo: MD5 enc*/
	std::string res = "01234567890123456789012345678901";
	return res;
}

static std::string AESEnc(std::string message, std::string key){
	/*todo*/
	return message;
}

static std::string AESDec(std::string message, std::string key){
	/*todo*/
	return message;
}

#endif