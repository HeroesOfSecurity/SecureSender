#include <include/client.h>
#include <QThread>
#include <include/chat_thread.h>

using namespace std;

void Client::incomingConnection(qintptr handle)
{
    ChatThread *thread = new ChatThread(handle, parent());
    thread->start();
}

string Client::getIP(string name){
    for(int i = 0; i < online_users.size(); i++){
        if(online_users[i].getUsername().compare(name) == 0)
            return online_users[i].getIP();
    }
    return "";
}

void Client::startChat(string ip){
    QHostAddress host(QString(ip.c_str()));
    ChatThread *thread = new ChatThread(host, this);
    thread->start();
}
