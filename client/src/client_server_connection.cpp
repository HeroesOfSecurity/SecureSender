#include "include/client_server_connection.h"
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

#include <mbedtls/entropy_poll.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/pk.h>


using namespace std;

ClientServerConnection::ClientServerConnection(QObject *parent) {
    server_soc = new QSslSocket(this);
    crypto = new Crypto();
}


ClientServerConnection::~ClientServerConnection(){
}


bool ClientServerConnection::connect_to_server()
{
    server_soc->connectToHost(QHostAddress(QString(SERVER_IP)), SERVER_PORT);
    if (!server_soc->waitForConnected()) {
        qDebug() << "Could not connect to server";
        return false;
    }
    qDebug() << "Connection to server SUCCESFUL";
    return true;
}


void ClientServerConnection::disconnect()
{
    if(is_connected())
    {
        server_soc->disconnectFromHost();
        //delete server_soc;
        //server_soc = nullptr;
        qDebug() << "Disconnected from server";
    }
}

bool ClientServerConnection::is_connected()
{
    return server_soc->state() == QTcpSocket::ConnectedState;
}


int ClientServerConnection::sign_in(string &username, string &password){

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


int ClientServerConnection::sign_up(string username, string password){
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


std::vector<User> ClientServerConnection::get_online_users(){
    QJsonObject mes;
    mes.insert("function", QJsonValue(QString::fromStdString("online_users")));
    mes.insert("arguments", QJsonValue());
    send(mes);
    qDebug() << "Call Online Users";
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

void ClientServerConnection::send(QJsonObject &mes){
    QByteArray arr;
    QJsonDocument js(mes);
    arr = js.toJson();
    server_soc->write(arr);
    server_soc->waitForBytesWritten();
    server_soc->flush();
}

QJsonObject ClientServerConnection::respond(){
    server_soc->waitForReadyRead();
    QByteArray arr = server_soc->readAll();
    server_soc->flush();
    QJsonObject json = QJsonDocument::fromJson(arr).object();
    return json;
}

int ClientServerConnection::logout(std::string username){
    QJsonObject mes;
    mes.insert("function", QJsonValue(QString::fromStdString("logout")));
    mes.insert("arguments", QJsonValue(QJsonArray::fromStringList({QString::fromStdString(username)})));
    send(mes);
    QJsonObject json = respond();
    int res = json["result"].toInt();
    return res;
}
