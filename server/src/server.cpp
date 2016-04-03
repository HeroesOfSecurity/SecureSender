#include "server.h"


Server::Server(){
    dbHelper = new DBHelper(string("database.db"));
    ps = Crypto();
}


int Server::register_new_user(std::string username, std::string password)
{
    if(dbHelper->client_exists(username))
    {
        return 1;
    }
    
    std::string salt;
    ps.generate_salt(salt);

    //generate hash
    std::string hash;
    ps.perform_pbkdf2(password, salt, hash);
    
    //insert user to database
    Client new_client = Client(username, hash, salt);
    dbHelper->create_client(new_client);
    return 0;
}

int Server::authenticate(std::string username, std::string password)
{
    
    if(!dbHelper->client_exists(username))
    {
        return 1;
    }
    
    Client client = dbHelper->get_client(username);
    std::string salt = client.get_salt();
    
    //generate hash
    std::string hash;
    ps.perform_pbkdf2(password, salt, hash);
    
    //compare generated hash with hash in DB
    std::string saved_hash = client.get_salt();
    if(hash != saved_hash)
    {
        return 2;
    }
    
    all_online_users.insert(username);
    return 0;
}


std::vector<std::string> Server::online_users(std::string username)
{
    
    /*
    if(!dbHelper->client_exists(username))
    {
        return 1;
    }
    
    Client client = dbHelper->get_client(username);
    std::vector<std::string> online_user_contacts;
    std::vector<std::string> all_user_contacts = dbHelper.(username);
    for(int i = 0; i < contacts.size(); i++)
    {
        if(all_online_users(all_user_contacts[i]))
        {
            online_user_contacts.push_back(all_user_contacts[i]);
        }
    }*/
    std::vector<std::string> output(all_online_users.size());
    std::copy(all_online_users.begin(), all_online_users.end(), output.begin());
    
    return output;
}
