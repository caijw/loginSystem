#ifndef CRYPTO_H
#define CRYPTO_H
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <string>
#include <md5.h>
#include <hex.h>

static std::string getMd5(std::string message){
	CryptoPP::byte digest[ CryptoPP::Weak::MD5::DIGESTSIZE ];

	CryptoPP::Weak::MD5 hash;
	hash.CalculateDigest( digest, (const CryptoPP::byte*)message.c_str(), message.length() );

	CryptoPP::HexEncoder encoder;
	std::string output;

	encoder.Attach( new CryptoPP::StringSink( output ) );
	encoder.Put( digest, sizeof(digest) );
	encoder.MessageEnd();

	return output;
}

#endif