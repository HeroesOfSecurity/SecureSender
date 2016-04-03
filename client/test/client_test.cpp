/**
 * @file client_test.cpp
 * @author Kristian Jakubik
 * @licence MIT Licence
 */

#include "client.h"

// Tell CATCH to define its main function here
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

using namespace std;

Client c;
TEST_CASE("Sign in") {
    string name("Jozko");
    string psw("heslo");
    int status = c.sign_in(name, psw);
    CHECK(status == 0);
    status = c.sign_in("Jozk", psw);
    CHECK(status == 1);
    status = s.register_new_user(name, "hesl");
    CHECK(status == 2);
}
