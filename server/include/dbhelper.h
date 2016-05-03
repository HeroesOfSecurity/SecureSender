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
#include <QList>
#include <QString>
#include <string>
#include <QString>
#include <QNetworkAddressEntry>
#include <stdexcept>

typedef struct Online_User{
QString name;
QHostAddress ip;
QString token;
}online_user;

class myDBException : public std::exception{
    virtual const char* what() const throw()
    {
      return "User is not in db";
    }
};

using namespace std;

class DBHelper {
    
    sqlite3 *db;
    QList<online_user> online_users;

public:
    
    DBHelper(std::string file_name);
    ~DBHelper();
    
    int create_client(Client client);
    Client get_client(std::string username);
    bool client_exists(std::string username);
    vector<vector<string> > query(const char* query);

    bool sign_in_client(QString username, QHostAddress ip);
    bool logout_client(QString username);
    QList<online_user> get_online_users();
};

#endif	/* DBHELPER_H */

