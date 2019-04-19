//
//  Encryption_functions.c
//  P&D
//
//  Created by Augustin Moutarlier on 12/03/2019.
//  Copyright © 2019 Augustin MOUTARLIER. All rights reserved.
//

#include "Encryption_functions.h"
#include "Galois_Multiplication.h"

// The following function computes all intermediate cipher-texts that will be sent to decryption, as well as T, argument used for next iteration and final tag computation (cfr annotated document).
void ghash_e(uint16_t *H, unsigned char *Pi, unsigned char *Ci, unsigned char *Y, aes_key *key, uint16_t *T){

    unsigned char Ti[16]= {0x00};
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

void tag(uint16_t *T, unsigned char *lenC, uint16_t *T0, uint16_t *H){
    uint16_t Z[8] = {0x00};
    uint8_t i;
    xor((unsigned char*) T, lenC);
    gmult(T, H, (uint16_t*)Z);
    for (i=0; i<8; i++) {
        T[i] = Z[i];
    }
    xor((unsigned char*)T,(unsigned char*)T0);
}

