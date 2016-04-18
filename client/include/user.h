/* 
 * File:   user.h
 * Author: pedro1
 *
 * Created on March 15, 2016, 9:07 PM
 */

#ifndef USER_H
#define	USER_H

#include <string>

class User
{
private:
    
    std::string username;
    std::string ip_address;
    std::string certificate;
    
public:
    User() {};
    User(std::string u, std::string ip) : username(u), ip_address(ip){ }
    
};

#endif	/* USER_H */


