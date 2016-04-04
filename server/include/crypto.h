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


#define SALT_SIZE 64
#define HASH_SIZE 64


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
    int generate_salt(std::string& salt);
    
    int perform_pbkdf2(std::string& password, std::string& salt, std::string& output_hash);
    
    
    /**
     * @brief Function performs PBKFD2 algorithm on hash and stores it in INI file
     * @return status
     */
    int store_password_hash(std::string& salt);
    
    
    /**
     * @brief This function computes SHA-512 hash on given string
     * @param string string to hash
     * @param sha_result hash of string
     */
    void hash(unsigned char *string, unsigned char* sha_result);
    
};

#endif	/* PASSWORD_STORE_H */

