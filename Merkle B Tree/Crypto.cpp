//////////////////////////////////////
//
// Crypto.cpp
// Ke Wang
//
// 
//////////////////////////////////////
// Version History:
// 29th, Mar, 2014 -- Created
//////////////////////////////////////

#include "Crypto.h"
#include "Tools.h"
#include <openssl\evp.h>
#include <openssl\rsa.h>
#include <openssl\pem.h>
#include <openssl\bio.h>
#include <string>

using namespace std;


Crypto::Crypto(){}

Crypto::~Crypto(){}

string Crypto::hash(string plain, string method) {
	OpenSSL_add_all_digests();

	const EVP_MD* digestMethod = EVP_get_digestbyname(method.c_str());
	if (digestMethod == NULL) {
		cout << "ERROR: Digest Name Not Found - " << method <<  endl;
		exit(1);
	}
	
	EVP_MD_CTX *context = EVP_MD_CTX_create();
	EVP_DigestInit_ex(context, digestMethod, NULL);
	EVP_DigestUpdate(context, (const void *)plain.c_str(), plain.size());
	unsigned char md[EVP_MAX_MD_SIZE];
	unsigned int mdLength;
	EVP_DigestFinal_ex(context, md, &mdLength);
	
	string digestResult((const char*)md, mdLength);
	EVP_MD_CTX_destroy(context);
	
	return digestResult;
}

void Crypto::printHashValue(string s) {
	int stringLength = s.length();
	for (int i = 0;i != stringLength;i++) {
		printf("%02x",(unsigned char)s[i]);
	}
}

void Crypto::generateRSAKeys(std::string name, int bits, int eInt) {
	RSA *rsa = RSA_new();
	BIGNUM *e = BN_new();
	BN_set_word(e, eInt);

	// the main function to generate a pair of RSA keys
	RSA_generate_key_ex(rsa, bits, e, NULL);

	// output the keys to files
	std::string privateKeyFilename = name + ".private";
	std::string publicKeyFilename = name + ".public";

	BIO* privateBIO = BIO_new_file(privateKeyFilename.c_str(),"wt");
	PEM_write_bio_RSAPrivateKey(privateBIO, rsa, NULL, NULL, 0, NULL, NULL);
	BIO_free(privateBIO);

	BIO* publicBIO = BIO_new_file(publicKeyFilename.c_str(), "wt");
	PEM_write_bio_RSAPublicKey(publicBIO, rsa);
	BIO_free(publicBIO);
}

std::string Crypto::signWithPrivateKey(string name, string hashValue) {
	// open the file and get the private key
	string fullfilename = name + ".private";
	BIO *bio = BIO_new_file(fullfilename.c_str(),"rt");
	RSA *rsa = RSA_new();
	PEM_read_bio_RSAPrivateKey(bio,&rsa,NULL,NULL);
	BIO_free(bio);

	if (hashValue.length() >= RSA_size(rsa)) {
		cout << "The hash value length is not excpted\n";
		exit(1);
	}
	
	unsigned char *result =  (unsigned char *)OPENSSL_malloc(RSA_size(rsa));
	RSA_private_encrypt(hashValue.length(),(const unsigned char*)hashValue.c_str(),result,rsa,RSA_PKCS1_PADDING);

	string resultString((char*)result,RSA_size(rsa));
	return resultString;
}

bool Crypto::verifyWithPublicKey(string name, string encryptedValue, string hashValue) {
	string fullfilename = name + ".public";
	BIO *bio = BIO_new_file(fullfilename.c_str(),"rt");
	RSA *rsa = RSA_new();
	PEM_read_bio_RSAPublicKey(bio,&rsa,NULL,NULL);
	BIO_free(bio);

	unsigned char *result = (unsigned char *)OPENSSL_malloc(RSA_size(rsa));
	int resultLen = RSA_public_decrypt(RSA_size(rsa),(const unsigned char *)encryptedValue.c_str(),result,rsa,RSA_PKCS1_PADDING);
	result[resultLen] = 0;
	string resultString((char *)result);
	
	return resultString == hashValue;
}