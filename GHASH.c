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
uint16_t H[8];
uint16_t T1[8];
uint16_t Ti[8];
uint16_t T[8] = {0x0000};  // To be check if needed to set entire array to zero or if this is ok
uint16_t null[8] = {0x0000};
uint32_t Y[4];
uint32_t cst = 0xffffffff;

// Varaibles that will probably be created in main or in other places (e.g : IV will need FPGA)
uint16_t Pi[8];    // These two variables corresponds to Pi and Ci, intermediate 128 bits plain and cipher - texts
uint16_t Ci[8];
uint32_t IV[3];
uint64_t lenC;
uint16_t key[8];

void incr(uint16_t *Y){
    Y[0] = Y[0] + 1;
    if (Y[0] == cst) {
        printf("New Pair of Key / IV is required");
    }
}

// Initialization: compute Y0 with the IV. Y will corresponds to counter in algorithm schematics
void initialization(uint32_t *Y, uint32_t *IV){
    Y[0] = 0x00000001;
    uint8_t i;
    for (i=1; i<4; i++) {
        Y[i] = IV[i-1];
    }
    aes_encrypt(key, Y, T1);
    aes_encrypt(key, null, H);
}

// The following function computes all intermediate cipher-texts that will be sent to decryption, as well as T, argument used for next iteration and final tag computation (cfr annotated document).
void ghash(uint16_t *H, uint16_t *Pi, uint16_t *Ci, uint32_t *Y, uint16_t *Ti){
    
    uint8_t i;
    
    incr(Y);
    aes_encrypt(key, Y, Ti);
    xor(Pi, Ti);
    for (i=1; i<8; i++) {
        Ci[i] = Pi[i];
    }
    xor(T, Ci);
    gmult(T, H, T);
}

void tag(uint16_t *T, uint64_t *lenC, uint16_t *T1, uint16_t *H){
    xor(T, lenC);
    gmult(T, H, T);
    xor(T,T1);
}

