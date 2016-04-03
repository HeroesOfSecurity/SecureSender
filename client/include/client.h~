/* 
 * File:   client.h
 * Author: pedro1
 *
 * Created on March 15, 2016, 8:39 PM
 */

#ifndef CLIENT_H
#define	CLIENT_H

#include <string.h>
#include <vector>

class Client
{
private:
    std::string ip_address;
    std::string server_ip;
    
public:
    
    /**
     * @brief This function registers new user on the server.
     * @param username
     * @param password
     * @param status of the registration
     */
    void sign_up(std::string username, std::string password, int *status);
    
    /**
     * @brief This function log in user on the server.
     * @param username
     * @param password
     * @param status of the login
     * @return token from the server
     */
    char* sign_in(std::string username, std::string password, int *status);
    
    /**
     * @brief This function gives you list of online users.
     * @return online users
     */
    std::vector<User> get_online_users();
    
    /**
     * @brief This function tries to establish connection and chat with certain user.
     * @param user
     * @param status of the chat 
     */
    Chat start_chat(User user, int *status);
    
    /**
     * @brief This function logs off the user.
     */
    void disconnect();
};

