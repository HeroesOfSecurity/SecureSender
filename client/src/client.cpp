
#include "client.h"
#include "server.h"
#include "client_server_connection.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <session.h>

using namespace std;

Client::Client(){
    token = nullptr;
    session = Session();
}

Client::~Client(){
    delete[] token;
}

int Client::sign_in(string &username, string &password){
    //Pouzit JSON
    string mes = "sing_in;" + username + ";" + password + ";";
    Crypto c;
    unsigned char* en_mes;
    c.encrypt(session.get_key(), mes, "", en_mes);
    //send
    //wait for res
    unsigned char* des_mes;
    c.decrypt(session.get_key(), des_mes, "", mes);
    //check for respond
    return OK;
}

