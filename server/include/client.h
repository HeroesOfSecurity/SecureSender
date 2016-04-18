/*
 * File:   client.h
 * Author: peter
 *
 * Created on March 20, 2016, 4:57 PM
 */

#ifndef CLIENT_H
#define	CLIENT_H
#include <cstdlib>
#include <stdio.h>
#include <string>
#include <vector>

class Client{

    std::string username;
    std::string hash;
    std::string salt;

    //friends not supported yet
    //std::vector<std::string> friends;

public:
    
    Client(std::string username, std::string hash, std::string salt);

    //NOT SUPPORTED YET
    //void add_friend(std::string name);

    std::string get_username();
    std::string get_hash();
    std::string get_salt();
    
};

#endif	/* CLIENT_H */

