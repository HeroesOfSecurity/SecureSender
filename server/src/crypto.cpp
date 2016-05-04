#include "crypto.h"
#include <iostream>

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#define mbedtls_fprintf    fprintf
#define mbedtls_printf     printf
#endif

#if defined(MBEDTLS_CTR_DRBG_C) && defined(MBEDTLS_ENTROPY_C)
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#endif


#include "mbedtls/pkcs5.h"






Crypto::Crypto() {
    iteration_no_pbfkd2 = 1000;


}

int Crypto::perform_pbkdf2(std::string& password, unsigned char* salt, std::string& output_hash)
{
    int ret;
    mbedtls_md_context_t ctx;
    
    mbedtls_md_init(&ctx);
    mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(MBEDTLS_MD_SHA256), 1);
    unsigned char hash[HASH_SIZE];
    ret = mbedtls_pkcs5_pbkdf2_hmac(&ctx, (const unsigned char *)password.c_str(),
                       password.length(), salt,
                       SALT_SIZE,
                       iteration_no_pbfkd2,
                       HASH_SIZE, hash);
    //cout << "Hash" << hash << endl;
    for(int i = 0; i < HASH_SIZE; i++)
        output_hash.push_back(hash[i]);
    std::cout << "HASH: "<< output_hash << std::endl;
    
    if(ret != 0)
    {
        mbedtls_printf("verification failed : %d", ret);
    }
    
    mbedtls_printf("\n");

    mbedtls_md_free(&ctx);

    return ret;
}



int Crypto::generate_salt(unsigned char* salt) {
    int i, k, ret;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_entropy_context entropy;
    unsigned char buf[SALT_SIZE];

    mbedtls_ctr_drbg_init(&ctr_drbg);
    mbedtls_entropy_init(&entropy);

    ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, NULL, 0);
    if (ret != 0) {
        mbedtls_printf("failed in mbedtls_ctr_drbg_seed: %d\n", ret);
        goto cleanup;
    }
    mbedtls_ctr_drbg_set_prediction_resistance(&ctr_drbg, MBEDTLS_CTR_DRBG_PR_OFF);


    ret = mbedtls_ctr_drbg_random(&ctr_drbg, salt, SALT_SIZE);
    if (ret != 0) {
        mbedtls_printf("failed!\n");
        goto cleanup;
    }

    mbedtls_printf("Generating %ldkb of data... %04.1f" \
                "%% done\r", (long) (sizeof (buf) * k / 1024), (100 * (float) (i + 1)) / k);
    fflush(stdout);

    ret = 0;

cleanup:
    mbedtls_printf("\n");

    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);

    return ret;

}



void Crypto::hash(unsigned char *fileContent, unsigned char* sha_result)    {
     mbedtls_sha512(fileContent, strlen((const char*)fileContent), sha_result, 0);
}




