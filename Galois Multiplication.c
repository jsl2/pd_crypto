//
//  Galois Multiplication.c
//  P&D
//
//  Created by Augustin Moutarlier on 10/03/2019.
//  Copyright © 2019 Augustin MOUTARLIER. All rights reserved.
//

#include "Galois Multiplication.h"

// Method 1 corresponding to Algorithm 1 from paper

uint16_t X[8];
uint16_t Y[8];
uint16_t Z[8];
uint16_t R[9] = {0x0008,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00d1};


// The following function is right-shifting a 128bit number that is represented as an array of 8 elements of 16 bits.
void right_shift(uint16_t *V){
    uint8_t carry = 0x00;
    uint8_t i;
    uint16_t mask = 0x8000;
   //for (i=1; i<=V[0]; i++)
    for (i=0; i<8; i++) {
        V[i] = V[i] + carry;
        carry = V[i] & mask;
        V[i]<<=1;
    }
}

// The following function is doing an XOR between a and b and store its result in a
void xor(uint16_t *a, uint16_t *b){
    uint8_t i;
    for (i=0; i<8; i++) {
        a[i] = a[i]^b[i];
    }
    
}

void gmult(uint16_t *X, uint16_t *Y, uint16_t*Z){
    uint8_t i;
    uint8_t j;
    uint16_t z = 0x0000;
    uint16_t V[8];
    uint16_t mask_y = 0x0001;
    uint16_t mask_v = 0x8000;
    
    for (i=0; i<8; i++) {
        V[i] = X[i];
        Z[i] = 0;
    }
    
    for (i = 0; i<8; i++) {
        mask_y = 0x0001;
        for (j=0; j<16; j++) {
            if ((Y[i] & mask_y) != 0) {
                xor(Z,V);
                mask_y <<= 1;
            }
            else{
            }
            if ((V[7] & mask_v) == 0) {
                right_shift(V);
            }
            else{
                right_shift(V);
                V[7] = V[7]^R[7];
                //From algorithm 1, result from XOR of rightshift(V)^R as R has only 8 bits different from 0, and a^b = a if b=0
            }
        }
        Z[i] = z;
    }
}


