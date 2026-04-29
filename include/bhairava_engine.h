#ifndef BHAIRAVA_ENGINE_H
#define BHAIRAVA_ENGINE_H

int derive_key(const char *password, unsigned char *salt, unsigned char *key);

int generate_hmac(const unsigned char *data, int data_len,
                  const unsigned char *hmac_key,
                  unsigned char *out_hmac);

int bhairav_encrypt(const unsigned char *plaintext, int plaintext_len,
                    const unsigned char *key,
                    const unsigned char *iv,
                    unsigned char *ciphertext);

int bhairav_decrypt(const unsigned char *ciphertext, int ciphertext_len,
                    const unsigned char *key,
                    const unsigned char *iv,
                    unsigned char *plaintext);

int bhairav_encrypt_data_secure(const unsigned char *input, int input_len,
                                const char *password,
                                unsigned char *output);

int bhairav_decrypt_data_secure(const unsigned char *input, int input_len,
                                const char *password,
                                unsigned char *output);

#endif
