#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>

#include "dbhelper.h"
#include <crypto.h>

/**
 * @brief The SocketThread class
 * This class represents handler of single connection to client.
 */
class SocketThread : public QThread
{
    Q_OBJECT

    qintptr sock_ptr;
    QTcpSocket *socket;

    DBHelper* dbHelper;
    Crypto ps;

    std::string username;
    bool authenticated = false;

public:
    explicit SocketThread(qintptr socket_ptr, DBHelper*& dbHelper, QObject *parent = 0);

    ~SocketThread();

    void run() override;

    /**
     * @brief This function tries to register new user to database
     * @param username username of new user
     * @param password password if new user
     * @return status, 0 if registration was succesful
     *                 1 username exists
     *                 2 password is weak
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
     * @return list of online users
     */
    QList<online_user> online_users();

    /**
     * @brief This function add friend to the user
     * @param name of the friend
     */
    void add_friend(std::string name);

private:

    int readJsonObject(QString &function_type, QJsonArray &arguments);

    void send(QJsonObject &mes);

signals:

private slots:

    /**
     * @brief readData slot which is runned on readyRead signal from socket
     * This slot reads data from socket and then sends back response
     */
    void readData();



    /**
     * @brief quit slot which is runned on disconnected signal from socket
     * This slot ends the thread and emits signal finished (this behaviour is inherited from QThread)
     */
    void quit();


};

#endif // SOCKETTHREAD_H
