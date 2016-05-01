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

Client::Client(QCoreApplication &a) : app(a){
    token = nullptr;
    session = Session();
}

Client::~Client(){
    delete[] token;
}

void Client::connection(QTcpSocket &soc){
    soc.connectToHost(QHostAddress(QString("127.0.0.1")), 8082);
    if (!soc.waitForConnected()) {
        std::cerr << "Could not connect to server";
        exit(0);
    }
}

void Client::send(QTcpSocket &soc, QJsonObject &mes){
    QByteArray arr;
    QJsonDocument js(mes);
    arr = js.toJson();
    soc.write(arr);
    soc.waitForBytesWritten();
}

QJsonObject Client::respond(QTcpSocket &soc){
    soc.waitForReadyRead();
    QByteArray arr = soc.readAll();
    QJsonObject json = QJsonDocument::fromJson(arr).object();
    return json;
}

int Client::sign_up(string username, string password){
    QTcpSocket soc(&app);
    QJsonObject mes;
    mes.insert("function", QJsonValue(QString::fromStdString("sign_up")));
    mes.insert("arguments", QJsonValue(QJsonArray::fromStringList({QString::fromStdString(username), QString::fromStdString(password)})));
    connection(soc);
    send(soc, mes);
    QJsonObject json = respond(soc);
    return json["result"].toInt();
}


int Client::sign_in(string &username, string &password){

    QTcpSocket soc(&app);
    QJsonObject mes;
    mes.insert("function", QJsonValue(QString::fromStdString("sign_in")));
    mes.insert("arguments", QJsonValue(QJsonArray::fromStringList({QString::fromStdString(username), QString::fromStdString(password)})));
    connection(soc);
    send(soc, mes);
    QJsonObject json = respond(soc);
    return json["result"].toInt();
}

std::vector<User> Client::get_online_users(){
    QTcpSocket soc(&app);
    QJsonObject mes;
    mes.insert("function", QJsonValue(QString::fromStdString("online_users")));
    mes.insert("arguments", QJsonValue());
    connection(soc);
    send(soc, mes);
    QJsonObject json = respond(soc);
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

