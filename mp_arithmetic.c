#include <stdint.h>
#include <stdio.h>
#include "mp_arithmetic.h"

void print_radix(uint16_t *x) {
    uint16_t i = 0;
    printf("{%#x, ", x[0]);
    for (i = 1; i < x[0]; i++) {
        printf("%#06x, ", x[i]);
    }
    printf("%#06x};\n", x[x[0]]);
}

void print_radix_number(uint16_t *x) {
    uint16_t i = 0;
    for (i = x[0]; i > 0; i--) {
        printf("%04x", x[i]);
    }
    printf("\n");
}

/*
 * Decrease length of x until MSB is not 0
 */
void remove_leading_zeros(uint16_t *x) {
    uint16_t i;
    for (i = x[0]; x[i] == 0 && i > 0; i--)
        x[0]--;
}

/*
 * Performs multi-precision twos complement.
 * Special case: 0x1, 0x8000 -> 0x2, 0x8000, 0x0000
 *               to indicate change of sign
 */
void twos_complement(uint16_t *x, uint16_t *w) {
    uint16_t i;
    uint16_t c = 0;
    for (i = 1; i <= x[0]; i++)
        w[i] = ~x[i];
    w[1] = w[1] + ONE;
    c = (uint16_t) (w[1] == 0);
    for (i = 2; i < x[0] + ONE && c; i++) {
        w[i] = w[i] + c;
        c = (uint16_t) (w[i] == 0);
    }
    w[0] = x[0];
    if (w[x[0]] == MAX_NEG && x[x[0]] == MAX_NEG) {
        w[0]++;
        w[x[0] + 1] = 0x0;
    }
}

/*
 * Sign extend such that length of x is x[0] + length
 */
void sign_extend(uint16_t *x, uint16_t length) {
    uint16_t i;
    uint16_t neg;
    neg = IS_NEGATIVE(x[x[0]]);
    for (i = x[0] + ONE; i <= x[0] + length; i++) {
        x[i] = neg ? NEG_ONE : ZERO;
    }
    x[0] += length;
}

/*
 * Sign extend such that length of x is x[0] + length
 */
void zero_pad(uint16_t *x, uint16_t length) {
    uint16_t i;

    for (i = x[0] + ONE; i <= x[0] + length; i++) {
        x[i] = ZERO;
    }
    x[0] += length;
}

/*
 * Signed arbitrary precision addition.
 * Size of output may exceed size of largest input.
 * If x and y have different sign, output will not exceed size of largest input.
 *
 * Based on algorithm 14.7 - Handbook of applied cryptography
 */
void mp_add(uint16_t *x, uint16_t *y, uint16_t *w) {
    uint16_t c = 0;
    uint16_t i = 0;
    uint16_t n = 0;
    uint16_t x_neg;
    uint16_t y_neg;
    uint16_t temp[MAX_SIZE] = {0};
    uint16_t x_size = x[0];
    uint16_t y_size = y[0];

    if (x_size != y_size) {
        if (x_size > y_size)
            sign_extend(y, x_size - y_size);
        else
            sign_extend(x, y_size - x_size);
    }
    x_neg = IS_NEGATIVE(x[x[0]]);
    y_neg = IS_NEGATIVE(y[y[0]]);
    if (y_neg) {
        twos_complement(y, temp);
        mp_sub(x, temp, w);
        x[0] = x_size;
        y[0] = y_size;
        return;
    }

    n = x[0];
    for (i = 0; i < n; i++) {
        w[i + 1] = x[i + 1] + y[i + 1] + c;
        if (!(i > 0 && (!w[i + 1]) && c))
            c = (uint16_t) (((uint32_t) x[i + 1] + (uint32_t) y[i + 1] + c) > 0xFFFF);
    }
    w[0] = n;
    if (c > 0 && !x_neg) {
        w[n + 1] = 1;
        w[0]++;
    } else if (!x_neg && IS_NEGATIVE(w[n])) {
        w[n + 1] = 0;
        w[0]++;
    }
    x[0] = x_size;
    y[0] = y_size;
}

/*
 * Unsigned arbitrary precision addition.
 * Size of output may exceed size of largest input.
 * If x and y have different sign, output will not exceed size of largest input.
 *
 * Based on algorithm 14.7 - Handbook of applied cryptography
 */
void mp_add_u(uint16_t *x, uint16_t *y, uint16_t *w) {
    uint16_t c = 0;
    uint16_t i = 0;
    uint16_t n = 0;
    uint16_t x_size = x[0];
    uint16_t y_size = y[0];

    if (x_size != y_size) {
        if (x_size > y_size)
            zero_pad(y, x_size - y_size);
        else
            zero_pad(x, y_size - x_size);
    }

    n = x[0];
    for (i = 0; i < n; i++) {
        w[i + 1] = x[i + 1] + y[i + 1] + c;
        c = (uint16_t) (((uint32_t) x[i + 1] + (uint32_t) y[i + 1] + c) > 0xFFFF);
    }
    w[0] = n;
    if (c > 0) {
        w[n + 1] = 1;
        w[0]++;
    }
    x[0] = x_size;
    y[0] = y_size;
}

/*
 * Signed arbitrary precision subtraction.
 * Size of output may exceed size of largest input.
 * If x and y have the same sign, output will not exceed size of largest input.
 *
 * Based on algorithm 14.9 - Handbook of applied cryptography
 */
void mp_sub(uint16_t *x, uint16_t *y, uint16_t *w) {
    uint16_t i = 0;
    uint16_t c = 0;
    uint16_t n;
    uint16_t temp[MAX_SIZE] = {0};
    uint16_t x_neg;
    uint16_t y_neg;
    uint16_t x_size = x[0];
    uint16_t y_size = y[0];

    if (x_size != y_size) {
        if (x_size > y_size) {
            sign_extend(y, x_size - y_size);
        } else {
            sign_extend(x, y_size - x_size);
        }
    }
    x_neg = IS_NEGATIVE(x[x[0]]);
    y_neg = IS_NEGATIVE(y[y[0]]);
    if (y_neg) {
        twos_complement(y, temp);
        mp_add(x, temp, w);
        x[0] = x_size;
        y[0] = y_size;
        if (w[w[0]] == 0 && w[0] > x_size)
            w[0]--;
        return;
    }

    n = x[0];

    for (i = 0; i < n; i++) {
        w[i + 1] = x[i + 1] - y[i + 1] - c;
        c = (uint16_t) ((y[i + 1] + c) > x[i + 1]);
    }
    w[0] = x[0];
    if (x_neg && !IS_NEGATIVE(w[n])) {
        w[0]++;
        w[n + 1] = NEG_ONE;
    }
    x[0] = x_size;
    y[0] = y_size;
}

void mp_sub_u(uint16_t *x, uint16_t *y, uint16_t *w) {
    uint16_t i = 0;
    uint16_t c = 0;
    uint16_t n;
    uint16_t x_size = x[0];
    uint16_t y_size = y[0];

    if (x_size != y_size) {
        if (x_size > y_size) {
            zero_pad(y, x_size - y_size);
        } else {
            zero_pad(x, y_size - x_size);
        }
    }

    n = x[0];

    for (i = 0; i < n; i++) {
        w[i + 1] = x[i + 1] - y[i + 1] - c;
        c = (uint16_t) ((y[i + 1] + c) > x[i + 1]);
    }
    w[0] = x[0];
    x[0] = x_size;
    y[0] = y_size;
}

/*
 * Multiple-precision multiplication
 * Algorithm 14.12 - Handbook of applied cryptography
 */
void mp_mult(uint16_t *x, uint16_t *y, uint16_t *w) {
    uint16_t i, j;
    uint16_t c;
    uint32_t uv = 0;
    for (i = 0; i < x[0] + y[0]; i++)
        w[i + 1] = 0;
    for (i = 0; i < y[0]; i++) {
        c = 0;
        for (j = 0; j < x[0]; j++) {
            uv = w[i + j + 1] + ((uint32_t) x[j + 1]) * ((uint32_t) y[i + 1]) + c;
            w[i + j + 1] = (uint16_t) uv;
            c = (uint16_t) (uv >> 16u);
        }
        w[i + x[0] + 1] = (uint16_t) (uv >> 16u);
    }
    if (uv >> 16u) {
        w[0] = x[0] + y[0];
    } else {
        w[0] = x[0] + y[0] - ONE;
    }
}

void mp_square(uint16_t *x, uint16_t *w) {
    uint16_t i, j;
    uint32_t c;
    uint64_t uv = 0;
    uint64_t temp = 0;

    /* Treat input as positive */
    if (IS_NEGATIVE(x[x[0]])) {
        zero_pad(x, 1);
    }

    for (i = 0; i < 2 * x[0]; i++)
        w[i + 1] = 0;
    for (i = 0; i < x[0]; i++) {
        uv = (uint64_t) w[2 * i + 1] + (uint64_t) x[i + 1] * (uint64_t) x[i + 1];
        w[2 * i + 1] = (uint16_t) uv;
        c = (uint32_t) (uv >> 16u);
        for (j = i + ONE; j < x[0]; j++) {
            uv = (uint64_t) w[i + j + 1];
            temp = (uint64_t) x[j + 1] * (uint64_t) x[i + 1];
            temp <<= 1;
            uv += (temp + c);
            w[i + j + 1] = (uint16_t) uv;
            c = (uint32_t) (uv >> 16u);
        }
        w[i + x[0] + 1] = (uint16_t) (uv >> 16u);
        w[i + x[0] + 2] = (uint16_t) (uv >> 32u);
    }
    w[0] = (uint16_t) 2 * x[0];
    remove_leading_zeros(w);
}

/*
 * Multiple-precicison multiplication modified such that one operand
 * is a single precision integer
 */
void mp_mult_scalar(uint16_t *x, uint16_t y, uint16_t *w) {
    uint16_t i, j;
    uint16_t c;
    uint32_t uv;
    for (i = 0; i <= x[0] + 2; i++)
        w[i + 1] = 0;

    c = 0;
    for (j = 0; j < x[0]; j++) {
        uv = ((uint32_t) x[j + 1]) * ((uint32_t) y) + c;
        w[j + 1] = (uint16_t) uv;
        c = (uint16_t) (uv >> 16u);
    }
    if (c) {
        w[x[0] + 1] = c;
        w[0] = x[0] + ONE;
    } else {
        w[0] = x[0];
    }
}

/*
 * Copy multiple precision integer
 */
void mp_copy(uint16_t *src, uint16_t *dest) {
    uint16_t i;
    for (i = 0; i <= src[0]; i++)
        dest[i] = src[i];
}

/*
 * Right shift by one bit.
 * Maintains sign bit of input.
 */
void mp_diveq2(uint16_t *x) {
    uint16_t i;
    uint16_t is_neg = IS_NEGATIVE(x[x[0]]);
    for (i = 1; i <= x[0]; i++) {
        x[i] >>= 1;
        if ((i < x[0]) && (!IS_EVEN(x[i + 1])))
            x[i] |= MAX_NEG;
    }
    if (is_neg)
        x[x[0]] |= MAX_NEG;
}

/*
 * Left shift by one bit.
 * Maintains sign bit of input.
 */
void mp_muleq2(uint16_t *x) {
    uint16_t i, t;
    uint16_t c = 0;

    for (i = 1; i <= x[0]; i++) {
        t = x[i] & MAX_NEG;
        x[i] <<= 1;
        if (c)
            x[i] |= 1;
        c = t;
    }
    /* Sign extend or zero pad if overflow into sign bit */
    if (c) {
        x[0]++;
        x[x[0]] = 0xFFFF;
    } else if (IS_NEGATIVE(x[x[0]])) {
        x[0]++;
        x[x[0]] = 0x0;
    }
}

/*
 * Binary extended gcd algorithm
 * See algorithm 14.61 - Handbook of applied cryptography
 * www.ucl.ac.uk/~ucahcjm/combopt/ext_gcd_python_programs.pdf
 */
void binary_extended_gcd(uint16_t *x, uint16_t *y, uint16_t *a, uint16_t *b, uint16_t *gcd) {
    uint16_t x_[MAX_SIZE] = {0};
    uint16_t y_[MAX_SIZE] = {0};
    uint16_t u[MAX_SIZE] = {0};
    uint16_t v[MAX_SIZE] = {0};
    uint16_t g[MAX_SIZE] = {0};
    uint16_t A[MAX_SIZE] = {0};
    uint16_t B[MAX_SIZE] = {0};
    uint16_t C[MAX_SIZE] = {0};
    uint16_t D[MAX_SIZE] = {0};
    uint16_t temp[MAX_SIZE] = {0};
    mp_copy(x, x_);
    mp_copy(y, y_);

    g[0] = 1;
    g[1] = 1;
    while (IS_EVEN(x_[1]) && IS_EVEN(y_[1])) {
        mp_diveq2(x_);
        mp_diveq2(y_);
        mp_muleq2(g);
    }
    mp_copy(x_, u);
    mp_copy(y_, v);
    A[0] = 1;
    B[0] = 1;
    C[0] = 1;
    D[0] = 1;
    A[1] = 1;
    B[1] = 0;
    C[1] = 0;
    D[1] = 1;
    while (IS_EVEN(u[1])) {
        mp_diveq2(u);
        if (IS_EVEN(A[1]) && IS_EVEN(B[1])) {
            mp_diveq2(A);
            mp_diveq2(B);
        } else {
            mp_add(A, y_, temp);
            mp_copy(temp, A);
            mp_diveq2(A);
            mp_sub(B, x_, temp);
            mp_copy(temp, B);
            mp_diveq2(B);
        }
    }
    while (!is_equal(u, v)) {
        if (IS_EVEN(v[1])) {
            mp_diveq2(v);
            if (IS_EVEN(C[1]) && IS_EVEN(D[1])) {
                mp_diveq2(C);
                mp_diveq2(D);
            } else {
                mp_add(C, y_, temp);
                mp_copy(temp, C);
                mp_diveq2(C);
                mp_sub(D, x_, temp);
                mp_copy(temp, D);
                mp_diveq2(D);
            }
        } else if (is_gteq(u, v)) {
            mp_copy(u, temp);
            mp_copy(v, u);
            mp_copy(temp, v);

            mp_copy(A, temp);
            mp_copy(C, A);
            mp_copy(temp, C);

            mp_copy(B, temp);
            mp_copy(D, B);
            mp_copy(temp, D);
        } else {
            mp_sub(v, u, temp);
            mp_copy(temp, v);
            mp_sub(C, A, temp);
            mp_copy(temp, C);
            mp_sub(D, B, temp);
            mp_copy(temp, D);
        }
    }

    mp_copy(C, a);
    mp_copy(D, b);
    mp_mult(g, v, temp);
    mp_copy(temp, gcd);
    remove_leading_zeros(gcd);
}

/*
 * Returns the multiplicative inverse by applying the binary extended gcd algorithm
 */
void mul_inv(uint16_t *x, uint16_t *y, uint16_t *w) {
    uint16_t temp1[MAX_SIZE] = {0};
    uint16_t temp2[MAX_SIZE] = {0};
    binary_extended_gcd(x, y, w, temp1, temp2);
    if (IS_NEGATIVE(w[w[0]])) {
        mp_add(w, y, temp1);
        mp_copy(temp1, w);
    }
    remove_leading_zeros(w);
}

/*
 * Left shift multiple precision integer by a multiple of 16
 */
void lsh_radix(const uint16_t *x, uint16_t sh, uint16_t *w) {
    uint16_t i;
    for (i = 1; i <= sh + 1; i++)
        w[i] = 0;
    for (i = sh + ONE; i <= x[0] + sh + ONE; i++)
        w[i] = x[i - sh];
    w[0] = x[0] + sh;
}

/*
 * Right shift multiple precision integer by a multiple of 16
 */
void rsh_radix(const uint16_t *x, uint16_t sh, uint16_t *w) {
    uint16_t i;
    if (sh >= x[0]) {
        w[0] = 0;
        w[1] = 0;
        return;
    }
    for (i = 1; i <= x[0] - sh; i++)
        w[i] = 0;
    for (i = 1; i <= x[0] - sh; i++)
        w[i] = x[i + sh];
    w[0] = x[0] - sh;
}


/*
 * Multiple-precision division - signed only
 * Algorithm 14.20 - Handbook of applied cryptography
 */
void mp_div(uint16_t *x, uint16_t *y, uint16_t *q, uint16_t *r) {
    uint16_t i, j;
    uint16_t n = x[0] - ONE;
    uint16_t t = y[0] - ONE;
    uint16_t temp[MAX_SIZE] = {0};
    uint16_t temp2[MAX_SIZE] = {0};
    uint16_t temp3[MAX_SIZE] = {0};

    mp_copy(x, r);
    if (t == 0)
        return;
    if (n < t) {
        if (is_gt(y, x)) {
            q[0] = 1;
            q[0] = 0;
            mp_copy(x, r);
            remove_leading_zeros(r);
            return;
        } else {
            sign_extend(x, t - n);
        }
    }
    for (j = 0; j <= n - t; j++)
        q[j + 1] = 0;
    q[0] = n - t + ONE;
    lsh_radix(y, n - t, temp2);
    while (is_gteq(r, temp2)) {
        q[n - t + 1] += 1;
        mp_sub(r, temp2, temp);
        mp_copy(temp, r);
    }

    for (i = n; i > t; i--) {
        if (r[i + 1] == y[t + 1])
            q[i - t] = NEG_ONE;
        else
            q[i - t] = (uint16_t) ((((uint32_t) r[i + 1] << 16u) + r[i]) / y[t + 1]);
        while (((uint64_t) q[i - t] * (((uint64_t) (y[t + 1]) << 16u) + y[t])) >
               ((uint64_t) r[i + 1] << 32u) + ((uint64_t) r[i] << 16u) + ((uint64_t) r[i - 1]))
            q[i - t] -= 1;
        lsh_radix(y, i - t - ONE, temp2);
        mp_mult_scalar(temp2, q[i - t], temp);
        if (temp[0] < r[0])
            zero_pad(temp, r[0] - temp[0]);
        mp_sub(r, temp, temp3);
        if (IS_NEGATIVE(temp3[temp3[0]])) {
            mp_add(temp3, temp2, r);
            q[i - t] -= 1;
        } else {
            mp_copy(temp3, r);
        }
    }

    remove_leading_zeros(r);
    remove_leading_zeros(q);
}

/*
 * Barrett modular reduction
 * Algorithm 14.42 - Handbook of applied cryptography
 */
void barret_reduction(uint16_t *x, uint16_t *m, uint16_t *mew, uint16_t *r) {
    uint16_t k, x_size, m_size, i;
    uint16_t r1[MAX_SIZE];
    uint16_t q1[MAX_SIZE];
    uint16_t q2[MAX_SIZE];
    uint16_t q3[MAX_SIZE];
    uint16_t r2[MAX_SIZE];
    uint16_t temp[MAX_SIZE];
    uint16_t temp2[MAX_SIZE];
    k = m[0];
    x_size = x[0];
    m_size = m[0];
    rsh_radix(x, k - ONE, q1);
    if (x_size < 2 * k)
        sign_extend(x, (uint16_t) (2 * k) - x_size);
    mp_mult(mew, q1, q2);
    rsh_radix(q2, k + ONE, q3);
    mp_copy(x, r1);
    r1[0] = k + ONE;
    mp_mult(q3, m, r2); /* not all digits of multiplication require computation - can be optimized*/
    r2[0] = k + ONE;
    mp_sub(r1, r2, r);
    if (IS_NEGATIVE(r[r[0]])) {
        temp[0] = k + ONE;
        temp[k + 1] = ONE;
        for (i = 1; i <= k; i++)
            temp[i] = ZERO;
        mp_add(r, temp, temp2);
        mp_copy(temp2, r);
    }
    while (is_gteq(r, m)) {
        mp_sub(r, m, temp);
        mp_copy(temp, r);
    }
    x[0] = x_size;
    m[0] = m_size;
    remove_leading_zeros(r);
}


/*
 * Multiple-precision multiplication - only computed up to limit number of words
 * Returns x * y mod m where m is (2^16)^n
 * Algorithm 14.12 - Handbook of applied cryptography
 */
void mp_mult_l(uint16_t *x, uint16_t *y, uint16_t *w, uint16_t words) {
    uint16_t i, j;
    uint16_t c;
    uint32_t uv = 0;
    for (i = 0; i <= words; i++)
        w[i + 1] = 0;
    for (i = 0; i < y[0]; i++) {
        c = 0;
        for (j = 0; j < x[0]; j++) {
            if (i + j + 1 > words)
                break;
            uv = w[i + j + 1] + ((uint32_t) x[j + 1]) * ((uint32_t) y[i + 1]) + c;
            w[i + j + 1] = (uint16_t) (uv & 0xFFFFu);
            c = (uint16_t) ((uv >> 16u) & 0xFFFFu);
        }
        if (i + x[0] + 1 <= words)
            w[i + x[0] + 1] = (uint16_t) ((uv >> 16u) & 0xFFFFu);
    }
    w[0] = words;
    remove_leading_zeros(w);
}

/*
 * Montgomery product for Diffie Hellman. R = 2^4096
 * */
void dh_mon_pro(uint16_t *x, uint16_t *y, uint16_t *m, uint16_t *m_prime, uint16_t *u) {
    uint16_t temp[MAX_SIZE] = {0};
    uint16_t temp2[MAX_SIZE] = {0};
    uint16_t temp3[MAX_SIZE] = {0};
    uint16_t res[MAX_SIZE] = {0};
    mp_mult(x, y, temp);
    mp_mult_l(temp, m_prime, temp2, 256);
    mp_mult(temp2, m, temp3);
    mp_add_u(temp, temp3, temp2);
    rsh_radix(temp2, 256, res);

    if (is_gteq_u(res, m)) {
        mp_sub_u(res, m, temp);
        mp_copy(temp, res);
    }
    remove_leading_zeros(res);
    mp_copy(res, u);
}

/*
 * Montgomery exponentiation for Diffie Hellman. R = 2^4096
 * */
void dh_mon_exp(uint16_t *x, uint16_t *e, uint16_t *m, uint16_t *m_prime, uint16_t *R, uint16_t *a) {
    uint16_t x_[MAX_SIZE];
    uint16_t temp[MAX_SIZE];
    uint16_t temp2[MAX_SIZE];
    uint16_t one[MAX_SIZE] = {1, 1};
    int32_t t, i, j;
    static int16_t count = 0;

    t = 32 - __builtin_clz(e[e[0]]);
    lsh_radix(x, 256, temp);
    mp_div(temp, m, temp2, x_);
    mp_div(R, m, temp2, a);

    for (i = e[0]; i >= 1; i--) {
        for (j = t - 1; j >= 0; j--) {
            dh_mon_pro(a, a, m, m_prime, temp);
            mp_copy(temp, a);
            count += 1;
            if (e[i] & (1u << (uint32_t)j)) {
                dh_mon_pro(a, x_, m, m_prime, temp);
                mp_copy(temp, a);
            }
        }
        t = 16;
    }
    dh_mon_pro(a, one, m, m_prime, temp);
    mp_copy(temp, a);
    remove_leading_zeros(m);
    remove_leading_zeros(m_prime);
    remove_leading_zeros(x);
    remove_leading_zeros(e);
}

/*
 * Montgomery multiplication
 * Algorithm 14.36 - Handbook of applied cryptography
 */
uint32_t mont_mult(uint16_t *x, uint16_t *y, uint16_t *m, uint16_t *m_prime, uint16_t *a) {
    uint16_t i;
    uint16_t ui;
    uint16_t temp[MAX_SIZE];
    uint16_t temp2[MAX_SIZE];
    uint16_t temp3[MAX_SIZE];
    if (!is_gteq(m, x) || !is_gteq(m, y))
        return FAILURE;
    if (m[0] != x[0] || m[0] != y[0])
        return FAILURE;
    for (i = 0; i <= x[0]; i++) {
        a[i + 1] = 0;
    }
    a[0] = x[0] + ONE;
    for (i = 0; i < x[0]; i++) {
        ui = a[1] + ((uint16_t) (x[i + 1]) * y[1]);
        ui = (uint16_t) (ui * m_prime[1]);
        mp_mult_scalar(y, x[i + 1], temp);
        mp_mult_scalar(m, ui, temp2);
        mp_add(temp, temp2, temp3);
        mp_add(temp3, a, temp);
        rsh_radix(temp, 1, a);
    }
    if (is_gteq(a, m)) {
        mp_sub(a, m, temp);
        mp_copy(temp, a);
    }
    return SUCCESS;
}

/*
 * Montgomery exponentiation
 * Algorithm 14.94 - Handbook of applied cryptography
 *
 * Modified to allow base with less than digits than modulo
 */
uint32_t mont_exp(uint16_t *x, uint16_t *e, uint16_t *m, uint16_t *m_prime, uint16_t *R_mod_m, uint16_t *R,
                  uint16_t *a) {
    uint16_t x_[MAX_SIZE];
    uint16_t temp[MAX_SIZE];
    uint16_t temp2[MAX_SIZE];
    uint16_t one[MAX_SIZE] = {1, 1};
    int32_t t, i, j;
    if (!is_gteq(m, x))
        return FAILURE;

    mp_copy(x, a);
    t = 32 - __builtin_clz(e[e[0]]);

    // Not use montgomery multiplication for this step
    // mont_mult(a, R_square_mod_m, m, m_prime, x_);
    mp_mult(x, R, temp);
    mp_div(temp, m, temp2, x_);
    mp_copy(R_mod_m, a);
    for (i = e[0]; i >= 1; i--) {
        for (j = t - 1; j >= 0; j--) {
            mont_mult(a, a, m, m_prime, temp);
            mp_copy(temp, a);
            if (e[i] & (1u << (uint32_t)j)) {
                mont_mult(a, x_, m, m_prime, temp);
                mp_copy(temp, a);
            }
        }
        t = 16;
    }
    mont_mult(a, one, m, m_prime, temp);
    mp_copy(temp, a);
    return SUCCESS;
}

uint32_t is_gteq(uint16_t *x, uint16_t *y) {
    uint16_t i;
    uint16_t larger = x[0] > y[0] ? x[0] : y[0];
    uint16_t x_size = x[0];
    uint16_t y_size = y[0];

    if (x[0] > y[0])
        sign_extend(y, x[0] - y[0]);
    else if (x[0] < y[0])
        sign_extend(x, y[0] - x[0]);

    for (i = larger; i > 0; i--) {
        if (x[i] > y[i])
            return TRUE;
        if (x[i] < y[i])
            return FALSE;
    }
    x[0] = x_size;
    y[0] = y_size;
    /* equal */
    return TRUE;
}

uint32_t is_gteq_u(uint16_t *x, uint16_t *y) {
    uint16_t i;
    uint16_t larger = x[0] > y[0] ? x[0] : y[0];
    uint16_t x_size = x[0];
    uint16_t y_size = y[0];

    if (x[0] > y[0])
        zero_pad(y, x[0] - y[0]);
    else if (x[0] < y[0])
        zero_pad(x, y[0] - x[0]);

    for (i = larger; i > 0; i--) {
        if (x[i] > y[i])
            return TRUE;
        if (x[i] < y[i])
            return FALSE;
    }
    x[0] = x_size;
    y[0] = y_size;
    /* equal */
    return TRUE;
}

uint32_t is_gt(uint16_t *x, uint16_t *y) {
    uint16_t i;
    uint16_t larger = x[0] > y[0] ? x[0] : y[0];
    uint16_t x_size = x[0];
    uint16_t y_size = y[0];

    if (x[0] > y[0])
        sign_extend(y, x[0] - y[0]);
    else if (x[0] < y[0])
        sign_extend(x, y[0] - x[0]);

    for (i = larger; i > 0; i--) {
        if (x[i] > y[i])
            return TRUE;
        if (x[i] < y[i])
            return FALSE;
    }
    x[0] = x_size;
    y[0] = y_size;
    /* equal */
    return FALSE;
}

uint32_t is_equal(uint16_t *x, uint16_t *y) {
    uint16_t i;
    uint16_t x_size = x[0];
    uint16_t y_size = y[0];
    remove_leading_zeros(x);
    remove_leading_zeros(y);
    uint16_t is_equal = TRUE;
    if (x[0] != y[0]) {
        is_equal = FALSE;
    } else {
        for (i = 1; i <= x[0]; i++) {
            if (x[i] != y[i]) {
                is_equal = FALSE;
                break;
            }
        }
    }
    x[0] = x_size;
    y[0] = y_size;
    return is_equal;
}

uint32_t is_zero(uint16_t *x) {
    uint16_t i;
    if (x[0] == 0)
        return TRUE;
    for (i = 1; i <= x[0]; i++)
        if (x[i] != 0)
            return FALSE;
    return TRUE;
}

uint32_t is_one(uint16_t *x) {
    remove_leading_zeros(x);
    if (x[0] == 1 && x[1] == 1)
        return TRUE;
    return FALSE;
}