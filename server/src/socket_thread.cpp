#include "socket_thread.h"
#include <iostream>
#include "constants.h"

#include <QDataStream>
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QSslConfiguration>
#include <QFile>
#include <QSslCertificate>
#include <QSslKey>

SocketThread::SocketThread(qintptr socket_ptr, DBHelper*& dbHelper, QObject *parent) : QThread(parent), sock_ptr(socket_ptr), dbHelper(dbHelper)
{
    qDebug() << "Constructing new thread";
    username = "";
    ps = Crypto();
}

void SocketThread::run()
{
    socket = new QSslSocket(nullptr);

    //set ssl configuration
    QSslConfiguration sslConfiguration;
    QFile certFile(QStringLiteral("server.crt"));
    QFile keyFile(QStringLiteral("server.key"));
    QSslCertificate certificate(&certFile, QSsl::Pem);
    QSslKey sslKey(&keyFile, QSsl::Rsa, QSsl::Pem);
    certFile.close();
    keyFile.close();
    sslConfiguration.setPeerVerifyMode(QSslSocket::VerifyNone);
    sslConfiguration.setLocalCertificate(certificate);
    sslConfiguration.setPrivateKey(sslKey);
    sslConfiguration.setProtocol(QSsl::TlsV1SslV3);
    socket->setSslConfiguration(sslConfiguration);

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
    socket->close();
    delete socket;
}


int SocketThread::readJsonObject(QString &function_type, QJsonArray &arguments)
{
    QByteArray arr = socket->readAll();
    socket->flush();
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
       string user = arguments[0].toString().toUtf8().constData();
       string password = arguments[1].toString().toUtf8().constData();
       int res = register_new_user(user, password);
       response["result"] = QJsonValue(res);
    }

    else if(function.compare("sign_in") == 0)
    {
        string user = arguments[0].toString().toUtf8().constData();
        string password = arguments[1].toString().toUtf8().constData();
        int res = authenticate(user, password);
        if(res == SUCCESS){
            username = user;
            dbHelper->sign_in_client(QString::fromStdString(username), socket->peerAddress());
        }
        response["result"] = QJsonValue(res);

    }
    else if(function.compare("online_users") == 0)
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
    else if(function.compare("logout") == 0){
        cout << "LogOUT" << endl;
        QString user = arguments[0].toString();
        std::cout << "KLIENT: " << user.toStdString() << std::endl;
        dbHelper->logout_client(user);
        response["result"] = QJsonValue(0);
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
    socket->flush();
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



