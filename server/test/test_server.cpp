/**
 * @file test_server.cpp
 * @author Kristian Jakubik
 * @licence MIT Licence
 */

#include "server.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>
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
    for(int i = 0; i < 2000; i++){
        CHECK(s.register_new_user(name + i, psw) == 0);
    }
}

TEST_CASE("Authentication"){
    string name("Jozko");
    string psw("heslo");
    CHECK(s.authenticate(name, psw) == 0);
    CHECK(s.authenticate("Marek", psw) != 0);
    CHECK(s.authenticate(name, "heslo1") == 0);

}

TEST_CASE("Encryption/Decryption") {
    string name("Jozko");

}
