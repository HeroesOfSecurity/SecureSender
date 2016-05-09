/* 
 * File:   client_server_connection.h
 * Author: pedro1
 *
 * Created on March 20, 2016, 11:53 AM
 */

#ifndef CLIENT_SERVER_CONNECTION_H
#define	CLIENT_SERVER_CONNECTION_H


#include <string>
#include <QObject>
#include <QSslSocket>
#include <include/user.h>
#include <include/crypto.h>

enum Status {OK, W_USR, W_PSWD};


#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8082

#define KEY_LENGTH 32

class ClientServerConnection : QObject
{
    Q_OBJECT
private:

    //socket with server
    QSslSocket *server_soc;

    Crypto *crypto;
    unsigned char aes_key[KEY_LENGTH];

public:
    ClientServerConnection(QObject *parent);
    virtual ~ClientServerConnection();
    /**
     * @brief This function connects client to server
     * @return true -> connection succesful, false -> fail
     */
    bool connect_to_server();

    /**
     * @brief This function establish aes key with server
     * @return true -> OK, false -> otherwise
     */
    bool establishKey();

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
     * @brief This function logout user from server
     * @param username client's username
     * @return status
     */
    int logout(std::string username);

    /**
     * @brief This function gives you list of online users.
     * @return online users
     */
    std::vector<User> get_online_users();


private:
    void send(QJsonObject &mes);
    QJsonObject respond();


};

#endif	/* CLIENT_SERVER_CONNECTION_H */

