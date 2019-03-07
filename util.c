#include <stdint.h>
#include <stdio.h>
#include "util.h"

void print_radix(uint16_t* x) {
  uint16_t i = 0;
  printf("{%#x, ", x[0]);
  for(i = 1; i < x[0]; i++) {
    printf("%#06x, ", x[i]);
  }
  printf("%#06x}\n", x[x[0]]);
}

uint32_t mp_add(uint16_t* x, uint16_t* y, uint16_t* w) {
  uint16_t c = 0;
  uint16_t i = 0;
  uint16_t n = 0;
  if (x[0] != y[0])
    return FAILURE; /* do both have n base b digits? -- could also pad here */
  n = x[0];  
  for(i = 0; i < n; i++) {
    w[i + 1] = x[i + 1] + y[i + 1] + c;
    c = w[i + 1] < x[i + 1];
  }
  if (c > 0) {
    w[0] = n + 1; 
    w[n + 1] = c;
  } else {
    w[0] = n;
  }
  return SUCCESS;  
}

uint32_t mp_sub(uint16_t* x, uint16_t* y, uint16_t* w) {  
  uint16_t i = 0;
  uint16_t c = 0;
  uint16_t n = x[0];
  if (x[0] != y[0])
    return FAILURE;
  for(i = 0; i < n; i++) {
    w[i + 1] = x[i + 1] - y[i + 1] - c;
    c = y[i + 1] > x[i + 1];
  }
  w[0] = x[0];
  return SUCCESS;
}

uint32_t mp_mult(uint16_t* x, uint16_t* y, uint16_t* w) {
  uint16_t i, j;  
  uint16_t c;
  uint32_t uv;
  for (i = 0; i <= x[0] + y[0] + 1; i++)
    w[i+1] = 0;
  for (i = 0; i <= y[0]; i++) {
    c = 0;
    for (j = 0; j <= x[0]; j++) {
      uv = w[i + j + 1] + ((uint32_t)x[j + 1])*((uint32_t)y[i + 1]) + c;
      w[i + j + 1] = uv;
      c = (uv >> 16);
    }
    if (uv >> 16) {
      w[i + x[0] + 2] = (uv >> 16);
      w[0] = x[0] + y[0] + 1;
    } else {
      w[0] = x[0] + y[0];
    }
  }    
  return SUCCESS;
}

uint32_t check_equal(uint16_t* x, uint16_t* y) {
  uint16_t i;
  if (x[0] != y[0])
    return FAILURE;
  for (i = 1; i <= x[0]; i++)
    if (x[i] != y[i])
      return FAILURE;
  
  return SUCCESS;
}