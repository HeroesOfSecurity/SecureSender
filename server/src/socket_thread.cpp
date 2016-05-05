#include "socket_thread.h"
#include <iostream>
#include "constants.h"

#include <QDataStream>
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>

SocketThread::SocketThread(qintptr socket_ptr, DBHelper*& dbHelper, QObject *parent) : QThread(parent), sock_ptr(socket_ptr), dbHelper(dbHelper)
{
    qDebug() << "Constructing new thread";
    ps = Crypto();
}

void SocketThread::run()
{
    socket = new QTcpSocket(NULL);
    socket->setSocketDescriptor(sock_ptr);
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()), Qt::DirectConnection);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readData()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(quit()), Qt::DirectConnection);
    const QHostAddress &connected = socket->peerAddress();
    qDebug()<< connected.toString();
    exec();
}

SocketThread::~SocketThread()
{
    qDebug() << "Quiting";
    delete socket;
}


int SocketThread::readJsonObject(QString &function_type, QJsonArray &arguments)
{
    QByteArray arr = socket->readAll();
    QJsonObject json = QJsonDocument::fromJson(arr).object();
    if (json.isEmpty()) {
        return 1;
    }
    function_type = json["function"].toString();
    if(!json["arguments"].isNull())
        arguments = json["arguments"].toArray();
    return 0;
}

void SocketThread::readData()
{
    qDebug() << "Reading data";

    //parse client request
    QString function;
    QJsonArray arguments;
    if(readJsonObject(function, arguments))
    {
        return;
    }

    QJsonObject response;
    if(function.compare("sign_up") == 0){
       username = arguments[0].toString().toUtf8().constData();
       string password = arguments[1].toString().toUtf8().constData();
       int res = register_new_user(username, password);
       response["result"] = QJsonValue(res);
       send(response);
    }

    else if(function.compare("sign_in") == 0)
    {
        username = arguments[0].toString().toUtf8().constData();
        string password = arguments[1].toString().toUtf8().constData();
        int res = authenticate(username, password);
        if(res == SUCCESS){
            dbHelper->sign_in_client(QString::fromStdString(username), socket->peerAddress());
        }
        response["result"] = QJsonValue(res);

    } else if(function.compare("online_users") == 0)
    {
        QList<online_user> online_list = online_users();
        QJsonArray usersArray;
        for(auto it = online_list.begin(); it != online_list.end(); ++it){
            QJsonObject online_user;
            online_user.insert("username", QJsonValue(it->name));
            online_user.insert("ip_address", QJsonValue(it->ip.toString()));

            usersArray.push_back(online_user);
        }
        response["users"] = usersArray;
    }
    //send response back to client
    send(response);
}

void SocketThread::quit()
{
    dbHelper->logout_client(QString::fromStdString(username));
    exit(0);
}

void SocketThread::send(QJsonObject &mes)
{
    QByteArray arr;
    QJsonDocument js(mes);
    arr = js.toJson();
    socket->write(arr);
    socket->waitForBytesWritten();
}


int SocketThread::register_new_user(std::string username, std::string password)
{
    if(dbHelper->client_exists(username))
    {
        return 1;
    }

    unsigned char salt[SALT_SIZE];
    ps.generate_salt(salt);

    //generate hash
    std::string hash;
    ps.perform_pbkdf2(password, salt, hash);
    string s_salt;
    for(int i = 0; i < SALT_SIZE; i++)
        s_salt.push_back(salt[i]);
    Client new_client = Client(username, hash, s_salt);

    dbHelper->create_client(new_client);
    return 0;
}

int SocketThread::authenticate(std::string username, std::string password)
{

    if(!dbHelper->client_exists(username))
    {
        return ERR_AUTHENTICATE;
    }

    Client client = dbHelper->get_client(username);
    std::string salt = client.get_salt();

    //generate hash
    std::string hash;
    ps.perform_pbkdf2(password, (unsigned char*)salt.c_str(), hash);

    //compare generated hash with hash in DB
    std::string saved_hash = client.get_hash();
    if(hash != saved_hash)
    {
        return ERR_AUTHENTICATE;
    }

    return SUCCESS;
}

QList<online_user> SocketThread::online_users()
{
    return dbHelper->get_online_users();
}

