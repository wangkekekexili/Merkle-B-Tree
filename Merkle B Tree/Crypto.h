//////////////////////////////////////
//
// Crypto.h
// Ke Wang
// 
// To Provide a wrapper for the 
// encryption and hash functions
// using openssl
//
//////////////////////////////////////
// Version History:
// 30th, Mar, 2014 -- Add RSA facilities
// 29th, Mar, 2014 -- Created
//
//////////////////////////////////////

#ifndef _CRYPTO_H_
#define _CRYPTO_H_

#include <openssl\rsa.h>
#include <openssl\evp.h>

#include <string>




class Crypto {
public:
	Crypto();
	~Crypto();
	
	static std::string hash(std::string text, std::string method);
	static void printHashValue(std::string);

	// generate a pair of RSA keys using openssl
	//
	// the pair of keys will be saved separetedly into files
	// naming name.private and name.public
	//
	// 'bits' is for the length of modulus 
	// 'e' is for public exponent
	static void generateRSAKeys(std::string name, int bits = 1024, int e = 65537);

	// sign a hash value with the private key
	// from the 'name.private' file
	// Using OPENSSL
	static std::string signWithPrivateKey(std::string name, std::string hashValue);

	// verify the value with the public key
	// 
	// return true if veification passes
	// and false if not
	static bool verifyWithPublicKey(std::string name, std::string encryptedValue, std::string hashValue);
};

#endif // _CRYPTO_H_