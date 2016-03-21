/* 
 * File:   main.cpp
 * Author: pedro1
 *
 * Created on February 26, 2016, 12:50 PM
 */
#include "crypto.h"


#include "string.h"
#include "stdio.h"


using namespace std;


int main(int argc, char** argv) {
    
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

    
    
    unsigned char *fileInput;
    
    int outSize;
    
    Crypto::readFile("../input.txt", &fileInput);
    
//  SHA2-512  
    unsigned char shaResult1[64] = { 0 };
    unsigned char shaResult2[64] = { 0 };
    
    Crypto::hashFile((unsigned char*)fileInput, shaResult1);
    
    unsigned int length = strlen((const char *)fileInput);
    unsigned char encryptedOutput[length];
    unsigned char decryptedOutput[length];
    
    memset( encryptedOutput, 0, length*sizeof(char) );
    memset( decryptedOutput, 0, length*sizeof(char) );
      
    
    printf("Povodny subor: %s\n", fileInput);
    printf("Dlzka povodneho subora: %lu\n", strlen((const char *)fileInput)); 
    Crypto::encrypt(key, fileInput, strlen((const char *)fileInput), encryptedOutput, outSize, encryptIv);
    printf("Zasifrovany subor: %s\n", encryptedOutput); 
    printf("Dlzka zasifrovaneho subora: %lu\n", strlen((const char *)encryptedOutput));
    Crypto::writeToFile("../encrypt_output.txt", encryptedOutput);
//    printf("outSize -> %d, strlen -> %d", outSize, strlen((const char*)encryptedOutput));
    Crypto::decrypt(key, (const unsigned char*)encryptedOutput, outSize, decryptedOutput, decryptIv);
    printf("Desifrovany subor: %s\n", decryptedOutput);
    printf("Dlzka desifrovaneho subora: %lu\n", strlen((const char *)decryptedOutput));
    Crypto::writeToFile("../decrypt_output.txt", decryptedOutput); 
    
    Crypto::hashFile((unsigned char*)decryptedOutput, shaResult2);
    
    printf("Hash1 -> ");
    for (const unsigned char* p = shaResult1; *p; ++p)
    {
        printf("%02x", *p);
        ++p;
    }
    printf("\n");
    printf("Hash2 -> ");
    for (const unsigned char* p = shaResult2; *p; ++p)
    {
        printf("%02x", *p);
    }
    printf("\n");
    if(memcmp(shaResult1, shaResult2, strlen((const char*)fileInput)))
    {
        printf("Hashes are the same.\n");
    } else  {
        printf("Hashes are different.\n");
    }
    
    delete[] fileInput;
    return 0;
}
