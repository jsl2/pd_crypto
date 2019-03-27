#include <stdint.h>
#include <stdio.h>
#include "util.h"

#define RUN_TEST(test, x, y) {if (is_equal(x,y) == TRUE) { printf(test); printf(" passed!\n"); } else { printf(test); printf(" failed!\n");} }

uint16_t a[MAX_SIZE] = {0x8, 0x4867, 0x3767, 0x2d60, 0xa503, 0x7fe4, 0xa540, 0xc070, 0x67e7};
uint16_t b[MAX_SIZE] = {0x8, 0x78f9, 0x4952, 0x2293, 0xabbe, 0xb0c7, 0x2fb9, 0xe566, 0x4c4c};
/* e = gcd(a,b) */
uint16_t e[MAX_SIZE] = {0x1, 0x0001};
/* f = a^{-1} mod b */
uint16_t f[MAX_SIZE] = {0x8, 0x778c, 0x6c90, 0xa80c, 0xe431, 0x4674, 0x1b2c, 0x02a4, 0x3e99};
uint16_t m[MAX_SIZE] = {0x7, 0x78fa, 0xe4d2, 0x7165, 0x9ded, 0x9b72, 0x10f1, 0x1d56};
/* g = a / m  */
uint16_t g[MAX_SIZE] = {0x2, 0x8ab8, 0x0003};
/* h = a mod b */
uint16_t h[MAX_SIZE] = {0x8, 0xcf6e, 0xee14, 0xacc, 0xf945, 0xcf1c, 0x7586, 0xdb0a, 0x1b9a};
/* i = a * b */
uint16_t i[MAX_SIZE] = {0x10, 0xb42f, 0xdd63, 0xb06d, 0x0758, 0x5f64, 0x8fbe, 0x1be2, 0x03a2,
                        0x7400, 0x74d7, 0xb0f8, 0x81df, 0x9061, 0x5b5a, 0x030a, 0x1ef8};
/* j = a + b */
uint16_t j[MAX_SIZE] = {0x9, 0xc160, 0x80b9, 0x4ff3, 0x50c1, 0x30ac, 0xd4fa, 0xa5d6, 0xb434, 0x0000};
/* k = a - b */
uint16_t k[MAX_SIZE] = {0x8, 0xcf6e, 0xee14, 0x0acc, 0xf945, 0xcf1c, 0x7586, 0xdb0a, 0x1b9a};
uint16_t s = 0x2b73;
/* l = s * a */
uint16_t l[MAX_SIZE] = {0x9, 0xd345, 0x3c8e, 0x8b87, 0xa90c, 0xdb6d, 0x1174, 0x5e5c, 0x9a6e, 0x11a2};

uint16_t p[MAX_SIZE] = {0x8, 0xda84, 0xba01, 0xf583, 0xb33d, 0xd97a, 0x865f, 0x57b4, 0x16b8};
uint16_t q[MAX_SIZE] = {0x8, 0xcbfa, 0x5c6b, 0x8a92, 0xa8f9, 0xdf4c, 0x9055, 0xdef5, 0x213e};
uint16_t r[MAX_SIZE] = {0x8, 0xd639, 0xfe13, 0x6b1b, 0xb86c, 0xbd13, 0x1212, 0x7ca7, 0x43e3};
uint16_t R[MAX_SIZE] = {0x9, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0001};
uint16_t R_square[MAX_SIZE] = {0x11, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0001};
uint16_t r_prime[MAX_SIZE] = {0x1, 0xC7F7};
uint16_t base[MAX_SIZE] = {0x2, 0x0000, 0x0001};
uint16_t R_square_mod_r[MAX_SIZE] = {0x8, 0xe589, 0x4bda, 0xdad7, 0x4355, 0x3470, 0x7f65, 0x704d, 0x36d3};
uint16_t R_mod_r[MAX_SIZE] = {0x8, 0x7d55, 0x5c4, 0xbeac, 0xd6ba, 0xc8c4, 0xc9c7, 0x8a0a, 0x3455};
uint16_t p_square[MAX_SIZE] = {0x10, 0x1410, 0x3f8d, 0xfca3, 0x2c34, 0xb016, 0xee8a, 0x6fb, 0x2026, 0xa194, 0x52cb, 0x533, 0x1a54, 0xd5cc, 0x38a5, 0x33d1, 0x204};
uint16_t p_mod_q[MAX_SIZE] = {0x8, 0xda84, 0xba01, 0xf583, 0xb33d, 0xd97a, 0x865f, 0x57b4, 0x16b8};
uint16_t pq_mod_r[MAX_SIZE] = {0x8, 0x1008, 0x40d6, 0xbd4b, 0xf112, 0x91b5, 0xd9a3, 0x9163, 0x3cc1};
uint16_t p_pow_q_mod_r[MAX_SIZE] = {0x8, 0x7380, 0x18ea, 0x7918, 0x9e48, 0x925d, 0x9a0c, 0xe689, 0xd75};

uint16_t u[MAX_SIZE] = {0x1, 383};
uint16_t v[MAX_SIZE] = {0x1, 271};

uint16_t neg_one[MAX_SIZE] = {0x8, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
uint16_t neg_two[MAX_SIZE] = {0x8, 0xFFFE, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
uint16_t max_neg[MAX_SIZE] = {0x8, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8000};
uint16_t max_neg_plus[MAX_SIZE] = {0x8, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8000};
uint16_t max_neg_twice[MAX_SIZE] = {0x9, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFFFF};
uint16_t max_neg_min[MAX_SIZE] = {0x9, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x7FFF, 0xFFFF};
uint16_t max_pos[MAX_SIZE] = {0x8, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x7FFF};
uint16_t zero[MAX_SIZE] = {0x8, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000};
uint16_t one[MAX_SIZE] = {0x8, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000};
uint16_t two[MAX_SIZE] = {0x8, 0x0002, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000};
uint16_t test_array[MAX_SIZE] = {0};
uint16_t test_array2[MAX_SIZE] = {0};
uint16_t test_array3[MAX_SIZE] = {0};

int main(void) {
    mp_add(a, b, test_array);
    RUN_TEST("Addition test a + b", test_array, j);

    mp_add(neg_one, two, test_array);
    RUN_TEST("Addition test -1 + 2", test_array, one);

    mp_add(two, neg_one, test_array);
    RUN_TEST("Addition test 2 + -1", test_array, one);

    mp_add(zero, neg_one, test_array);
    RUN_TEST("Addition test 0 + -1", test_array, neg_one);

    mp_add(max_neg, neg_one, test_array);
    RUN_TEST("Addition test (max neg) + -1", test_array, max_neg_min);

    mp_add(neg_one, max_neg, test_array);
    RUN_TEST("Addition test -1 + (max neg)", test_array, max_neg_min);

    mp_add(max_neg_plus, neg_one, test_array);
    RUN_TEST("Addition test (max neg + 1) + -1", test_array, max_neg);

    mp_add(max_neg, max_neg, test_array);
    RUN_TEST("Addition test (max neg) + (max neg)", test_array, max_neg_twice);

    mp_add(neg_one, neg_one, test_array);
    RUN_TEST("Addition test -1 + -1", test_array, neg_two);

    mp_add(neg_one, one, test_array);
    RUN_TEST("Addition test -1 + 1", test_array, zero);

    mp_sub(zero, neg_one, test_array);
    RUN_TEST("Subtraction test 0 - -1", test_array, one);

    mp_sub(max_neg, neg_one, test_array);
    RUN_TEST("Subtraction test (max neg) - -1", test_array, max_neg_plus);

    mp_sub(max_neg, one, test_array);
    RUN_TEST("Subtraction test (max neg) - 1", test_array, max_neg_min);

    mp_sub(neg_one, max_neg, test_array);
    RUN_TEST("Subtraction test -1 - (max neg)", test_array, max_pos);

    mp_sub(max_neg_plus, one, test_array);
    RUN_TEST("Subtraction test (max neg + 1) - 1", test_array, max_neg);

    mp_sub(max_neg, max_neg, test_array);
    RUN_TEST("Subtraction test (max neg) - (max neg)", test_array, zero);

    mp_sub(neg_one, neg_one, test_array);
    RUN_TEST("Subtraction test -1 - -1", test_array, zero);

    mp_sub(neg_one, one, test_array);
    RUN_TEST("Subtraction test -1 - 1", test_array, neg_two);

    mp_sub(a, b, test_array);
    RUN_TEST("Subtraction test a - b", test_array, k);

    mp_mult(a, b, test_array);
    RUN_TEST("Multiplication test a * b", test_array, i);

    mp_mult_scalar(a, s, test_array);
    RUN_TEST("Multiplication test s * a", test_array, l);

    mp_div(a, m, test_array, test_array2);
    RUN_TEST("Division test a / m", test_array, g);

    mp_div(a, b, test_array, test_array2);
    RUN_TEST("Modulo test a mod b", test_array2, h);

    binary_extended_gcd(b, a, test_array, test_array2, test_array3);
    RUN_TEST("GCD test: gcd(a,b)", test_array3, e);

    mul_inv(a, b, test_array);
    RUN_TEST("Modular inverse a^{-1} mod b", test_array, f);

    mp_div(R, q, test_array2, test_array3);
    barret_reduction(p, q, test_array2, test_array);
    RUN_TEST("p mod q (barret reduction)", test_array, p_mod_q);

    mp_square(p, test_array);
    RUN_TEST("p*p", test_array, p_square);

    mp_square(R, test_array);
    RUN_TEST("R*R", test_array, R_square);

    /* This divide needs debugging!
     * mp_div(R_square, r, test_array, test_array2);
    RUN_TEST("R*R mod r", test_array2, R_square_mod_r);
    print_radix(test_array);*/

    mul_inv(r, base, test_array);
    twos_complement(test_array, test_array2);
    RUN_TEST("r prime", test_array2, r_prime);

    mont_mult(p, q, r, r_prime, test_array);
    mont_mult(R_square_mod_r, test_array, r, r_prime, test_array2);
    RUN_TEST("Montgomery multiplication pq mod r", test_array2, pq_mod_r);

    mont_exp(p, q, r, r_prime, R_mod_r, R_square_mod_r, test_array);
    RUN_TEST("Montgomery exponentiation p^q mod r", test_array, p_pow_q_mod_r);
    return 0;
}