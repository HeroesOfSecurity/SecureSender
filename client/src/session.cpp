#include <include/crypto.h>
#include <include/session.h>
#include <cstring>


Session::Session()
{
    mbedtls_entropy_context entropy_ctx;
    mbedtls_ctr_drbg_context ctr_drbg_ctx;
    
    mbedtls_entropy_init( &entropy_ctx );
    
    const char *personalization = "v87sNR45EJgU3UOtnxFkmDZgV6V3xX3f";

    mbedtls_ctr_drbg_init( &ctr_drbg_ctx);
    mbedtls_ctr_drbg_seed(&ctr_drbg_ctx, mbedtls_entropy_func, &entropy_ctx, (const unsigned char *)personalization, strlen(personalization));
    
    //mbedtls_ctr_drbg_random(&ctr_drbg_ctx, key, KEY_LENGTH);
}
