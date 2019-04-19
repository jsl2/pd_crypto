/*
  Galois Multiplication.h
  P&D

  Created by Augustin Moutarlier on 10/03/2019.
  Copyright © 2019 Augustin MOUTARLIER. All rights reserved.
*/

#ifndef Galois_Multiplication_h
#define Galois_Multiplication_h

void increment(uint8_t *Y);

void increment_lenC(uint8_t *lenC);
void right_shift(uint8_t *V);

void xor(uint8_t *a, uint8_t *b);
void gmult(uint16_t *X, uint16_t *W, uint16_t*Z);

#endif /* Galois_Multiplication_h */
