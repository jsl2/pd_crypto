//
//  Encryption.h
//  P&D
//
//  Created by Valere MOUTARLIER on 28/03/19.
//  Copyright © 2019 Augustin MOUTARLIER. All rights reserved.
//

#ifndef Encryption_h
#define Encryption_h

#include <stdio.h>
#include "aes.h"

void encryption(unsigned char *IV, aes_key *key, unsigned char *lenC, unsigned char *buffer_out, unsigned char *buffer_in, uint16_t *numBits);

#endif /* Encryption_h */
