#include <stdio.h>
#include <wolfssl/options.h>
#include <wolfssl/wolfcrypt/aes.h>
#include <wolfssl/wolfcrypt/chacha20_poly1305.h>
#include <wolfssl/wolfcrypt/rsa.h>
#include <wolfssl/wolfcrypt/ecc.h>
#include <time.h>

#define BUFFER_SIZE 1024
#define NUM_ITERATIONS 1000

void aes_benchmark(int key_size, int block_size) {
    byte key[AES_MAX_KEY_SIZE];
    byte iv[AES_BLOCK_SIZE];
    byte input[BUFFER_SIZE];
    byte output[BUFFER_SIZE];
    Aes aes;
    int i, j;
    double total_time = 0;

    for (i = 0; i < key_size; i++) {
        key[i] = (byte)i; // Key initialization
    }

    for (i = 0; i < AES_BLOCK_SIZE; i++) {
        iv[i] = (byte)i; // IV initialization
    }

    for (i = 0; i < BUFFER_SIZE; i++) {
        input[i] = (byte)i; // Input initialization
    }

    for (j = 0; j < NUM_ITERATIONS; j++) {
        clock_t start = clock();

        aes_set_key(&aes, key, key_size, iv, AES_ENCRYPTION);
        aes_encrypt(&aes, output, input, block_size);

        clock_t end = clock();
        total_time += ((double)(end - start) / CLOCKS_PER_SEC);
    }

    printf("AES Encryption Time (Key Size: %d, Block Size: %d): %f ms\n", key_size, block_size, (total_time / NUM_ITERATIONS) * 1000);
}

void chacha20_benchmark(int key_size, int block_size) {
    byte key[CHACHA20_POLY1305_AEAD_KEYSIZE];
    byte iv[CHACHA20_POLY1305_AEAD_IV_SIZE];
    byte input[BUFFER_SIZE];
    byte output[BUFFER_SIZE];
    byte outCypherText[BUFFER_SIZE];
    byte outAuthTag[CHACHA20_POLY1305_AEAD_AUTHTAG_SIZE];
    ChaCha20_Poly1305 ctx;
    int i, j;
    double total_time = 0;

    for (i = 0; i < key_size; i++) {
        key[i] = (byte)i; // Key initialization
    }

    for (i = 0; i < CHACHA20_POLY1305_AEAD_IV_SIZE; i++) {
        iv[i] = (byte)i; // IV initialization
    }

    for (i = 0; i < BUFFER_SIZE; i++) {
        input[i] = (byte)i; // Input initialization
    }

    for (j = 0; j < NUM_ITERATIONS; j++) {
        clock_t start = clock();

        wc_ChaCha20Poly1305_SetKey(&ctx, key, key_size);
        wc_ChaCha20Poly1305_SetNonce(&ctx, iv);
        
        wc_ChaCha20Poly1305_Encrypt(&ctx, output, input, block_size, outCypherText, outAuthTag);

        clock_t end = clock();
        total_time += ((double)(end - start) / CLOCKS_PER_SEC);
    }

    printf("ChaCha20 Encryption Time (Key Size: %d, Block Size: %d): %f ms\n", key_size, block_size, (total_time / NUM_ITERATIONS) * 1000);
}

void rsa_benchmark(int key_size) {
    RsaKey rsa_key;
    byte input[BUFFER_SIZE];
    byte output[BUFFER_SIZE];
    int i, j;
    double total_time = 0;

    wc_InitRsaKey(&rsa_key, 0);

    for (i = 0; i < BUFFER_SIZE; i++) {
        input[i] = (byte)i; // Input initialization
    }

    for (j = 0; j < NUM_ITERATIONS; j++) {
        clock_t start = clock();

        wc_RsaPublicEncrypt(input, output, BUFFER_SIZE, &rsa_key, NULL, key_size);

        clock_t end = clock();
        total_time += ((double)(end - start) / CLOCKS_PER_SEC);
    }

    printf("RSA Encryption Time (Key Size: %d): %f ms\n", key_size, (total_time / NUM_ITERATIONS) * 1000);

    wc_FreeRsaKey(&rsa_key);
}

void ecc_benchmark(int key_size) {
    ecc_key eccKey;
    byte input[BUFFER_SIZE];
    byte output[BUFFER_SIZE];
    int i, j;
    double total_time = 0;

    wc_ecc_init(&eccKey);

    for (i = 0; i < BUFFER_SIZE; i++) {
        input[i] = (byte)i; // Input initialization
    }

    for (j = 0; j < NUM_ITERATIONS; j++) {
        clock_t start = clock();

        wc_ecc_encrypt(&input[0], &output[0], BUFFER_SIZE, &eccKey, key_size);

        clock_t end = clock();
        total_time += ((double)(end - start) / CLOCKS_PER_SEC);
    }

    printf("ECC Encryption Time (Key Size: %d): %f ms\n", key_size, (total_time / NUM_ITERATIONS) * 1000);

    wc_ecc_free(&eccKey);
}

int main() {
    aes_benchmark(16, BUFFER_SIZE);
    aes_benchmark(24, BUFFER_SIZE);
    aes_benchmark(32, BUFFER_SIZE);

    chacha20_benchmark(16, BUFFER_SIZE);
    chacha20_benchmark(24, BUFFER_SIZE);
    chacha20_benchmark(32, BUFFER_SIZE);

    rsa_benchmark(2048);
    rsa_benchmark(3072);
    rsa_benchmark(4096);

    ecc_benchmark(32);
    ecc_benchmark(48);
    ecc_benchmark(64);

    return 0;
}