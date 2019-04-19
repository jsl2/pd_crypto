//
//  Decryption_functions.c
//  P&D
//
//  Created by Augustin Moutarlier on 25/03/2019.
//  Copyright © 2019 Augustin MOUTARLIER. All rights reserved.
//

#include "lib/aes.h"
#include "galois_mult.h"
#include "decryption_functions.h"

void seq_num_check(unsigned char *Y, unsigned char *packet) {
    uint8_t i;
    for (i = 1; i < 5; i++) {
        if (Y[i + 11] != packet[i]) {
            printf("Order have been compromised");
        }
    }
}

void ghash_d(uint16_t *H, unsigned char *Pi, unsigned char *Ci, unsigned char *Y, aes_key *key, uint16_t *T) {
    unsigned char Ti[16] = {0x00};
    uint16_t Z[8] = {0x0000};
    unsigned char ZA[16] = {0x00};
    uint8_t i;
    incr(Y);
    for (i = 0; i < 16; i++) {
        ZA[i] = Y[i];
    }
    aes_encrypt(key, Y, Ti);
    for (i = 0; i < 16; i++) {
        ZA[i] = Y[i];
    }
    xor(Ti, Ci);
    for (i = 0; i < 16; i++) {
        ZA[i] = Ci[i];
    }
    for (i = 0; i < 16; i++) {
        Pi[i] = Ti[i];
    }
    xor((unsigned char *) T, Ci);
    gmult(T, H, Z);
    for (i = 0; i < 8; i++) {
        T[i] = Z[i];
    }
}

void tag_d(uint16_t *T, unsigned char *lenC, uint16_t *T0, uint16_t *H) {

    uint16_t Z[8] = {0x00};
    uint8_t i;
    xor((unsigned char *) T, lenC);
    gmult(T, H, (uint16_t *) Z);
    for (i = 0; i < 8; i++) {
        T[i] = Z[i];
    }
    xor((unsigned char *) T, (unsigned char *) T0);
}


