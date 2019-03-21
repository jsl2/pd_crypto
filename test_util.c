#include <stdint.h>
#include <stdio.h>
#include "util.h"

#define RUN_TEST(test, x, y) {if (check_equal(x,y) == SUCCESS) { printf(test); printf(" passed!\n"); } else { printf(test); printf(" failed!\n");} }

uint16_t a[] = {0x8, 0x4867, 0x3767, 0x2d60, 0xa503, 0x7fe4, 0xa540, 0xc070, 0x67e7};
uint16_t b[] = {0x8, 0x78f9, 0x4952, 0x2293, 0xabbe, 0xb0c7, 0x2fb9, 0xe566, 0x4c4c};
/* e = gcd(a,b) */
uint16_t e[] = {0x1, 0x0001};
/* f = a^{-1} mod b */
uint16_t f[] = {0x8, 0x778c, 0x6c90, 0xa80c, 0xe431, 0x4674, 0x1b2c, 0x02a4, 0x3e99};
uint16_t m[] = {0x7, 0x78fa, 0xe4d2, 0x7165, 0x9ded, 0x9b72, 0x10f1, 0x1d56};
/* g = a / m  */
uint16_t g[] = {0x2, 0x8ab8, 0x0003};
/* h = a mod b */
uint16_t h[] = {0x8, 0xcf6e, 0xee14, 0xacc, 0xf945, 0xcf1c, 0x7586, 0xdb0a, 0x1b9a};
/* i = a * b */
uint16_t i[] = {0x10, 0xb42f, 0xdd63, 0xb06d, 0x0758, 0x5f64, 0x8fbe, 0x1be2, 0x03a2,
                      0x7400, 0x74d7, 0xb0f8, 0x81df, 0x9061, 0x5b5a, 0x030a, 0x1ef8};
/* j = a + b */
uint16_t j[] = {0x8, 0xc160, 0x80b9, 0x4ff3, 0x50c1, 0x30ac, 0xd4fa, 0xa5d6, 0xb434};
/* k = a - b */
uint16_t k[] = {0x8, 0xcf6e, 0xee14, 0x0acc, 0xf945, 0xcf1c, 0x7586, 0xdb0a, 0x1b9a};
uint16_t s = 0x2b73;
/* l = s * a */
uint16_t l[] = {0x9, 0xd345, 0x3c8e, 0x8b87, 0xa90c, 0xdb6d, 0x1174, 0x5e5c, 0x9a6e, 0x11a2};

uint16_t u[] = {0x1, 383};
uint16_t v[] = {0x1, 271};
uint16_t test_array[MAX_SIZE] = {0};
uint16_t test_array2[MAX_SIZE] = {0};
uint16_t test_array3[MAX_SIZE] = {0};

int main(void) {
  mp_add(a, b, test_array);    
  RUN_TEST("Addition test a + b", test_array, j);  
  
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
  
  /*binary_extended_gcd(b, a, test_array, test_array2, test_array3);
  RUN_TEST("GCD test: gcd(a,b)", test_array3, e);
  */
  binary_extended_gcd(u, v, test_array, test_array2, test_array3);
  print_radix(test_array);
  print_radix(test_array2);
  /*RUN_TEST("Modular inverse a^{-1} mod b", test_array, f);*/
  return 0;
}