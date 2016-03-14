#include "crypto.h"

#include <iostream>
#include <fstream>
#include "string.h"

#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/aes.h"


using namespace std;

int Crypto::readFile(const char* fileName, unsigned char** fileContent)
{
    ifstream file(fileName);
    if (file.is_open())
    {
        file.seekg(0, ios::end);
        int size = file.tellg();
        printf("%d", size);
        *fileContent = new unsigned char [size + 16];
        file.seekg (0, ios::beg);
        file.read (*((char**)fileContent), size);
        file.close();
	return 0;
    } else  {
        cout << "Unable to open input file"; 
    }
    
    return 1;
}



void Crypto::writeToFile(const char* fileName, unsigned char* fileContent)
{
    ofstream file(fileName);
    
    if (file.is_open())
    {
        file << fileContent;
        file.close();
    } else {
        cout << "Unable to open input file"; 
    }
}



bool Crypto::encrypt(unsigned char* key, const unsigned char* inputData, int inputSize, 
        unsigned char* encData, int & encSize, unsigned char * encIv)
{
//    PKCS#7
//    The value of each added byte is the number of bytes that are added,
//    i.e. N bytes, each of value N are added. The number of bytes added will
//    depend on the block boundary to which the message needs to be extended.
    int padLen = BLOCKLEN - (inputSize % BLOCKLEN);
    int tempBuffSize = inputSize + padLen;
    unsigned char * tempBuff = new unsigned char[tempBuffSize];
    memcpy(tempBuff, inputData, inputSize);
    for (int pos = 0; pos < padLen; ++pos)
    {
        tempBuff[inputSize + pos] = padLen;  // padding the data 
    }  
    
    mbedtls_aes_context aes_ctx;
    
    mbedtls_aes_setkey_enc(&aes_ctx, key, KEYLEN * 8);
    mbedtls_aes_crypt_cbc(&aes_ctx, MBEDTLS_AES_ENCRYPT, tempBuffSize, encIv, tempBuff, encData);
    encSize = tempBuffSize;
    
    delete[] tempBuff;
    return true;
}




bool Crypto::decrypt(unsigned char* key, const unsigned char * inputData, int inputSize, 
        unsigned char * decOutput, unsigned char * decIv)
{    
    mbedtls_aes_context aes_ctx;
    
    mbedtls_aes_init( &aes_ctx );
    
    mbedtls_aes_setkey_dec(&aes_ctx, key, KEYLEN * 8);
    mbedtls_aes_crypt_cbc(&aes_ctx, MBEDTLS_AES_DECRYPT, inputSize, decIv, inputData, decOutput);
    decOutput[inputSize - decOutput[inputSize-1]] = '\0';
    
    return true;
}



void Crypto::hashFile(unsigned char *fileContent, unsigned char* sha_result){
    mbedtls_sha512(fileContent, strlen((const char*)fileContent), sha_result, 0);   
//    cout << "Hash -> " << sha_result << endl;
}
