#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bhairava_engine.h"


unsigned char *read_file(const char *filename, int *length)
{
    FILE *f = fopen(filename, "rb");
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    *length = ftell(f);
    rewind(f);

    unsigned char *buffer = malloc(*length);
    if (!buffer) return NULL;

    fread(buffer, 1, *length, f);
    fclose(f);

    return buffer;
}


void usage()
{
    printf("Usage:\n");
    printf("./bhairav encrypt <input> <output> <password>\n");
    printf("./bhairav decrypt <input> <output> <password>\n");
}


int main(int argc, char *argv[])
{
    if (argc != 5) {
        usage();
        return 1;
    }

    char *mode = argv[1];
    char *input_file = argv[2];
    char *output_file = argv[3];
    const char *password = argv[4];

    int input_len = 0;
    unsigned char *input_data = read_file(input_file, &input_len);

    if (!input_data) {
        printf("Error reading input file\n");
        return 1;
    }

   
    unsigned char *output_data = malloc(input_len + 128);
    if (!output_data) {
        printf("Memory allocation failed\n");
        free(input_data);
        return 1;
    }

    int output_len = 0;

   
    if (strcmp(mode, "encrypt") == 0) {

        output_len = bhairav_encrypt_data_secure(
            input_data,
            input_len,
            password,
            output_data
        );

        printf("Encryption successful\n");
    }

   
    else if (strcmp(mode, "decrypt") == 0) {

        output_len = bhairav_decrypt_data_secure(
            input_data,
            input_len,
            password,
            output_data
        );

        if (output_len < 0) {
            printf("Decryption failed (wrong password or tampered data)\n");
            free(input_data);
            free(output_data);
            return 1;
        }

        printf("Decryption successful\n");
    }

    else {
        usage();
        free(input_data);
        free(output_data);
        return 1;
    }

   
    FILE *f = fopen(output_file, "wb");
    if (!f) {
        printf("Failed to open output file\n");
        free(input_data);
        free(output_data);
        return 1;
    }

    fwrite(output_data, 1, output_len, f);
    fclose(f);

    free(input_data);
    free(output_data);

    return 0;
}
