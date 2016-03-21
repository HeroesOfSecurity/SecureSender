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

using namespace std;
class Client{
    
    string username;
    string hash;
    string salt;
    
public:
    
    Client(string username, string hash, string salt);
    
    string get_username();
    string get_hash();
    string get_salt();
    
};

#endif	/* CLIENT_H */

