#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/hmac.h>
#include <openssl/err.h>
#include <string.h>
#include <stdlib.h>
#include "bhairava_engine.h"

void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}


int derive_key(const char *password, unsigned char *salt, unsigned char *key)
{
    if (!PKCS5_PBKDF2_HMAC(password,
                           strlen(password),
                           salt,
                           16,
                           100000,
                           EVP_sha256(),
                           32,
                           key))
    {
        return 0;
    }
    return 1;
}

int generate_hmac(const unsigned char *data, int data_len,
                  const unsigned char *hmac_key,
                  unsigned char *out_hmac)
{
    unsigned int len;

    HMAC(EVP_sha256(),
         hmac_key, 16,
         data, data_len,
         out_hmac, &len);

    return len;
}


int bhairav_encrypt(const unsigned char *plaintext, int plaintext_len,
                    const unsigned char *key,
                    const unsigned char *iv,
                    unsigned char *ciphertext)
{
    EVP_CIPHER_CTX *ctx;
    int len, ciphertext_len;

    ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handleErrors();

    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();

    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();

    ciphertext_len = len;

    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        handleErrors();

    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}


int bhairav_decrypt(const unsigned char *ciphertext, int ciphertext_len,
                    const unsigned char *key,
                    const unsigned char *iv,
                    unsigned char *plaintext)
{
    EVP_CIPHER_CTX *ctx;
    int len, plaintext_len;

    ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handleErrors();

    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        handleErrors();

    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        handleErrors();

    plaintext_len = len;

    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
        handleErrors();

    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

int bhairav_encrypt_data_secure(const unsigned char *input, int input_len,
                                const char *password,
                                unsigned char *output)
{
    unsigned char salt[16];
    unsigned char iv[16];
    unsigned char key[32];

    unsigned char aes_key[16];
    unsigned char hmac_key[16];

    unsigned char hmac[32];

    int enc_len;
    int total_len;

   
    if (1 != RAND_bytes(salt, 16)) handleErrors();
    if (1 != RAND_bytes(iv, 16)) handleErrors();

   
    if (!derive_key(password, salt, key))
        handleErrors();

   
    memcpy(aes_key, key, 16);
    memcpy(hmac_key, key + 16, 16);

   
    memcpy(output, "BHV1", 4);
    memcpy(output + 4, salt, 16);
    memcpy(output + 20, iv, 16);

   
    enc_len = bhairav_encrypt(input, input_len,
                              aes_key, iv,
                              output + 36);

    total_len = enc_len + 36;

   
    generate_hmac(output, total_len, hmac_key, hmac);

    memcpy(output + total_len, hmac, 32);

    return total_len + 32;
}

int bhairav_decrypt_data_secure(const unsigned char *input, int input_len,
                                const char *password,
                                unsigned char *output)
{
    unsigned char salt[16];
    unsigned char iv[16];
    unsigned char key[32];

    unsigned char aes_key[16];
    unsigned char hmac_key[16];

    unsigned char computed_hmac[32];
    unsigned char received_hmac[32];

    int data_len;
    int plain_len;

   
    if (memcmp(input, "BHV1", 4) != 0)
        return -1;

   
    memcpy(salt, input + 4, 16);
    memcpy(iv, input + 20, 16);

   
    data_len = input_len - 32;
    memcpy(received_hmac, input + data_len, 32);

    
    if (!derive_key(password, salt, key))
        return -1;

    memcpy(aes_key, key, 16);
    memcpy(hmac_key, key + 16, 16);

    
    generate_hmac(input, data_len, hmac_key, computed_hmac);

    if (CRYPTO_memcmp(received_hmac, computed_hmac, 32) != 0)
        return -2;  

    
    plain_len = bhairav_decrypt(input + 36,
                                data_len - 36,
                                aes_key,
                                iv,
                                output);

    return plain_len;
}
