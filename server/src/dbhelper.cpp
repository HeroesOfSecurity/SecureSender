#include "dbhelper.h"
#include <cstddef>
#include <exception>

DBHelper::DBHelper(std::string file_name) {
    int rc;

    rc = sqlite3_open(file_name.c_str(), &db);

    if (rc) {
        printf("Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    } else {
        printf("Opened database successfully\n");
    }

    string createQuery = "CREATE TABLE IF NOT EXISTS clients (username varchar(20) PRIMARY KEY, hash varchar(64) NOT NULL, salt varchar(64) NOT NULL);";
    sqlite3_stmt *createStmt;
    cout << "Creating Table Statement" << endl;
    sqlite3_prepare_v2(db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);
    cout << "Stepping Table Statement" << endl;
    if (sqlite3_step(createStmt) != SQLITE_DONE) cout << "Didn't Create Table!" << endl;
    else cout << "Table created" << endl;
    sqlite3_finalize(createStmt);
}

DBHelper::~DBHelper() {
    sqlite3_close(db);
}

Client DBHelper::get_client(std::string username) {
    
    string createQuery = std::string("SELECT * from clients where username= ?;");
    sqlite3_stmt *createStmt;
    sqlite3_prepare_v2(db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);
    sqlite3_bind_text(createStmt, 1, username.c_str(), username.length(), nullptr);
    if (sqlite3_step(createStmt) != SQLITE_ROW) {
            sqlite3_finalize(createStmt);
            throw myDBException();
    }
    Client client((const char*)sqlite3_column_text(createStmt, 0), (const char*)sqlite3_column_text(createStmt, 1), (const char*)sqlite3_column_text(createStmt, 2));
    sqlite3_finalize(createStmt);
    return client;
}

bool DBHelper::client_exists(string username){
    try{
        get_client(username);
    }catch(myDBException &ex){
        return false;
    }
    return true;
}

int DBHelper::create_client(Client client){
    string createQuery = string("INSERT INTO clients (username, hash, salt) VALUES(?,?,?);");
    sqlite3_stmt *insertStmt;
    cout << "Creating Insert Statement" << endl;
    sqlite3_prepare_v2(db, createQuery.c_str(), createQuery.size(), &insertStmt, NULL);
    sqlite3_bind_text(insertStmt, 1, client.get_username().c_str(), client.get_username().length(), nullptr);
    sqlite3_bind_text(insertStmt, 2, client.get_hash().c_str(), client.get_hash().length(), nullptr);
    sqlite3_bind_text(insertStmt, 3, client.get_salt().c_str(), client.get_salt().length(), nullptr);
    cout << "Stepping Insert Statement" << endl;
    if (sqlite3_step(insertStmt) != SQLITE_DONE) {
        cout << "Didn't create client!" << endl;
        sqlite3_finalize(insertStmt);
        return 1;
    }
    else {
        cout << "Client created!!!" << endl;
        sqlite3_finalize(insertStmt);
        return 0;
    }
}

bool DBHelper::sign_in_client(QString username, QHostAddress ip)
{
    online_user user;
    user.name = username;
    user.ip = ip;
    user.token = QString("token");
    online_users.push_back(user);
    return true;
}


bool DBHelper::logout_client(QString username)
{
    cout << "Username" << username.toStdString() << endl;
    for(auto user = online_users.begin(); user != online_users.end(); ++user){
       if(user->name.compare(username) == 0)
            online_users.erase(user);
   }
    return true;
}

QList<online_user> DBHelper::get_online_users()
{
    return online_users;
}
