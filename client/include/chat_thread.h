#ifndef CHAT_THREAD_H
#define CHAT_THREAD_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QProcess>


/**
 * @brief The SocketThread class
 * This class represents handler of single connection to client.
 */
class ChatThread : public QThread
{
    Q_OBJECT

    QTcpSocket *socket;

    std::string username;
    QProcess term;

public:
    explicit ChatThread(qintptr socket_ptr, QObject *parent = 0);
    explicit ChatThread(QHostAddress ip, QObject *parent = 0);
    ~ChatThread();

    void run() override;

private:
    void createTerminal();

signals:

private slots:

    /**
     * @brief readData slot which is runned on readyRead signal from socket
     * This slot reads data from socket and then sends back response
     */
    void readData();
    void sendData();



    /**
     * @brief quit slot which is runned on disconnected signal from socket
     * This slot ends the thread and emits signal finished (this behaviour is inherited from QThread)
     */
    void quit();


};




#endif // CHAT_THREAD_H
