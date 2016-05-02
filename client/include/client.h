/* 
 * File:   client.h
 * Author: pedro1
 *
 * Created on March 15, 2016, 8:39 PM
 */

#ifndef CLIENT_H
#define	CLIENT_H

#include <string>
#include <string.h>
#include <vector>
#include <QTcpServer>
#include "user.h"
#include "session.h"
//#include <chat.h>
#include <qt5/QtNetwork/QTcpSocket>
#include <qt5/QtCore/QCoreApplication>
#include <qt5/QtCore/QJsonObject>
enum Status {OK, W_USR, W_PSWD};


#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8082


class Client
{
private:
    QObject &parent;

    //socket with server
    QTcpSocket *server_soc;

    std::string ip_address;
    std::string server_ip;
    unsigned char *token;


public:

    //Client();
    Client(QObject &a);
    ~Client();



    /**
     * @brief This function connects client to server
     * @return true -> connection succesful, false -> fail
     */
    bool connect_to_server();

    /**
     * @brief This function disconnects client from server
     */
    void disconnect();

    /**
     * @brief This function tells you if you're connected
     */
    bool is_connected();
    
    /**
     * @brief This function registers new user on the server.
     * @param username
     * @param password
     * @ret status of the registration
     */
    int sign_up(std::string username, std::string password);
    
    /**
     * @brief This function log in user on the server.
     * @param username
     * @param password
     * @param token from the server
     * @return status of the authentication
     */
    int sign_in(std::string &username, std::string &password);
    
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
//    Chat start_chat(User user, int *status);
    



private:

    void send(QJsonObject &mes);
    QJsonObject respond();


};

#endif 
