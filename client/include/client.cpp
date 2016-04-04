
#include "client.h"
#include "server.h"
#include "client_server_connection.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

Client::Client(){
    token = nullptr;
}

Client::~Client(){
    delete[] token;
}

int Client::sign_in(string &username, string &password){
    Server s;
    if(int value = s.authenticate(username, password))
        return value;
    token = new char[username.length() + 10 * sizeof(char)];
    memcpy(token, "token", 5 * sizeof(char));
    return OK;
}

