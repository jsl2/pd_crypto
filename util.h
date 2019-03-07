#ifndef _UTIL_H_INCL
#define _UTIL_H_INCL

#define SUCCESS 0
#define FAILURE 1

void print_radix(uint16_t* x);

uint32_t mp_add(uint16_t* x, uint16_t* y, uint16_t* w);

uint32_t mp_sub(uint16_t* x, uint16_t* y, uint16_t* w);

uint32_t mp_mult(uint16_t* x, uint16_t* y, uint16_t* w);

uint32_t check_equal(uint16_t* x, uint16_t* y);

#endif