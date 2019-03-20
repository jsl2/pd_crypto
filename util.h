#ifndef _UTIL_H_INCL
#define _UTIL_H_INCL

#define SUCCESS 1
#define FAILURE 0

#define MAX_SIZE 512

void print_radix(uint16_t* x);

uint32_t zero_pad(uint16_t* x, uint16_t zeros);

uint32_t mp_add(uint16_t* x, uint16_t* y, uint16_t* w);

uint32_t mp_sub(uint16_t* x, uint16_t* y, uint16_t* w);

uint32_t mp_mult(uint16_t* x, uint16_t* y, uint16_t* w);

uint32_t mp_mult_scalar(uint16_t* x, uint16_t y, uint16_t* w);

uint32_t mp_div(uint16_t* x, uint16_t* y, uint16_t* q, uint16_t* r);

uint32_t check_equal(uint16_t* x, uint16_t* y);

uint32_t check_gteq(uint16_t* x, uint16_t* y);

uint32_t check_gt(uint16_t* x, uint16_t* y);

uint32_t binary_extended_gcd(uint16_t* x, uint16_t* y, uint16_t* a, uint16_t* b, uint16_t* gcd);

#endif