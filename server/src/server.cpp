#include "server.h"
#include "socket_thread.h"
#include <iostream>


using namespace std;
Server::Server(){
    dbHelper = new DBHelper(string("database.db"));
}

Server::Server(qint16 port, QObject *parent) : QTcpServer(parent), port(port)
{
    dbHelper = new DBHelper(string("database.db"));
    Client c("ahoj", "hash", "salt");
    dbHelper->create_client(c);
    bool a = dbHelper->client_exists("ahoj");
    if(!a)
        cout << "Jmeno" << endl;
    else
        cout << "NIE" << endl;
    qDebug() << "Server is created";
}


Server::~Server() {
    qDebug() << "Closing server";
}

void Server::start()
{
    if (!this->listen(QHostAddress::Any, port)) {
        qDebug() << "Server start failed";
        qDebug() << this->errorString();
        emit finished();
        exit(0);
    }
}

void Server::incomingConnection(qintptr handle)
{
    SocketThread *thread = new SocketThread(handle, dbHelper, parent());
    thread->start();
}



