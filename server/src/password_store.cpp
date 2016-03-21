#include "password_store.h"



PasswordStore::PasswordStore() {
    iniFile.SetUnicode();
    iniFile.LoadFile("database.ini");
}


PasswordStore::store_password_hash(std::string hash)    {
    // adding a new section
    SI_Error rc = iniFile.SetValue("passwords", NULL, NULL);
    if (rc < 0) return false;
    printf("section: %s\n", rc == SI_INSERTED ? 
        "inserted" : "updated");

    // adding a new key ("new-section" will be added 
    // automatically if it doesn't already exist)
    rc = iniFile.SetValue("passwords", "pedro1", hash);
    if (rc < 0) return false;
    printf("key: %s\n", rc == SI_INSERTED ? 
        "inserted" : "updated");

    // changing the value of a key
//    rc = iniFile.SetValue("passwords", "pedro1", "zmenene heslo");
//    if (rc < 0) return false;
//    printf("key: %s\n", rc == SI_INSERTED ? 
//        "inserted" : "updated");
}



