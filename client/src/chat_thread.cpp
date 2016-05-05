#include <include/chat_thread.h>
#include <iostream>

#include <QDataStream>
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QProcess>

ChatThread::ChatThread(qintptr socket_ptr, QObject *parent) : QThread(parent)
{
    qDebug() << "Constructing new chat thread";
    username = "";
    socket = new QTcpSocket(NULL);
    socket->setSocketDescriptor(socket_ptr);
}

ChatThread::ChatThread(QHostAddress ip, QObject *parent) : QThread(parent)
{
    qDebug() << "Constructing new chat thread";
    username = "";
    socket = new QTcpSocket(NULL);
    socket->connectToHost(ip, 8083);
}

void ChatThread::createTerminal(){
    term.start("xterm");
    term.waitForStarted();
}

void ChatThread::run()
{
    createTerminal();
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()), Qt::DirectConnection);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readData()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(quit()), Qt::DirectConnection);
    connect(&term, SIGNAL(readyRead()), this, SLOT(sendData()), Qt::DirectConnection);
    const QHostAddress &connected = socket->peerAddress();
    qDebug()<< connected.toString();
    exec();
}

ChatThread::~ChatThread()
{
    qDebug() << "Quiting";
    delete socket;
}


void ChatThread::readData()
{
    qDebug() << "Reading data";
    QByteArray mes = socket->readAll();
    term.write(mes);
}

void ChatThread::sendData(){
    QString mes = QString(term.readAll());
    if(mes.compare(QString("/quit")) == 0)
    {
        emit disconnect();
    }
    socket->write(mes.toUtf8());
    socket->waitForBytesWritten();
}

void ChatThread::quit()
{
    term.close();
    exit(0);
}




