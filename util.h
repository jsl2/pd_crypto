#ifndef _UTIL_H_INCL
#define _UTIL_H_INCL

#define ZERO    ((uint16_t) 0)
#define ONE     ((uint16_t) 1)
#define NEG_ONE ((uint16_t) 0xFFFF)
#define MAX_NEG ((uint16_t) 0x8000)

#define IS_EVEN(x) (!((x) & (uint16_t) ONE))
#define IS_NEGATIVE(x) ((x) & (uint16_t) MAX_NEG)

#define TRUE 1
#define FALSE 0

#define MAX_SIZE 512

void print_radix(uint16_t* x);

void mp_copy(uint16_t* src, uint16_t* dest);

void mul_inv(uint16_t *x, uint16_t *y, uint16_t *w);

void sign_extend(uint16_t* x, uint16_t zeros);

void mp_add(uint16_t* x, uint16_t* y, uint16_t* w);

void mp_sub(uint16_t* x, uint16_t* y, uint16_t* w);

void mp_mult(uint16_t* x, uint16_t* y, uint16_t* w);

void mp_mult_scalar(uint16_t* x, uint16_t y, uint16_t* w);

void mp_div(uint16_t* x, uint16_t* y, uint16_t* q, uint16_t* r);

void binary_extended_gcd(uint16_t* x, uint16_t* y, uint16_t* a, uint16_t* b, uint16_t* gcd);

uint32_t is_equal(uint16_t *x, uint16_t *y);

uint32_t is_gteq(uint16_t *x, uint16_t *y);

uint32_t is_gt(uint16_t *x, uint16_t *y);

#endif