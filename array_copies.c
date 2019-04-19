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

void tag_check(unsigned char *T, unsigned char *Te) {
    uint8_t i;
    for (i=0; i<16; i++) {
        if (T[i] != Te[i]) {
            printf("Tag does not match ! Discard data \n");
            break;
        }
    }
}

