//
//  Decryption_functions.h
//  P&D
//
//  Created by Augustin Moutarlier on 25/03/2019.
//  Copyright © 2019 Augustin MOUTARLIER. All rights reserved.
//

#ifndef Decryption_functions_h
#define Decryption_functions_h

#include <stdio.h>
#include "lib/aes.h"

void seqNumCheck(unsigned char *seqNum, unsigned char *Y);
void incr_d(unsigned char *Y);
void ghash_d(uint16_t *H, unsigned char *Pi, unsigned char *Ci, unsigned char *Y, aes_key *key, uint16_t *T);
void tag_d(uint16_t *T, unsigned char *lenC, uint16_t *T0, uint16_t *H);

#endif /* Decryption_functions_h */
