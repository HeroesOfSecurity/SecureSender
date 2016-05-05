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
#include <algorithm>


#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8082


class Client
{
private:
    std::vector<User> online_users;
    std::string username;

public:

    Client(std::string username) : username(username) {

}
    //Client(QObject &a);
    ~Client() {};
    std::string getUsername() { return username;}
    void setOnlineUsers(std::vector<User> &users) {
        for(int i = 0; i < users.size(); i++){
            online_users.push_back(users[i]);
        }
    }
    /**
     * @brief This function tries to establish connection and chat with certain user.
     * @param user
     * @param status of the chat 
     */
//    Chat start_chat(User user, int *status);
    






};

#endif 
