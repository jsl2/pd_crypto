//
//  array_copies.h
//  P&D
//
//  Created by Augustin Moutarlier on 25/03/2019.
//  Copyright © 2019 Augustin MOUTARLIER. All rights reserved.
//

#ifndef array_copies_h
#define array_copies_h

#include <stdio.h>
#include "lib/aes.h"

void initialization(unsigned char *Y, unsigned char *IV, aes_key *key, uint16_t *H, uint16_t *T0, uint16_t *T);

void cbts(unsigned char *buffer, unsigned char *Pi, uint8_t j);
void cstb(unsigned char *Ci, unsigned char *C, uint8_t j);
void npIt(uint16_t *numBits, uint8_t *np);

void data_1(unsigned char *Y, unsigned char *packet_1);
void data_int(unsigned char *Y, unsigned char *Ci, unsigned char *packet_int);
void data_last(unsigned char *Y,unsigned char *Te, unsigned char *packet_last);

void packet_1_d(unsigned char *packet_1, unsigned char *IV);
void packet_int_d(unsigned char *packet_int, unsigned char *Ci);
void packet_last_d(unsigned char *packet_last, unsigned char *Te);

//void lenc(unsigned char *Y, unsigned short *lenC);
void inversion(unsigned char *lenC);

void tag_check(unsigned char *T, unsigned char *Te);
void shortToChar(unsigned char *buff_in, unsigned char *buffer_in);
void charToShort(unsigned char *buffer_out, unsigned char *buff_out);
void print_short(unsigned short *buff_out);

void circularBuffer(unsigned char *data_in, unsigned char *buffer, unsigned char *data_out, uint16_t *p1, uint16_t *p2, uint8_t *data_ready, uint8_t *exception);
void circularBuffer_out(unsigned char *data_in_d, unsigned char *buffer, unsigned char *data_out_d, uint16_t *p1_out, uint16_t *p2_out, uint8_t *data_ready_out);

#endif /* array_copies_h */
