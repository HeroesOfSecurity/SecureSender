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
    QDataStream input(socket);
    QJsonObject json;
    QString str;
    input >> str;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(str.toUtf8());
    if (jsonDoc.isNull()) {
        return 1;
    }
    QJsonObject jsonObject = jsonDoc.object();
    if(!jsonObject["arguments"].isArray())
    {
        return 1;
    }

    function_type = json["function"].toString();
    arguments = json["arguments"].toArray();
    return 0;
}

void SocketThread::readData()
{
    qDebug() << "Reading data";

    //parse client request
    QString function;
    QJsonArray arguments;
    if(!readJsonObject(function, arguments))
    {
        return;
    }

    QJsonObject response;
    if(function.compare("sign_in") == 0)
    {
        username = arguments[0].toString().toUtf8().constData();
        std::string password = arguments[1].toString().toUtf8().constData();
        int res = authenticate(username, password);
        if(res != SUCCESS)
        {
            return;
        }
        dbHelper->sign_in_client(QString::fromStdString(username));
        response["result"] = QJsonValue(res);
        //authenticated = true;
    } else if(/*authenticated && */function.compare("online_users") == 0)
    {
        QList<QString> online_list = online_users();
        response["result"] = QJsonValue(QJsonArray::fromStringList(online_list));
    }

    //send response back to client
    send(*socket, response);
}

void SocketThread::quit()
{
//    if(authenticated)
//    {
        dbHelper->logout_client(QString::fromStdString(username));
//    }
    exit(0);
}

void SocketThread::send(QTcpSocket &soc, QJsonObject &mes)
{
    QByteArray arr;
    QDataStream output(&arr, QIODevice::WriteOnly);
    output << mes;
    soc.write(arr);
}

int SocketThread::register_new_user(std::string username, std::string password)
{
    if(dbHelper->client_exists(username))
    {
        return 1;
    }

    std::string salt;
    ps.generate_salt(salt);

    //generate hash
    std::string hash;
    ps.perform_pbkdf2(password, salt, hash);

    //insert user to database
    Client new_client = Client(username, hash, salt);
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
    ps.perform_pbkdf2(password, salt, hash);

    //compare generated hash with hash in DB
    std::string saved_hash = client.get_salt();
    if(hash != saved_hash)
    {
        return ERR_AUTHENTICATE;
    }

    return SUCCESS;
}


QList<QString> SocketThread::online_users()
{

    /*
    if(!dbHelper->client_exists(username))
    {
        return 1;
    }

    Client client = dbHelper->get_client(username);
    std::vector<std::string> online_user_contacts;
    std::vector<std::string> all_user_contacts = dbHelper.(username);
    for(int i = 0; i < contacts.size(); i++)
    {
        if(all_online_users(all_user_contacts[i]))
        {
            online_user_contacts.push_back(all_user_contacts[i]);
        }
    }*/
    //std::vector<std::string> output(all_online_users.size());
    //std::copy(all_online_users.begin(), all_online_users.end(), output.begin());

    return dbHelper->get_online_users();
}


//NOT SUPPORTED YET
/*
void SocketThread::add_friend(string name){
    return;
}
*/
