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

void seq_num_check(unsigned char *Y, unsigned char *packet);
void ghash_d(uint16_t *H, unsigned char *Pi, unsigned char *Ci, unsigned char *Y, aes_key *key, uint16_t *T);

#endif /* Decryption_functions_h */
