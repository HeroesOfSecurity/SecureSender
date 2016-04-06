#include "crypto.h"
#include "stdio.h"
#include "client.h"
#include <cstring>
#include <string>
#include <sstream>
#include <iostream>

Crypto::Crypto()
{
    mbedtls_gcm_init(&gcm_ctx);
    mbedtls_entropy_init( &entropy_ctx );
    
    const char *personalization = "v87sNR45EJgU3UOtnxFkmDZgV6V3xX3f";

    mbedtls_ctr_drbg_init( &ctr_drbg_ctx);
    mbedtls_ctr_drbg_seed(&ctr_drbg_ctx, mbedtls_entropy_func, &entropy_ctx, (const unsigned char *)personalization, strlen(personalization));
}





void Crypto::protect_msg(const unsigned char * key, const unsigned char * iv, 
        const unsigned char* message, int original_size, const unsigned char* add_data, int add_data_size, unsigned char** enc_message)
{
//    unsigned char iv[IV_LENGTH];
    unsigned char tag[TAG_LENGTH];
    
    unsigned char *tmp_enc_msg = new unsigned char[original_size + IV_LENGTH + TAG_LENGTH + sizeof(int)]();
    unsigned char *output = new unsigned char[original_size]();
//    mbedtls_ctr_drbg_random(&ctr_drbg_ctx, iv, IV_LENGTH);

    mbedtls_gcm_setkey(&gcm_ctx, MBEDTLS_CIPHER_ID_AES, key, KEY_LENGTH * 8);
    if( mbedtls_gcm_crypt_and_tag(&gcm_ctx, MBEDTLS_GCM_ENCRYPT, original_size, iv, 
            IV_LENGTH, add_data, add_data_size, message, output, TAG_LENGTH, tag) != 0)
    {
        printf("Encryption of GCM has failed.\n");
    }
    
    memcpy(tmp_enc_msg, &original_size, sizeof(int));
    memcpy(tmp_enc_msg + sizeof(int), iv, IV_LENGTH);
    memcpy(tmp_enc_msg + sizeof(int) + IV_LENGTH, tag, TAG_LENGTH);
    memcpy(tmp_enc_msg + sizeof(int) + IV_LENGTH + TAG_LENGTH, output, original_size);
    
    *enc_message = tmp_enc_msg;
}


int Crypto::unprotect_msg(const unsigned char * key, const unsigned char * enc_message, 
        const unsigned char * add_data, unsigned char ** dec_message)
{
    // add_data contains {add_data_size || add_data}   
    int add_data_size = *((int *)add_data); 
    
    // parse enc_message
    // enc_message contains {original_size || iv || tag || message}
    int original_size = *((int *)enc_message);
    if(original_size > MAX_MESSAGE_SIZE)
    {
        return ERR_MSG_SIZE;
    }
    
    unsigned char iv[IV_LENGTH];
    unsigned char tag[TAG_LENGTH];
    unsigned char input_message[original_size];
    memcpy(iv, enc_message + sizeof(int), IV_LENGTH);
    memcpy(tag, enc_message + sizeof(int) + IV_LENGTH, TAG_LENGTH);
    memcpy(input_message, enc_message + sizeof(int) + IV_LENGTH + TAG_LENGTH, original_size);
    
    unsigned char *tmp_dec_msg = new unsigned char[original_size];
    
    mbedtls_gcm_setkey(&gcm_ctx, MBEDTLS_CIPHER_ID_AES, key, KEY_LENGTH * 8);
    int status = mbedtls_gcm_auth_decrypt(&gcm_ctx, original_size, iv, IV_LENGTH, add_data + sizeof(int), add_data_size, tag, TAG_LENGTH, input_message, tmp_dec_msg);
    if (status == MBEDTLS_ERR_GCM_AUTH_FAILED) {
	printf("Authentication has failed because computed and provided tags are different.\n");
        return ERR_AUTHENTICATION_FAILED;
    }
    
    *dec_message = tmp_dec_msg;
    return SUCCESS;
}

int Crypto::generate_init_vector(size_t iv_len, unsigned char** init_vector)
{
    unsigned char *iv = new unsigned char[iv_len];
    if(mbedtls_ctr_drbg_random(&ctr_drbg_ctx, iv, iv_len) != 0)
    {
        return 1;
    }
    *init_vector = iv;
    return SUCCESS;
}
