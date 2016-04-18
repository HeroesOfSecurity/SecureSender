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
#include <algorithm>

#include "crypto.h"
#include "dbhelper.h"
#include "client.h"

#include <QObject>
#include <QTcpServer>
#include <QThread>



class Server: public QTcpServer
{
private:

    Q_OBJECT
    qint16 port;

    DBHelper* dbHelper;
    
public:
    
    Server();
    explicit Server(qint16 port, QObject *parent = 0);
    ~Server();


    /**
     * @brief start Method starts infinite loop
     */
    void start();

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



/**
 * @brief The Controler class
 * This class handles stdin and reads commands from stdin
 * The only valid command is quit which ends the application
 */
class Controler : public QThread
{
    Q_OBJECT
public:

    Controler(QObject *parent = 0): QThread(parent)
    { }

    virtual ~Controler()
    { }

    void run() override;
};



#endif	/* SERVER_API_H */

