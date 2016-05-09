/* 
 * File:   password_store.h
 * Author: pedro1
 *
 * Created on March 20, 2016, 10:50 AM
 */



#ifndef PASSWORD_STORE_H
#define	PASSWORD_STORE_H

#include <string>
#include <cstring>



#define SALT_SIZE 32
#define HASH_SIZE 64


#include <mbedtls/pk.h>

class Crypto
{
private:
    
    int iteration_no_pbfkd2;
    
public:
    
    Crypto();
    
    /**
     * @brief Function generates salt for password
     * @return salt
     */
    int generate_salt(unsigned char *salt);
    
    int perform_pbkdf2(std::string& password, unsigned char *salt, std::string& output_hash);
    
    
    /**
     * @brief This function computes SHA-512 hash on given string
     * @param string string to hash
     * @param sha_result hash of string
     */
    void hash(unsigned char *string, unsigned char* sha_result);


    int generateRSAKey(mbedtls_pk_context& rsa_ctx);
    
};

#endif	/* PASSWORD_STORE_H */

