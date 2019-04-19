//
//  Galois Multiplication.c
//  P&D
//
//  Created by Augustin Moutarlier on 10/03/2019.
//  Copyright © 2019 Augustin MOUTARLIER. All rights reserved.
//

#include "galois_mult.h"

// Method 1 corresponding to Algorithm 1 from paper, where W corresponds to Y in algorithm

//uint16_t X[8];
//uint16_t W[8];
//uint16_t Z[8];
unsigned char R[16] = "\xe1\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
uint32_t cst = 0xffffffff;

// Increment function for Y
void incr(uint8_t *Y) {
    uint8_t i;
    if (Y[12] == 0xFF && Y[13] == 0xFF && Y[14] == 0xFF && Y[15] == 0xFF) {
        printf("New Pair of Key / IV is required");
    }
    Y[15] += 1;
    for (i = 0; i < 3; i++) {
        if (Y[15 - i] == 0)
            Y[14 - i] += 1;
        else
            break;
    }
}

void incr_lenC(uint8_t *lenC) {
    uint8_t i;
    lenC[15] = lenC[15] + (uint8_t) 128;
    for (i = 0; i < 7; i++) {
        if (lenC[15 - i] == 0)
            lenC[14 - i] += 1;
        else
            break;
    }
}

// The following function is right-shifting a 128bit number that is represented as an array of 8 elements of 16 bits.
void right_shift(uint8_t *V){
    uint8_t carry; // Carry due to previous element in array
    uint8_t i;
    uint8_t mask = 0x01;
    uint8_t X[16];
    
    for (i=0; i<16; i++) {
        X[i] = V[i];
    }
    
    V[0] >>=1;
    
    for (i=1; i<16; i++) {
        carry = X[i-1] & mask;
        carry <<= 7;
        V[i] >>= 1;
        V[i] |= carry;
    }
}

// The following function is doing an XOR between a and b and store its result in a
void xor(unsigned char *a, unsigned char *b){
    uint8_t i;
    for (i=0; i<16; i++) {
        a[i] = a[i]^b[i];
    }
}

void gmult(uint16_t *X, uint16_t *Y, uint16_t*Z){
    uint8_t i;
    uint8_t j;
    uint16_t V[8];
    uint16_t W[8];
    uint16_t mask_y1;
    uint16_t mask_y2;
    uint16_t mask_y3;
    uint16_t mask_y4;
    uint16_t mask_v = 0x0100;
    
    for (i=0; i<8; i++) {
        V[i] = X[i];
        Z[i] = 0x0000;
        W[i] = Y[i];
    }
    
    for (i = 0; i<8; i++) {
        mask_y1 = 0x0080;
        mask_y2 = 0x0008;
        mask_y3 = 0x8000;
        mask_y4 = 0x0800;
        for (j=0; j<4; j++) {
            if (W[i] & mask_y1) {
                xor((unsigned char*) Z,(unsigned char*) V);
                mask_y1 >>= 1;
            }
            else{
                mask_y1 >>= 1;
            }
            if (V[7] & mask_v) {
                right_shift((uint8_t*)V);
                xor((unsigned char*) V, R);
            }
            else{
                right_shift((uint8_t*)V);
            }
        }
        for (j=0; j<4; j++) {
            if (W[i] & mask_y2) {
                xor((unsigned char*) Z,(unsigned char*) V);
                mask_y2 >>= 1;
            }
            else{
                mask_y2 >>= 1;
            }
            if (V[7] & mask_v) {
                right_shift((uint8_t*)V);
                xor((unsigned char*) V, R);
            }
            else{
                right_shift((uint8_t*)V);
            }
        }
        for (j=0; j<4; j++) {
            if (W[i] & mask_y3) {
                xor((unsigned char*) Z,(unsigned char*) V);
                mask_y3 >>= 1;
            }
            else{
                mask_y3 >>= 1;
            }
            if (V[7] & mask_v) {
                right_shift((uint8_t*)V);
                xor((unsigned char*) V, R);
            }
            else{
                right_shift((uint8_t*)V);
            }
        }
        for (j=0; j<4; j++) {
            if (W[i] & mask_y4) {
                xor((unsigned char*) Z,(unsigned char*) V);
                mask_y4 >>= 1;
            }
            else{
                mask_y4 >>= 1;
            }
            if (V[7] & mask_v) {
                right_shift((uint8_t*)V);
                xor((unsigned char*) V, R);
            }
            else{
                right_shift((uint8_t*)V);
            }
        }
    }
}


