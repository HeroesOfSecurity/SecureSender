
#include "dbhelper.h"
#include <cstddef>

DBHelper::DBHelper(std::string file_name) {
    int rc;

    rc = sqlite3_open(file_name.c_str(), &db);

    if (rc) {
        printf("Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    } else {
        printf("Opened database successfully\n");
    }

    string createQuery = "CREATE TABLE IF NOT EXISTS clients (username varchar(20) PRIMARY KEY, hash varchar(32) NOT NULL, salt varchar(32) NOT NULL);";
    sqlite3_stmt *createStmt;
    cout << "Creating Table Statement" << endl;
    sqlite3_prepare_v2(db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);
    cout << "Stepping Table Statement" << endl;
    if (sqlite3_step(createStmt) != SQLITE_DONE) cout << "Didn't Create Table!" << endl;
    else cout << "Table created" << endl;

}

DBHelper::~DBHelper() {
    sqlite3_close(db);
}

Client DBHelper::get_client(std::string username) {
    
    string sql = std::string("SELECT * from clients where username='") + username + "';";
    
    vector<vector<string> > result_set = query(sql.c_str());
    
    for(vector<vector<string> >::iterator it = result_set.begin(); it != result_set.end(); ++it) {
        /*for(vector<string>::iterator iter = it->begin(); iter != it->end(); ++iter) {
            cout << *iter << endl;
        }*/
        
        string salt = it->back();
        it->pop_back();
        
        string hash = it->back();
        it->pop_back();
        
        string username = it->back();
        it->pop_back();
        
        return Client(username,hash,salt);
    }
    
}


bool DBHelper::client_exists(std::string username) {
    
    string sql = std::string("SELECT * from clients where username='") + username + "';";
    
    vector<vector<string> > result_set = query(sql.c_str());
    
    for(vector<vector<string> >::iterator it = result_set.begin(); it != result_set.end(); ++it) {
        /*for(vector<string>::iterator iter = it->begin(); iter != it->end(); ++iter) {
            cout << *iter << endl;
        }*/
        
        return true;
    }
    return false;
}

vector<vector<string> > DBHelper::query(const char* query) {

    sqlite3_stmt *statement;
    vector<vector<string> > results;

    if (sqlite3_prepare_v2(db, query, -1, &statement, 0) == SQLITE_OK) {
        int cols = sqlite3_column_count(statement);
        int result = 0;
        while (true) {
            result = sqlite3_step(statement);

            if (result == SQLITE_ROW) {
                vector<string> values;
                for (int col = 0; col < cols; col++) {
                    values.push_back((char*) sqlite3_column_text(statement, col));
                }
                results.push_back(values);
            } else {
                break;
            }
        }
        sqlite3_finalize(statement);
    }

    string error = sqlite3_errmsg(db);
    if (error != "not an error") cout << query << " " << error << endl;

    return results;
}

int DBHelper::create_client(Client client){
    string sql = "INSERT INTO clients (username, hash, salt) VALUES('" + client.get_username() + "','" + client.get_hash() + "','" + client.get_salt() + "');";

    sqlite3_stmt *insertStmt;
    cout << "Creating Insert Statement" << endl;
    sqlite3_prepare(db, sql.c_str(), sql.size(), &insertStmt, NULL);
    cout << "Stepping Insert Statement" << endl;
    if (sqlite3_step(insertStmt) != SQLITE_DONE) {
        cout << "Didn't create client!" << endl; 
        return 1;
    }
    else {
        cout << "Client created!!!";
        return 0;
    }
}
