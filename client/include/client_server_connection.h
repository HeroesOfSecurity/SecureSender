/* 
 * File:   client_server_connection.h
 * Author: pedro1
 *
 * Created on March 20, 2016, 11:53 AM
 */

#ifndef CLIENT_SERVER_CONNECTION_H
#define	CLIENT_SERVER_CONNECTION_H


#include <string>

class ClientServerConnection
{
private:
    std::string token;
public:
    
    /**
     * @brief This function generates token
     * @return token
     */
    std::string generateToken();
};

#endif	/* CLIENT_SERVER_CONNECTION_H */

