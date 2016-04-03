/* 
 * File:   server_api.h
 * Author: pedro1
 *
 * Created on March 20, 2016, 11:44 AM
 */

#ifndef SERVER_API_H
#define	SERVER_API_H

#include <string>
#include <vector>

class Server
{
public:
    
    /**
     * @brief This function tries to register new user to database
     * @param username username of new user
     * @param password password if new user
     * @return status, if registration was succesful
     */
    int register_new_user(std::string username, std::string password);
    
    /**
     * @brief This function tries to authenticate user
     * @param username username of authenticated user
     * @param hash password hash of authenticated user
     * @return status, if authentication was succesful
     */
    int authenticate(std::string username, std::string hash);
    
    /**
     * @brief This function returns list of online users
     * @param username username for which we try to find online users
     * @return list of online users
     */
    std::vector<std::string> online_users(std::string username);
};


#endif	/* SERVER_API_H */

