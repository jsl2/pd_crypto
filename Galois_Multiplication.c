//
//  Galois Multiplication.c
//  P&D
//
//  Created by Augustin Moutarlier on 10/03/2019.
//  Copyright © 2019 Augustin MOUTARLIER. All rights reserved.
//

#include "Galois_Multiplication.h"

// Method 1 corresponding to Algorithm 1 from paper, where W corresponds to Y in algorithm

//uint16_t X[8];
//uint16_t W[8];
//uint16_t Z[8];
unsigned char R[16] = "\xe1\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
uint32_t cst = 0xffffffff;

// Increment function for Y
void incr(unsigned char *Y){
    
    if (Y[12] == 0xff) {
        printf("New Pair of Key / IV is required");
    }
    else if ((Y[15] == 0xff) & (Y[14] != 0xff)) {
        Y[14] = Y[14] + 1;
        if (Y[14]) {
            Y[15] = 0x00;
        }
    }
    else if ((Y[14] == 0xff) & (Y[15] == 0xff) & (Y[13] != 0xff)){
        Y[13] = Y[13] + 1;
        if (Y[13]) {
            Y[14] = 0x00;
            Y[15] = 0x00;
        }
    }
    else if ((Y[13] == 0xff) & (Y[14] == 0xff) & (Y[15] == 0xff) & (Y[12] != 0xff)){
        Y[12] = Y[12] + 1;
        if (Y[12]) {
            Y[13] = 0x00;
            Y[14] = 0x00;
            Y[15] = 0x00;
        }
    }
    else{
        Y[15] = Y[15] + 1;
    }
}

void incr_lenC(unsigned short *lenC){
    
    if ((lenC[7] == 0xff80) & (lenC[6] != 0xffff)) {
        lenC[6] = lenC[6] + 1;
        if (lenC[6]) {
            lenC[7] = 0x0000;
        }
    }
    else if ((lenC[6] == 0xffff) & (lenC[7] == 0xff80) & (lenC[5] != 0xffff)){
        lenC[5] = lenC[5] + 1;
        if (lenC[5]) {
            lenC[6] = 0x0000;
            lenC[7] = 0x0000;
        }
    }
    else if ((lenC[5] == 0xffff) & (lenC[6] == 0xffff) & (lenC[7] == 0xff80) & (lenC[4] != 0xffff)){
        lenC[4] = lenC[4] + 1;
        if (lenC[4]) {
            lenC[5] = 0x0000;
            lenC[6] = 0x0000;
            lenC[7] = 0x0000;
        }
    }
    else{
        lenC[7] = lenC[7] + 128;
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


