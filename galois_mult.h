//
//  Galois Multiplication.h
//  P&D
//
//  Created by Augustin Moutarlier on 10/03/2019.
//  Copyright © 2019 Augustin MOUTARLIER. All rights reserved.
//

#ifndef Galois_Multiplication_h
#define Galois_Multiplication_h

void increment(unsigned char *Y);

void increment_lenC(unsigned char *lenC);
void right_shift(uint8_t *V);
void xor(unsigned char *a, unsigned char *b);
void gmult(uint16_t *X, uint16_t *W, uint16_t*Z);

#endif /* Galois_Multiplication_h */
