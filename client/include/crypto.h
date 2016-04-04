
#ifndef CRYPTO_H
#define	CRYPTO_H


#include "mbedtls/gcm.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"


#define TAG_LENGTH 128
#define IV_LENGTH 16
#define KEY_LENGTH 256

//status
#define SUCCESS 0
#define AUTHENTICATION_FAILED 1

class Crypto
{
private:

    mbedtls_entropy_context entropy_ctx;
    mbedtls_ctr_drbg_context ctr_drbg_ctx;
    mbedtls_gcm_context gcm_ctx;
    
public:
    
    Crypto();
    
    /**
     * @brief This function creates HMAC for given message
     * @param message message to hash
     * @param key key for HMAC
     * @param hash created hash
     */
    void message_hash_HMAC(const char* &message, const char* &key, const char* &hash);
    
    
   
    /**
     * @brief This function creates RSA pair of keys, later public key will be sent to server to sign certificate
     * @param public_key generated public key
     * @param private_key generated private key
     * @param seed seed for random number generator
     */
    void createRSAPair(const char* &public_key, const char* &private_key, const int seed);
    
    
    
    /**
     * @brief This function creates HMAC for password + SALT
     * @param password password to hash
     * @param salt salt
     * @param key key for HMAC
     * @param hash created hash
     */
    void salt_and_password_hash_HMAC(const char* &password, const char* salt, const char* &key, const char* &hash);
    
    /**
     * @brief This function encrypts given message with AES-256
     * @param key key with size 256b for AES-256   
     * @param inputData data to encrypt
     * @param inputSize size of input data
     * @param encData encrypted data
     * @param encSize size of encrypted data(padding included)
     * @param encIv initialization vector
     */
    void encrypt(const unsigned char* message, const unsigned char* add_data, unsigned char* enc_message);
    
    
    /**
     * @brief This function decrypts encrypted message with AES-256
     * @param key key with size 256b for AES-256 
     * @param inputData data to decrypt
     * @param inputSize size of input data
     * @param decOutput decrypted data
     * @param decIv initialization vector
     */
    void decrypt(const unsigned char * message, const unsigned char * add_data, unsigned char * dec_message);
    
    
    /**
     * @brief This function encrypts message using GCM (encryption + authentication)
     * @param key key with size 256b for AES-256   
     * @param message data to encrypt
     * @param add_data additional data
     * @param enc_message this string will contain at the end of function encrypted data
     */
    void encrypt_msg(const unsigned char * key, const unsigned char* message, const unsigned char* add_data, unsigned char* enc_message);
    
    
    /**
     * @brief This function decrypts message using GCM, computes tag for received message and compare it with provided tag
     * @param key key with size 256b for AES-256   
     * @param message data to decrypt
     * @param add_data additional data
     * @param dec_message this string will contain at the end of function decrypted data
     * @return status, 0 -> SUCCESS, AUTHENTICATION_FAILED -> tags are different, authentication has failed
     */
    int decrypt_msg(const unsigned char * key, const unsigned char * message, const unsigned char * add_data, unsigned char * dec_message);
};


#endif	/* CRYPTO_H */


