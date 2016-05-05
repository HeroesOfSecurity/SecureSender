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


class Client : public QTcpServer
{
    Q_OBJECT
private:
    std::vector<User> online_users;
    std::string username;

public:

    Client(std::string username) : username(username) {
        if (!this->listen(QHostAddress::Any, 8083)) {
            qDebug() << "Client start failed";
            qDebug() << this->errorString();
            emit finished();
            exit(0);
        }
    }

    //Client(QObject &a);
    ~Client() {}
    std::string getUsername() { return username;}

    void setOnlineUsers(std::vector<User> &users) {
        for(int i = 0; i < users.size(); i++){
            online_users.push_back(users[i]);
        }
    }

    std::string getIP(std::string name);

    /**
     * @brief This function tries to establish connection and chat with certain user.
     * @param user
     * @param status of the chat 
     */
    void startChat(std::string ip);
    
protected:

    /**
     * @brief incomingConnection Method is evoked when new connection is established
     * Method starts new thread which handles communication with client
     * @param handle socket descriptor assigned to new connection
     */
    void incomingConnection(qintptr handle) override;

signals:

    /**
     * @brief finished Signal emited when the server was closed by error
     */
    void finished();



};

#endif 
