#include <include/client.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <include/session.h>
#include <qt5/QtNetwork/QTcpSocket>
#include <qt5/QtCore/QCoreApplication>
#include <qt5/QtNetwork/QHostAddress>
#include <qt5/QtCore/QJsonArray>
#include <qt5/QtCore/QJsonObject>
#include <qt5/QtCore/QJsonDocument>
#include <qt5/QtCore/QString>

using namespace std;

/*
Client::Client() : server_soc(nullptr){
    token = nullptr;
    session = Session();
}*/


Client::Client(QObject &par) : parent(par), server_soc(nullptr){
    token = nullptr;
}


Client::~Client(){
    delete[] token;
}

bool Client::connect_to_server()
{
    server_soc = new QTcpSocket(&parent);
    server_soc->connectToHost(QHostAddress(QString(SERVER_IP)), SERVER_PORT);
    if (!server_soc->waitForConnected()) {
        qDebug() << "Could not connect to server";
        return false;
    }
    qDebug() << "Connection to server SUCCESFUL";
    return true;
}


void Client::disconnect()
{
    if(is_connected())
    {
        server_soc->disconnectFromHost();
        delete server_soc;
        server_soc = nullptr;
        qDebug() << "Disconnected from server";
    }

}

bool Client::is_connected()
{
    return server_soc->state() == QTcpSocket::ConnectedState;
}


int Client::sign_in(string &username, string &password){

    QJsonObject mes;
    mes.insert("function", QJsonValue(QString::fromStdString("sign_in")));
    mes.insert("arguments", QJsonValue(QJsonArray::fromStringList({QString::fromStdString(username), QString::fromStdString(password)})));
    send(mes);
    QJsonObject json = respond();
    int res = json["result"].toInt();
    if(res)
    {
        qDebug() << "Authentication failed";
    } else
    {
        qDebug() << "You have been authenticated";
    }
    return res;
}


int Client::sign_up(string username, string password){
    QJsonObject mes;
    mes.insert("function", QJsonValue(QString::fromStdString("sign_up")));
    mes.insert("arguments", QJsonValue(QJsonArray::fromStringList({QString::fromStdString(username), QString::fromStdString(password)})));
    send(mes);
    QJsonObject json = respond();
    int res = json["result"].toInt();
    if(res)
    {
        qDebug() << "Can't create account";
    } else
    {

        qDebug() << "Account has been created";
    }
    return res;
}


std::vector<User> Client::get_online_users(){
    QJsonObject mes;
    mes.insert("function", QJsonValue(QString::fromStdString("online_users")));
    mes.insert("arguments", QJsonValue());
    send(mes);
    QJsonObject json = respond();
    vector<User> v;
    if(json["users"].isArray()){
        QJsonArray users = json["users"].toArray();
        foreach (QJsonValue user, users)
        {
            QJsonObject obj = user.toObject();
            User u(obj["username"].toString().toStdString(), obj["ip_address"].toString().toStdString());
            v.push_back(u);
        }
    }
    return v;
}

void Client::send(QJsonObject &mes){
    QByteArray arr;
    QJsonDocument js(mes);
    arr = js.toJson();
    server_soc->write(arr);
    server_soc->waitForBytesWritten();
}

QJsonObject Client::respond(){
    server_soc->waitForReadyRead();
    QByteArray arr = server_soc->readAll();
    QJsonObject json = QJsonDocument::fromJson(arr).object();
    return json;
}

