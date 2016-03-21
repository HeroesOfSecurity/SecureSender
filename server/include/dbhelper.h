/* 
 * File:   dbhelper.h
 * Author: peter
 *
 * Created on March 20, 2016, 4:40 PM
 */

#ifndef DBHELPER_H
#define	DBHELPER_H
#include "client.h"
#include <cstdlib>
#include <stdio.h>
#include <sqlite3.h>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

class DBHelper {
    
    sqlite3 *db;
    
public:
    
    DBHelper(std::string file_name);
    ~DBHelper();
    
    int create_client(std::string username, std::string hash, std::string salt);
    Client get_client(std::string username);
    vector<vector<string> > query(const char* query);
    
    
};

#endif	/* DBHELPER_H */

