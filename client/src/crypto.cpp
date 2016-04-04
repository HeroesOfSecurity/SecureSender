#include "crypto.h"
#include "stdio.h"
#include <cstring>
#include <sstream>
#include <iostream>

Crypto::Crypto()
{
    mbedtls_gcm_init(&gcm_ctx);
    mbedtls_entropy_init( &entropy_ctx );
    
    const char *personalization = "v87sNR45EJgU3UOtnxFkmDZgV6V3xX3f";
//    int ret = mbedtls_ctr_drbg_init( &ctr_drbg_ctx, mbedtls_entropy_func, &entropy_ctx,
//                     (const unsigned char *) personalization,
//                     strlen( personalization ) );
//    if( ret != 0 )
//    {
//        // ERROR HANDLING CODE FOR YOUR APP
//        printf("Error when initializing ctr_drbg context");
//    }
    mbedtls_ctr_drbg_init( &ctr_drbg_ctx);
    mbedtls_ctr_drbg_seed(&ctr_drbg_ctx, mbedtls_entropy_func, &entropy_ctx, (const unsigned char *)personalization, strlen(personalization));
}


/**
     * @brief This function encrypts given message with AES-256
     * @param key key with size 256b for AES-256   
     * @param inputData data to encrypt
     * @param inputSize size of input data
     * @param encData encrypted data
     * @param encSize size of encrypted data(padding included)
     * @param encIv initialization vector
     */
void Crypto::encrypt_msg(const unsigned char * key, const unsigned char* message, const unsigned char* add_data,
                unsigned char* enc_message)
{
    unsigned char iv[IV_LENGTH];
    unsigned char tag[TAG_LENGTH];
    enc_message = new unsigned char[strlen((const char*)message) + IV_LENGTH + TAG_LENGTH + 1];
    unsigned char *output = new unsigned char[strlen((const char*)message)];
    mbedtls_ctr_drbg_random(&ctr_drbg_ctx, iv, IV_LENGTH);
    
    mbedtls_gcm_setkey(&gcm_ctx, MBEDTLS_CIPHER_ID_AES, key, 256);
    mbedtls_gcm_crypt_and_tag(&gcm_ctx, MBEDTLS_GCM_ENCRYPT, strlen((const char*)message), iv, 
            IV_LENGTH, add_data, strlen((const char*)add_data), message, output, TAG_LENGTH, tag);
    
    memcpy(enc_message, iv, IV_LENGTH);
    memcpy(enc_message + IV_LENGTH, tag, TAG_LENGTH);
    memcpy(enc_message + IV_LENGTH + TAG_LENGTH, output, strlen((const char*)output));
    enc_message[strlen((const char*)enc_message)] = '\0';
    
    std::cout << "iv -> ";
    for(int i = 0; i < IV_LENGTH; i++)
    {
        std::cout << iv[i];
    }
    std::cout << std::endl;
    
    std::cout << "tag -> ";
    for(int i = 0; i < TAG_LENGTH; i++)
    {
        std::cout << tag[i];
    }
    std::cout << std::endl;
    
    std::cout << "output -> ";
    for(int i = 0; i < strlen((const char*)output); i++)
    {
        std::cout << output[i];
    }
    std::cout << std::endl;
    
    std::cout << "enc_message -> ";
    for(int i = 0; i < IV_LENGTH + TAG_LENGTH + strlen((const char*)output); i++)
    {
        std::cout << enc_message[i];
    }
    std::cout << std::endl;
    
}


int Crypto::decrypt_msg(const unsigned char * key, const unsigned char * message, const unsigned char * add_data, 
              unsigned char * dec_message)
{
    // message contains {message || iv || tag}
    size_t message_length = strlen((const char*)message) - IV_LENGTH - TAG_LENGTH;
    unsigned char iv[IV_LENGTH];
    unsigned char tag[TAG_LENGTH];
    unsigned char input_message[message_length];
    dec_message = new unsigned char[message_length];
    
    memcpy(iv, message, IV_LENGTH);
    memcpy(tag, message + IV_LENGTH, TAG_LENGTH);
    memcpy(input_message, message + IV_LENGTH + TAG_LENGTH, message_length);
    
    mbedtls_gcm_setkey(&gcm_ctx, MBEDTLS_CIPHER_ID_AES, key, KEY_LENGTH);
    if (mbedtls_gcm_auth_decrypt(&gcm_ctx, message_length, iv, IV_LENGTH, add_data, strlen((const char*)add_data), tag, TAG_LENGTH, input_message, dec_message) == MBEDTLS_ERR_GCM_AUTH_FAILED) {
	printf("Authentication has failed because computed and provided tags are different.");
        return AUTHENTICATION_FAILED;
    }
    
    return SUCCESS;
}