/**
 * @file client_test.cpp
 * @author Kristian Jakubik
 * @licence MIT Licence
 */

#include "client.h"

// Tell CATCH to define its main function here
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "session.h"

using namespace std;

//Client c;
//TEST_CASE("Sign in") {
//    string name("Jozko");
//    string psw("heslo");
//    int status = c.sign_in(name, psw);
//    CHECK(status == 0);
//    status = c.sign_in("Jozk", psw);
//    CHECK(status == 1);
//    status = s.register_new_user(name, "hesl");
//    CHECK(status == 2);
//}


TEST_CASE("Encryption/Decryption of message - GCM") {
    string key("abcdabcdabcdabcdabcdabcdabcdabcd");
    string message("Toto je tajna sprava");
    string add_data("");
    Crypto c;
    
    unsigned char* enc_message;
    unsigned char* dec_message;
    c.encrypt_msg((const unsigned char*)key.c_str(), (const unsigned char*)message.c_str(), (const unsigned char*)add_data.c_str(), enc_message);
    
    int status = c.decrypt_msg((const unsigned char*)key.c_str(), enc_message, (const unsigned char*)add_data.c_str(), dec_message);
    std::cout << "Ahoj" << enc_message << std::endl;
    CHECK(status == 0);
//    string wrong_add_data("wrong data");
//    status = c.decrypt_msg((const unsigned char*)key.c_str(), enc_message, (const unsigned char*)wrong_add_data.c_str(), dec_message);
//    CHECK(status == 1);
//    enc_message[5] = 'w';
//    status = c.decrypt_msg((const unsigned char*)key.c_str(), enc_message, (const unsigned char*)wrong_add_data.c_str(), dec_message);
//    CHECK(status == 1);
//    key[5] = 'w';
//    status = c.decrypt_msg((const unsigned char*)key.c_str(), enc_message, (const unsigned char*)wrong_add_data.c_str(), dec_message);
//    CHECK(status == 1);
}