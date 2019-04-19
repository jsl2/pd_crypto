#include <stdio.h>
#include "lib/aes.h"

#include "gcm.h"
#include "galois_mult.h"

uint8_t tag_check(unsigned char *T, unsigned char *Te) {
    uint8_t i;
    for (i = 0; i < 16; i++) {
        if (T[i] != Te[i]) {
#ifdef DEBUG_PRINT
            printf("Tag does not match ! Discard data \n");
#endif
            return 0;
        }
    }
    return 1;
}

void seq_num_check(unsigned char *Y, unsigned char *packet) {
    uint8_t i;
    for (i = 1; i < 5; i++) {
        if (Y[i + 11] != packet[i]) {
            printf("Order have been compromised");
        }
    }
}

void gcm_decrypt(uint16_t *H, unsigned char *Pi, unsigned char *Ci, unsigned char *Y, aes_key *key, uint16_t *T) {
    unsigned char Ti[16] = {0x00};
    uint16_t Z[8] = {0x0000};
    uint8_t i;
    increment(Y);

    aes_encrypt(key, Y, Ti);
    xor(Ti, Ci);

    for (i = 0; i < 16; i++) {
        Pi[i] = Ti[i];
    }
    xor((unsigned char *) T, Ci);
    gmult(T, H, Z);
    for (i = 0; i < 8; i++) {
        T[i] = Z[i];
    }
}


/* The following function computes all intermediate cipher-texts that will be sent to decryption, as well as T,
   argument used for next iteration and final tag computation (cfr annotated document). */
void gcm_encrypt(uint16_t *H, unsigned char *Pi, unsigned char *Ci, unsigned char *Y, aes_key *key, uint16_t *T) {
    unsigned char Ti[16] = {0x00};
    uint16_t Z[8] = {0x0000};
    uint8_t i;
    increment(Y);
    aes_encrypt(key, Y, Ti);
    xor(Pi, Ti);
    for (i = 0; i < 16; i++) {
        Ci[i] = Pi[i];
    }
    xor((unsigned char *) T, Ci);
    gmult(T, H, Z);
    for (i = 0; i < 8; i++) {
        T[i] = Z[i];
    }
}

void tag(uint16_t *T, unsigned char *lenC, uint16_t *T0, uint16_t *H) {
    uint16_t Z[8] = {0x00};
    uint8_t i;
    xor((unsigned char *) T, lenC);
    gmult(T, H, (uint16_t *) Z);
    for (i = 0; i < 8; i++) {
        T[i] = Z[i];
    }
    xor((unsigned char *) T, (unsigned char *) T0);
}


void initialization(unsigned char *Y, unsigned char *IV, aes_key *key, uint16_t *H, uint16_t *T0, uint16_t *T) {
    uint16_t null[8] = {0x0000};
    uint8_t i;
    Y[15] = 0x01;
    for (i = 0; i < 12; i++) {
        Y[i] = IV[i];
    }
    for (i = 12; i < 15; i++) {
        Y[i] = 0x00;
    }
    for (i = 0; i < 8; i++) {
        T[i] = 0x0000;
    }

    aes_encrypt(key, Y, (unsigned char *) T0);
    aes_encrypt(key, (unsigned char *) null, (unsigned char *) H);
}

