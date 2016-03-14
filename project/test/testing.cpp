/** +
 * @file main.cpp
 * @author Martin Ukrop
 * @licence MIT Licence
 */

#include "crypto.h"
#include "string.h"

// Tell CATCH to define its main function here
#define CATCH_CONFIG_MAIN
#include "catch.hpp"



TEST_CASE("Encrypt + Decrypt", "Test if encMessage and original message are the same") {
    
    //AES key - 16 bytes
    unsigned char key[16] =  { 0xa5, 0x84, 0x99, 0x8d, 0x0d, 0xbd, 0xb1, 0x54,
        0xbb, 0xc5, 0x4f, 0xed, 0x86, 0x9a, 0x66, 0x11 };
    
    //Initialization Vector
    unsigned char iv[16] = {
        0x6c, 0x70, 0xed, 0x50, 0xfd, 0xed, 0xb9, 0xda,
        0x51, 0xa3, 0x40, 0xbd, 0x92, 0x9d, 0x38, 0x9d };
    
    //iv for encryption and decryption
    unsigned char encryptIv[16];
    unsigned char decryptIv[16];
    
    memcpy(encryptIv, iv, 16);    
    memcpy(decryptIv, iv, 16);
    
    
    unsigned char message[] = "Secret Message";
    unsigned char encMessage[32] = { 0 };
    unsigned char decMessage[32] = { 0 };
    
    int outSize = 0;
    
    SECTION( "encryption", "encMessage divided by 16" )
    {
        Crypto::encrypt(key, message, strlen((const char *)message), encMessage, outSize, encryptIv);
        INFO("Length of encrypted message divided by 16 -> " << (strlen((char*)encMessage) % 16));
        CHECK( (strlen((char*)encMessage) % 16) == 0 );
        
        SECTION( "decryption", "decMessage same as original message" )
        {
            REQUIRE( message != decMessage );
            Crypto::decrypt(key, (const unsigned char*)encMessage, outSize, decMessage, decryptIv);    
            INFO("Decrypted message -> \"" << decMessage << "\"");
            CHECK( strcmp((char *)message, (char *)decMessage) == 0 );         
        }
    }
}

TEST_CASE("Hash", "Test if hashes are the same") {
    
    //AES key - 16 bytes
    unsigned char key[16] =  { 0xa5, 0x84, 0x99, 0x8d, 0x0d, 0xbd, 0xb1, 0x54,
        0xbb, 0xc5, 0x4f, 0xed, 0x86, 0x9a, 0x66, 0x11 };
    
    //Initialization Vector
    unsigned char iv[16] = {
        0x6c, 0x70, 0xed, 0x50, 0xfd, 0xed, 0xb9, 0xda,
        0x51, 0xa3, 0x40, 0xbd, 0x92, 0x9d, 0x38, 0x9d };
    
    //iv for encryption and decryption
    unsigned char encryptIv[16];
    unsigned char decryptIv[16];
    
    memcpy(encryptIv, iv, 16);    
    memcpy(decryptIv, iv, 16);
    
    
    unsigned char message[] = "Secret Message";
    unsigned char encMessage[32] = { 0 };
    unsigned char decMessage[32] = { 0 };
    
    int outSize = 0;
    
    //  SHA2-512  
    unsigned char shaResult1[65] = { 0 };
    unsigned char shaResult2[65] = { 0 };
    
    Crypto::hashFile(message, shaResult1);
    INFO("Hash of original message -> " << shaResult1);
    
    Crypto::encrypt(key, message, strlen((const char *)message), encMessage, outSize, encryptIv);
    Crypto::decrypt(key, (const unsigned char*)encMessage, outSize, decMessage, decryptIv);    
    
    Crypto::hashFile(message, shaResult2);
    INFO("Hash of decrypted message -> " << shaResult2);
    
    CHECK(strcmp((char *)shaResult1, (char *)shaResult2) == 0);
}





TEST_CASE("readFile", "Test reading of file") {

	unsigned char *file1 = 0;

	int result = Crypto::readFile("../fileExists.txt", &file1);
        CHECK( result == 0);

	delete[] file1;
 
	
	result = Crypto::readFile("file_not_exist.txt", &file1);
        CHECK(result == 1);

	delete[] file1;
}
