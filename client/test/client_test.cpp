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

int char2int(unsigned char input)
{
  if(input >= '0' && input <= '9')
    return input - '0';
  if(input >= 'A' && input <= 'F')
    return input - 'A' + 10;
  if(input >= 'a' && input <= 'f')
    return input - 'a' + 10;
  throw std::invalid_argument("Invalid input string");
}

// This function assumes src to be a zero terminated sanitized string with
// an even number of [0-9a-f] characters, and target to be sufficiently large
void hex2bin(const char* src, unsigned char* target)
{
  while(*src && src[1])
  {
    *(target++) = char2int(*src)*16 + char2int(src[1]);
    src += 2;
  }
}


//Test vector from http://www.ieee802.org/1/files/public/docs2011/bn-randall-test-vectors-0511-v1.pdf
// Chapter -> 54-byte Packet Encryption Using GCM-AES-256
//          TEST DATA (in hex):
//              original message -> "08000F101112131415161718191A1B1C1D1E1F202122232425262728292A2B2C2D2E2F30313233340004" (336 bits)
//              additional data -> "E20106D7CD0DF0761E8DCD3D88E54C2A76D457ED" (160 bits)
//              initialization_vector -> "F0761E8DCD3D000176D457ED" (96 bits)
//              ciphertext of message -> "C1623F55730C93533097ADDAD25664966125352B43ADACBD61C5EF3AC90B5BEE929CE4630EA79F6CE519" (336 bits)
//              authentication tag -> "12AF39C2D1FDC2051F8B7B3C9D397EF2" (128 bits)


TEST_CASE("Encryption Using GCM-AES-256") {
    
    Crypto c;
    
    unsigned char key[32];
    unsigned char message[42];
    unsigned char add_data[20];
    unsigned char iv[IV_LENGTH];
    unsigned char expected_enc_msg[42];
    unsigned char expected_auth_tag[TAG_LENGTH];
    
    hex2bin("691D3EE909D7F54167FD1CA0B5D769081F2BDE1AEE655FDBAB80BD5295AE6BE7", key);
    hex2bin("08000F101112131415161718191A1B1C1D1E1F202122232425262728292A2B2C2D2E2F30313233340004", message);
    hex2bin("E20106D7CD0DF0761E8DCD3D88E54C2A76D457ED", add_data);
    hex2bin("F0761E8DCD3D000176D457ED", iv);
    hex2bin("C1623F55730C93533097ADDAD25664966125352B43ADACBD61C5EF3AC90B5BEE929CE4630EA79F6CE519", expected_enc_msg);
    hex2bin("12AF39C2D1FDC2051F8B7B3C9D397EF2", expected_auth_tag);
    
    unsigned char* enc_message;
    
    c.protect_msg(key, iv, message, 42, add_data, 20, &enc_message);
    
    SECTION("TEST -> check ORIG_LENGTH part of encrypted message")
    {
        CHECK(*((int *)enc_message) == 42);
    }
    SECTION("TEST -> check IV part of encrypted message")
    {
        CHECK(memcmp(enc_message + sizeof(int), iv, IV_LENGTH) == 0);
    }
    SECTION("TEST -> check AUTH_TAG part of encrypted message")
    {
        CHECK(memcmp(enc_message + sizeof(int) + IV_LENGTH, expected_auth_tag, TAG_LENGTH) == 0);
    }
    SECTION("TEST -> check BODY part of encrypted message")
    {
        CHECK(memcmp(enc_message + sizeof(int) + IV_LENGTH + TAG_LENGTH, expected_enc_msg, 42) == 0);
    }
    
    delete[] enc_message;
}



TEST_CASE("Decryption Using GCM-AES-256") {
    
    Crypto c;
    int length = 42;
    unsigned char key[32];
    unsigned char enc_msg[42 + TAG_LENGTH + IV_LENGTH + sizeof(int)];
    unsigned char add_data[20];
    unsigned char expected_dec_msg[42];
    
    hex2bin("691D3EE909D7F54167FD1CA0B5D769081F2BDE1AEE655FDBAB80BD5295AE6BE7", key);
    
    hex2bin("2A000000F0761E8DCD3D000176D457ED12AF39C2D1FDC2051F8B7B3C9D397EF2C1623F55730C93533097ADDAD25664966125352B43ADACBD61C5EF3AC90B5BEE929CE4630EA79F6CE519", enc_msg);
    hex2bin("14000000E20106D7CD0DF0761E8DCD3D88E54C2A76D457ED", add_data); //size+add_data
    hex2bin("08000F101112131415161718191A1B1C1D1E1F202122232425262728292A2B2C2D2E2F30313233340004", expected_dec_msg);
    
       
    unsigned char* dec_message;
    
    SECTION("TEST -> decrypt correct encrypted message")
    {
        CHECK(c.unprotect_msg(key, enc_msg, add_data, &dec_message) == SUCCESS);
        CHECK(memcmp(dec_message, expected_dec_msg, 42) == 0);
        delete[] dec_message;
    }
    SECTION("TEST -> ORIG_LENGTH part of encrypted message is modified")
    {
        enc_msg[2] = 'X';
        CHECK(c.unprotect_msg(key, enc_msg, add_data, &dec_message) == ERR_MSG_SIZE);
    }
    SECTION("TEST -> AUTH_TAG part of encrypted message is modified")
    {
        enc_msg[19] = 'b';
        CHECK(c.unprotect_msg(key, enc_msg, add_data, &dec_message) == ERR_AUTHENTICATION_FAILED);        
    }
    SECTION("TEST -> BODY part of encrypted message is modified")
    {
        enc_msg[35] = 'c';
        CHECK(c.unprotect_msg(key, enc_msg, add_data, &dec_message) == ERR_AUTHENTICATION_FAILED);        
    }
    SECTION("TEST -> additional data modified")
    {
        add_data[14] = 'c';
        CHECK(c.unprotect_msg(key, enc_msg, add_data, &dec_message) == ERR_AUTHENTICATION_FAILED);        
    }
    SECTION("TEST -> key modified")
    {
        key[5] = 'z';
        CHECK(c.unprotect_msg(key, enc_msg, add_data, &dec_message) == ERR_AUTHENTICATION_FAILED);        
    }

}