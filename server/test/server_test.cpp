/**
 * @file server_test.cpp
 * @author Kristian Jakubik
 * @licence MIT Licence
 */

#include "server.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <iterator>
// Tell CATCH to define its main function here
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

using namespace std;

Server s;
TEST_CASE("Registration") {
    string name("Jozko");
    string psw("heslo");
    CHECK(s.register_new_user(name, psw) == 0);
    CHECK(s.register_new_user(name, "hesl") != 0);
    for(int i = 0; i < 500; i++){
        CHECK(s.register_new_user(name + to_string(i), psw) == 0);
    }
}

TEST_CASE("Authentication"){
    string name("Jozko");
    string psw("heslo");
    CHECK(s.authenticate(name, psw) == 0);
    CHECK(s.authenticate("Marek", psw) != 0);
    CHECK(s.authenticate(name, "heslo1") == 0);

}

/*TEST_CASE("Get Online users") {
    Server sev;
    string name("Jozko");
    string psw("heslo");
    sev.register_new_user("Peto", "heslo");
    sev.register_new_user("Marek", "heslo");
    vector<string> users = online_users(name);
    CHECK(users.size() == 3);
    for(auto user : users){
        //CHECK((user == "Jozko") || (user == "Peto") || (user == "Marek"));
    }
}
*/
