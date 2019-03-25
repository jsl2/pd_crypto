//
//  GHASH.c
//  P&D
//
//  Created by Augustin Moutarlier on 12/03/2019.
//  Copyright © 2019 Augustin MOUTARLIER. All rights reserved.
//

#include "GHASH.h"
#include "aes.h"
#include "Galois Multiplication.h"

// Local variables
//uint16_t H[8];
//uint16_t T1[8];
//uint16_t Ti[8];
//uint16_t T[8] = {0x0000};  // To be check if needed to set entire array to zero or if this is ok
uint16_t null[8] = {0x0000};
//uint32_t Y[4];
uint32_t cst = 0xffffffff;

// Varaibles that will probably be created in main or in other places (e.g : IV will need FPGA)
//uint16_t Pi[8];    // These two variables corresponds to Pi and Ci, intermediate 128 bits plain and cipher - texts
//uint16_t Ci[8];
//uint32_t IV[3];
//uint64_t lenC;
//uint8_t key[16];

void incr(unsigned char *Y){
    Y[15] = Y[15] + 1;
    if (Y[15] == cst) {
        printf("New Pair of Key / IV is required");
    }
}

// Initialization: compute Y0 with the IV. Y will corresponds to counter in algorithm schematics
void initialization(unsigned char *Y, unsigned char *IV, aes_key *key, uint16_t *H, uint16_t *T1){
    Y[15] = 0x01;
    uint8_t i;
    for (i=0; i<12; i++) {
        Y[i] = IV[i-1];
    }
    aes_encrypt(key, Y, (unsigned char*) T1);
    aes_encrypt(key, (unsigned char*)null, (unsigned char*) H);
}

// The following function computes all intermediate cipher-texts that will be sent to decryption, as well as T, argument used for next iteration and final tag computation (cfr annotated document).
void ghash(uint16_t *H, unsigned char *Pi, unsigned char *Ci, unsigned char *Y, unsigned char *Ti, aes_key *key, uint16_t *T){

    uint16_t Z[8] = {0x0000};
    uint8_t i;
    incr(Y);
    aes_encrypt(key, Y, Ti);
    xor(Pi, Ti);
    for (i=0; i<16; i++) {
        Ci[i] = Pi[i];
    }
    xor((unsigned char*) T, Ci);
    gmult(T, H, Z);
    for (i=0; i<8; i++) {
        T[i] = Z[i];
    }
}

void tag(uint16_t *T, unsigned char *lenC, uint16_t *T1, uint16_t *H){
    
    uint16_t Z[8] = {0x00};
    uint8_t i;
    xor((unsigned char*) T, lenC);
    gmult(T, H, (uint16_t*)Z);
    for (i=0; i<8; i++) {
        T[i] = Z[i];
    }
    xor((unsigned char*)T,(unsigned char*)T1);
}

