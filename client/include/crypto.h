/* 
 * File:   crypto.h
 * Author: pedro1
 *
 * Created on March 15, 2016, 9:19 PM
 */

#ifndef CRYPTO_H
#define	CRYPTO_H



class Crypto
{
    
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
    void encrypt(unsigned char* key, const unsigned char* inputData, int inputSize, 
                    unsigned char* encData, int & encSize, unsigned char * encIv);
    
    
    /**
     * @brief This function decrypts encrypted message with AES-256
     * @param key key with size 256b for AES-256 
     * @param inputData data to decrypt
     * @param inputSize size of input data
     * @param decOutput decrypted data
     * @param decIv initialization vector
     */
    void decrypt(unsigned char* key, const unsigned char * inputData, int inputSize,
                    unsigned char * decOutput, unsigned char * decIv);
    
};


#endif	/* CRYPTO_H */


