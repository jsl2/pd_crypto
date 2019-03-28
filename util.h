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

#define SUCCESS 1
#define FAILURE 0

#define MAX_SIZE 1024

void print_radix(uint16_t* x);

void mp_copy(uint16_t* src, uint16_t* dest);

void mul_inv(uint16_t *x, uint16_t *y, uint16_t *w);

void sign_extend(uint16_t* x, uint16_t zeros);

void mp_add(uint16_t* x, uint16_t* y, uint16_t* w);

void mp_sub(uint16_t* x, uint16_t* y, uint16_t* w);

void mp_mult(uint16_t* x, uint16_t* y, uint16_t* w);

void mp_square(uint16_t *x, uint16_t *w);

void mp_mult_scalar(uint16_t* x, uint16_t y, uint16_t* w);

void mp_div(uint16_t* x, uint16_t* y, uint16_t* q, uint16_t* r);

void binary_extended_gcd(uint16_t* x, uint16_t* y, uint16_t* a, uint16_t* b, uint16_t* gcd);

void barret_reduction(uint16_t* x, uint16_t* m, uint16_t* mew, uint16_t* r);

void twos_complement(uint16_t *x, uint16_t *w);

void dh_mon_exp(uint16_t *x, uint16_t *e, uint16_t *m, uint16_t *m_prime, uint16_t *R, uint16_t *a);

uint32_t mont_mult(uint16_t *x, uint16_t *y, uint16_t *m, uint16_t *m_prime, uint16_t *a);

uint32_t mont_exp(uint16_t *x, uint16_t *e, uint16_t *m, uint16_t *m_prime, uint16_t *R_mod_m, uint16_t *R, uint16_t *a);

uint32_t is_equal(uint16_t *x, uint16_t *y);

uint32_t is_gteq(uint16_t *x, uint16_t *y);

uint32_t is_gteq_u(uint16_t *x, uint16_t *y);

uint32_t is_gt(uint16_t *x, uint16_t *y);

uint32_t is_zero(uint16_t *x);

void remove_leading_zeros(uint16_t *x);

#endif