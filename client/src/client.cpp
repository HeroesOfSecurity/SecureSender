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
    soc.connectToHost(QHostAddress(QString("127.0.0.1")), 9999);
    if (!soc.waitForConnected()) {
        std::cerr << "Could not connect to server";
        exit(0);
    }
}

void Client::send(QTcpSocket &soc, QJsonObject &mes){
    QByteArray arr;
    QDataStream str(&arr, QIODevice::WriteOnly);
    str << mes;
    soc.write(arr);
    soc.waitForBytesWritten();
}

QJsonObject respond(QTcpSocket &soc){
    soc.waitForReadyRead();
    QDataStream u(&soc);
    QJsonObject json();
    u >> json;
    return json;
}

int Client::sign_in(string &username, string &password){

    QTcpSocket soc(app);
    connection(soc);
    QJsonArray mes = {QString("sign_in"), QString(username), QString(password)};
    send(soc, mes);
    QJsonObject json = respond(soc);
    return json[0];
}


