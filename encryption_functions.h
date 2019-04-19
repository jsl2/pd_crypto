//
//  Encryption_functions.h
//  P&D
//
//  Created by Augustin Moutarlier on 12/03/2019.
//  Copyright © 2019 Augustin MOUTARLIER. All rights reserved.
//

#ifndef Encryption_functions_h
#define Encryption_functions_h

#include <stdio.h>
#include "lib/aes.h"

void incr(unsigned char *Y);
void ghash_e(uint16_t *H, unsigned char *Pi, unsigned char *Ci, unsigned char *Y, aes_key *key, uint16_t *T);
void tag(uint16_t *T, unsigned char *lenC, uint16_t *T0, uint16_t *H);

#endif /* Encryption_functions_h */
