
#include "client.h"
#include "server.h"
#include "client_server_connection.h"

using namespace std;

Client::Client(){
    token = nullptr;
}

int Client::sign_in(string username, string password){
    Server s;
    if(int value = s.authenticate(username, password))
        return value;
    ClientServerConnection conn;
    token = "Token";
    token = conn.generateToken();
    return OK;
}

