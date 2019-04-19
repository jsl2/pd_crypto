#include <stdio.h>
#include "lib/aes.h"

#include "gcm.h"
#include "galois_mult.h"

uint8_t tag_check(uint8_t *T, uint8_t *Te) {
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

void seq_num_check(uint8_t *Y, uint8_t *packet) {
    uint8_t i;
    for (i = 1; i < 5; i++) {
        if (Y[i + 11] != packet[i]) {
            printf("Order have been compromised");
        }
    }
}

void gcm_decrypt(uint16_t *H, uint8_t *Pi, uint8_t *Ci, uint8_t *Y, aes_key *key, uint16_t *T) {
    uint8_t Ti[16] = {0x00};
    uint16_t Z[8] = {0x0000};
    uint8_t i;
    increment(Y);

    aes_encrypt(key, Y, Ti);
    xor(Ti, Ci);

    for (i = 0; i < 16; i++) {
        Pi[i] = Ti[i];
    }
    xor((uint8_t *) T, Ci);
    gmult(T, H, Z);
    for (i = 0; i < 8; i++) {
        T[i] = Z[i];
    }
}


/* The following function computes all intermediate cipher-texts that will be sent to decryption, as well as T,
   argument used for next iteration and final tag computation (cfr annotated document). */
void gcm_encrypt(uint16_t *H, uint8_t *Pi, uint8_t *Ci, uint8_t *Y, aes_key *key, uint16_t *T) {
    uint8_t Ti[16] = {0x00};
    uint16_t Z[8] = {0x0000};
    uint8_t i;
    increment(Y);
    aes_encrypt(key, Y, Ti);
    xor(Pi, Ti);
    for (i = 0; i < 16; i++) {
        Ci[i] = Pi[i];
    }
    xor((uint8_t *) T, Ci);
    gmult(T, H, Z);
    for (i = 0; i < 8; i++) {
        T[i] = Z[i];
    }
}

void tag(uint16_t *T, uint8_t *lenC, uint16_t *T0, uint16_t *H) {
    uint16_t Z[8] = {0x00};
    uint8_t i;
    xor((uint8_t *) T, lenC);
    gmult(T, H, (uint16_t *) Z);
    for (i = 0; i < 8; i++) {
        T[i] = Z[i];
    }
    xor((uint8_t *) T, (uint8_t *) T0);
}


void initialization(uint8_t *Y, uint8_t *IV, aes_key *key, uint16_t *H, uint16_t *T0, uint16_t *T) {
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

    aes_encrypt(key, Y, (uint8_t *) T0);
    aes_encrypt(key, (uint8_t *) null, (uint8_t *) H);
}

