
#include "client.h"

Client::Client(string username, string hash, string salt){
    this->username = username;
    this->hash = hash;
    this->salt = salt;
}


string Client::get_hash(){
    return hash;
}

string Client::get_salt(){
    return salt;
}

string Client::get_username(){
    return username;
}