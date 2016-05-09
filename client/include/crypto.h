
#ifndef CRYPTO_H
#define	CRYPTO_H


#include "mbedtls/gcm.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"


#define TAG_LENGTH 16
#define IV_LENGTH 12
#define KEY_LENGTH 32
#define MAX_MESSAGE_SIZE 500

//status
#define SUCCESS 0
#define ERR_AUTHENTICATION_FAILED 1
#define ERR_GENERATE_IV 2
#define ERR_MSG_SIZE 3


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
     * @param iv initializing vector   
     * @param message data to encrypt
     * @param original_size size of message
     * @param add_data additional data
     * @param add_data_size size of additional data
     * @param enc_message this string will contain at the end of function encrypted data
     *  !!! string is allocated with "new" so you have to call "delete[]" at the end of string's usage
     */
    void protect_msg(const unsigned char * key, const unsigned char * iv, const unsigned char* message, int original_size,
            const unsigned char* add_data, int add_data_size, unsigned char** enc_message);
    
    
    /**
     * @brief This function decrypts message using GCM assuring integrity of data
     * @param key key with size 256b for AES-256
     * @param message data to decrypt {original_size(size of body) || iv || tag || body_of_msg}
     * @param add_data additional data
     * @param dec_message this string will contain at the end of function decrypted data,
     *  !!! string is allocated with "new" so you have to call "delete[]" at the end of string's usage
     * @return status, SUCCESS -> unprotecting message was succesful
     *                 ERR_AUTHENTICATION_FAILED -> tags are different, authentication has failed
     *                 ERR_MSG_SIZE -> message is too big
     */
    int unprotect_msg(const unsigned char * key, const unsigned char * message,
            const unsigned char * add_data, unsigned char ** dec_message);
    
    
    /**
     * @brief This function generates random initialized vector with given size (in bytes)
     * @param iv_length length of generated IV
     * @return status, SUCCESS -> generating was succesful, ERR_GENERATE_IV -> fail
     */
    int generate_init_vector(size_t iv_length, unsigned char ** init_vector);


    /**
     * @brief generate_aes_key Function generates aes key
     * @param key generated key
     * @param key_length length of key
     */
    void generate_aes_key(unsigned char* key, int key_length);
};


#endif	/* CRYPTO_H */


