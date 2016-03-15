/* 
 * File:   crypto.h
 * Author: pedro1
 *
 * Created on March 4, 2016, 2:34 PM
 */

#ifndef CRYPTO_H
#define	CRYPTO_H


namespace Crypto {

    //length of key
    const int KEYLEN = 16;
    //CBS mode -> 16 bytes long blocks
    const int BLOCKLEN = 16;
    
    
    int readFile(const char* fileName, unsigned char** fileContent);
    void writeToFile(const char* fileName, unsigned char* fileContent);
    
    bool encrypt(unsigned char* key, const unsigned char* inputData, int inputSize, 
        unsigned char* encData, int & encSize, unsigned char * encIv);
    bool decrypt(unsigned char* key, const unsigned char * inputData, int inputSize, 
        unsigned char * decOutput, unsigned char * decIv);
    
    void hashFile(unsigned char *fileContent, unsigned char* sha_result);

}



#endif	/* CRYPTO_H */

