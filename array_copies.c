//
//  array_copies.c
//  P&D
//
//  Created by Augustin Moutarlier on 25/03/2019.
//  Copyright © 2019 Augustin MOUTARLIER. All rights reserved.
//

#include "array_copies.h"

// Initialisation: compute Y0 with the IV. Y will corresponds to counter in algorithm schematics
uint16_t null[8] = {0x0000};
void initialization(unsigned char *Y, unsigned char *IV, aes_key *key, uint16_t *H, uint16_t *T0, uint16_t *T){
    uint8_t i;
    Y[15] = 0x01;
    for (i=0; i<12; i++) {
        Y[i] = IV[i];
    }
    for (i=12; i<15; i++) {
        Y[i] = 0x00;
    }
    for (i=0; i<8; i++) {
        T[i] = 0x0000;
    }
    
    aes_encrypt(key, Y, (unsigned char*) T0);
    aes_encrypt(key, (unsigned char*)null, (unsigned char*) H);
}

// cbts : copy big array to small array
void cbts(unsigned char *buffer, unsigned char *Pi, uint8_t j){
    uint8_t i;
    for (i=0; i<16; i++) {
        Pi[i] = buffer[i + (j)*16];
    }
}

// cstb : copy small array to big array
void cstb(unsigned char *Ci, unsigned char *C, uint8_t j){
    uint8_t i;
    for (i=0; i<16; i++) {
        C[i+j*16] = Ci[i];
    }
}

// This function will calculate np depending on value given by the speech part, np Iteration
void npIt(uint16_t *numBits, uint8_t *np){
    uint8_t a;
    uint16_t b = numBits[0];
    uint8_t rest;
    a = b/128;
    rest = b%128;
    if (rest) {
        *np = a + 1;
    }
    else{
        *np = a;
    }
}

// Packets formation to have data ready to be sent

// packet_1 is an unsigned char of 17 elements
void data_1(unsigned char *Y, unsigned char *packet_1){
    uint8_t i;
    packet_1[0] = 0x04;
    for (i=1; i<5; i++) {
        packet_1[i] = Y[11 + i];
    }
    for (i=5; i<17; i++) {
        packet_1[i] = Y[i-5];
    }
}

//packet_int is an unsigned char of 21 elements
void data_int(unsigned char *Y, unsigned char *Ci, unsigned char *packet_int){
    uint8_t i;
    packet_int[0] = 0x05;
    for (i=1; i<5; i++) {
        packet_int[i] = Y[11 + i];
    }
    for (i=5; i<21; i++) {
        packet_int[i] = Ci[i-5];
    }
}

//packet_last is an unsigned char of 26 elements
void data_last(unsigned char *Y,unsigned char *T, unsigned char *packet_last){
    uint8_t i;
    packet_last[0] = 0x06;
    for (i=1; i<5; i++) {
        packet_last[i] = Y[11 + i];
    }
    for (i=5; i<21; i++) {
        packet_last[i] = T[i-5];
    }
}

// Packet decryption to form the cipher texts
void packet_1_d(unsigned char *packet_1, unsigned char *IV){
    uint8_t i;
    for (i=0; i<12; i++) {
        IV[i] = packet_1[i+5];
    }
}

void packet_int_d(unsigned char *packet_int, unsigned char *Ci){
    uint8_t i;
    for (i=0; i<16; i++) {
        Ci[i] = packet_int[i+5];
    }
}

void packet_last_d(unsigned char *packet_last, unsigned char *Te){
    uint8_t i;
    for (i=0; i<16; i++) {
        Te[i] = packet_last[i+5];
    }
}

/*
void lenc(unsigned char *Y, unsigned short *lenC){
    
    unsigned short a;
    uint8_t i;
    for (i=15; i>11; i--) {
        a = Y[i]*128;
        lenC[i-8] = a;
    }
    for (i=0; i<8; i++) {
        if ((lenC[i] == 0) & (lenC[i+1] != 0)) {
            lenC[i+1] = lenC[i+1] - 128;
        }
    }
}
*/
void inversion(unsigned char *lenC){
    unsigned char b;
    unsigned char c;
    uint8_t i;
    for (i=0; i<9; i+=2) {
        b = lenC[15-i];
        c = lenC[14-i];
        lenC[14-i] = b;
        lenC[15-i] = c;
    }
}

void tagCheck(unsigned char *T, unsigned char *Te){
    uint8_t i;
    for (i=0; i<16; i++) {
        if (T[i] - Te[i]) {
            printf("Tag do not match ! Discard data \n");
            break;
        }
    }
}

void shortToChar(unsigned char *buff_in, unsigned char *buffer_in){
    uint8_t i;
    for (i=0; i<16; i++) {
        buffer_in [i] = buff_in[i];
    }
}

void charToShort(unsigned char *buffer_out, unsigned char *buff_out){
    uint8_t i;
    for (i=0; i<16; i++) {
        buff_out [i] = buffer_out[i];
    }
}

void print_short(unsigned short *buff_out){
    int j;
    for(j = 0; j < 8; j++) {
        printf("%x ", buff_out[j]);
    }
    printf("\n");
}

void circularBuffer(unsigned char *data_in, unsigned char *buffer, unsigned char *data_out, uint16_t *p1, uint16_t *p2, uint8_t *data_ready, uint8_t *exception){
    uint8_t i;
    uint8_t j;
    uint16_t a;
    uint16_t b;
    for (i=0; i<15; i++) {
        buffer[((*p2)+i)%(128)] = data_in[i];
    }
    if ((*exception)) {
        (*p2) = ((*p2) + 16)%(128);
    }
    else{
        (*p2) = ((*p2) + 15)%(128);
    }
    (*data_ready) = 0x00;
    a = *p1;
    b = *p2;
    
    if (((*p2)-(*p1))>15) {
        for (j=0; j<16; j++) {
            data_out[j] = buffer[((*p1)+j)%(128)];
        }
        (*p1) = ((*p1) + 16)%(128);
        (*data_ready) = 0x01;
    }
    else if ((a>b)&(((128-(*p1))+(*p2))>15)) {
        for (j=0; j<16; j++) {
            data_out[j] = buffer[((*p1)+j)%(128)];
        }
        (*p1) = ((*p1) + 16)%(128);
        (*data_ready) = 0x01;
    }
}

void circularBuffer_out(unsigned char *data_in_d, unsigned char *buffer, unsigned char *data_out_d, uint16_t *p1_out, uint16_t *p2_out, uint8_t *data_ready_out){
    
    uint8_t i;
    uint8_t j;
    uint16_t a;
    uint16_t b;
    
    for (i=0; i<16; i++) {
        buffer[((*p2_out)+i)%(128)] = data_in_d[i];
    }
    (*p2_out) = ((*p2_out) + 16)%(128);
    (*data_ready_out) = 0x00;
    a = *p1_out;
    b = *p2_out;
    
    if (((*p2_out)-(*p1_out))>14) {
        for (j=0; j<15; j++) {
            data_out_d[j] = buffer[((*p1_out)+j)%(128)];
        }
        (*p1_out) = ((*p1_out) + 15)%(128);
        (*data_ready_out) = 0x01;
    }
    else if ((a>b)&(((128-(*p1_out))+(*p2_out))>14)) {
        for (j=0; j<15; j++) {
            data_out_d[j] = buffer[((*p1_out)+j)%(128)];
        }
        (*p1_out) = ((*p1_out) + 15)%(128);
        (*data_ready_out) = 0x01;
    }
}



