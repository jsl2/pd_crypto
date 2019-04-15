//
//  Decryption.h
//  P&D
//
//  Created by Augustin Moutarlier on 28/03/2019.
//  Copyright © 2019 Augustin MOUTARLIER. All rights reserved.
//

#ifndef Decryption_h
#define Decryption_h

#include <stdio.h>
#include "aes.h"

void decryption(unsigned char *data, aes_key *key, unsigned char *buffer_out);

#endif /* Decryption_h */
