/* 
 * File:   password_store.h
 * Author: pedro1
 *
 * Created on March 20, 2016, 10:50 AM
 */



#ifndef PASSWORD_STORE_H
#define	PASSWORD_STORE_H

#include <string>
#include <SimpleIni.h>



class PasswordStore
{
private:
    
    //Database
    CSimpleIniA iniFile;
    int iteration_no_pbfkd2;
    
public:
    
    PasswordStore();
    
    /**
     * @brief Function generates salt for password
     * @return salt
     */
    std::string generate_salt();
    
    /**
     * @brief Function performs PBKFD2 algorithm on hash and stores it in INI file
     * @return status
     */
    int store_password_hash(std::string hash);
    
};

#endif	/* PASSWORD_STORE_H */

