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

uint32_t remove_leading_zeros(uint16_t* x) {
  uint16_t i;
  for (i = x[0]; x[i] == 0; i--)
    x[0]--;
  return SUCCESS;
}

uint32_t zero_pad(uint16_t* x, uint16_t zeros) {
  uint16_t i;
  for (i = x[0] + 1; i <= x[0] + zeros; i++)
    x[i] = 0;
  x[0] += zeros;
  return SUCCESS;
}

uint32_t mp_add(uint16_t* x, uint16_t* y, uint16_t* w) {
  uint16_t c = 0;
  uint16_t i = 0;
  uint16_t n = 0;
  if (x[0] != y[0]) {
    if (x[0] > y[0])
      zero_pad(y, x[0] - y[0]);
    else
      zero_pad(x, y[0] - x[0]);
  }

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
  /*if (x[0] != y[0])
    return FAILURE;*/
  if (x[0] != y[0]) {
    if (x[0] > y[0]) {
      zero_pad(y, x[0] - y[0]);
    } else {
      zero_pad(x, y[0] - x[0]);
    }
  }
  for (i = 0; i < n; i++) {
    w[i + 1] = x[i + 1] - y[i + 1] - c;
    c = y[i + 1] > x[i + 1];
  }
  if (w[x[0]] == 0)
    w[0] = x[0] - 1;
  else
    w[0] = x[0];
  return SUCCESS;
}

uint32_t mp_mult(uint16_t* x, uint16_t* y, uint16_t* w) {
  uint16_t i, j;  
  uint16_t c;
  uint32_t uv;
  for (i = 0; i <= x[0] + y[0] + 1; i++)
    w[i + 1] = 0;
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

uint32_t mp_mult_scalar(uint16_t* x, uint16_t y, uint16_t* w) {
  uint16_t i, j;  
  uint16_t c;
  uint32_t uv;
  for (i = 0; i <= x[0] + 2; i++)
    w[i + 1] = 0;

  c = 0;
  for (j = 0; j <= x[0]; j++) {
    uv = w[j + 1] + ((uint32_t)x[j + 1])*((uint32_t)y) + c;
    w[j + 1] = uv;
    c = (uv >> 16);
  }  
  if (c) {    
    w[x[0] + 2] = c;
    w[0] = x[0] + 2;
  } else {
    w[0] = x[0] + 1;
  }
    
  return SUCCESS;
}

uint32_t mp_copy(uint16_t* src, uint16_t* dest) {
  uint16_t i;
  for (i = 0; i <= src[0]; i++)
    dest[i] = src[i];
  return SUCCESS;
}

/*uint32_t lehmer_gcd(uint16_t* x, uint16_t* y) {
  uint16_t* T[y[0]];
  int32_t A, B, C, D;
  uint16_t x_, y_;
  int32_t q, q_, t;
  if (!check_gteq(x, y))
    return FAILURE;
  while (y[0] > 1) {
    x_ = x[x[0]];
    y_ = y[y[0]];
    A = 1; B = 0; C = 0; D = 1;
    while (y_ + C != 0 && y_ + D != 0) {
      q = (x_ + A)/(y_ + C);
      q_ = (x_ + B)/(y_ + D);
      t = A - q*C;
      A = C;
      C = t;
      t = B - q*D;
      B = D;
      D = t;
      t = x_ - q*y_;
      x_ = y_;
      y_ = t;
    }
    if (B == 0) {
      //T = x mod y
      mp_copy(y, x);
      mp_copy(T, y);
    } else {
      //T = Ax + By
      //u = Cx + Dy
      //x = T
      //y = u
    }
    // v = gcd(x, y)
    return v;
  }
  return SUCCESS;
}*/

uint32_t lsh_radix(uint16_t* x, uint16_t sh, uint16_t* w) {
  uint16_t i;  
  for (i = 1; i <= sh + 1; i++)
    w[i] = 0;
  for (i = sh + 1; i <= x[0] + sh + 1; i++)
    w[i] = x[i - sh];
  w[0] = x[0] + sh;
  return SUCCESS;
}

uint32_t mp_div(uint16_t* x, uint16_t* y, uint16_t* q, uint16_t* r) {
  uint16_t i, j;
  uint16_t n = x[0] - 1;
  uint16_t t = y[0] - 1;
  uint16_t temp[32] = {0};
  uint16_t temp2[32] = {0};
  uint16_t temp3[32] = {0};
  if (n < t || t == 0)
    return FAILURE;  
  for (j = 0; j <= n - t; j++)
    q[j + 1] = 0;
  q[0] = n - t + 1;
  lsh_radix(y, n-t, temp2);
  
  while (check_gteq(x, temp2)) {    
    q[n - t + 1] += 1;
    mp_sub(x, temp2, temp);    
    mp_copy(temp, x);        
  }
  print_radix(x);
  for (i = n; i > t; i--) {
    if (x[i + 1] == y[t + 1])
      q[i - t] = 0xFFFF;    
    else
      q[i - t] = (((uint32_t)x[i + 1] << 16) + x[i]) / y[t + 1];
    while ((uint64_t)q[i - t] * (((uint64_t)(y[t + 1]) << 16) + y[t]) > ((uint64_t)x[i + 1] << 32) + ((uint64_t)x[i] << 16) + ((uint64_t)x[i - 1]))
      q[i - t] -= 1;    
    lsh_radix(y, i - t - 1, temp2);
   
    mp_mult_scalar(temp2, q[i - t], temp);
    mp_sub(x, temp, temp3);

    if (check_gt(temp, x)) {
      print_radix(temp3);
      mp_add(temp3, temp2, x);
      q[i - t] = q[i - t] - 1;
      print_radix(x);
    }
  }
  
  mp_copy(x, r);
  remove_leading_zeros(r);
  return SUCCESS;
}

uint32_t check_gteq(uint16_t* x, uint16_t* y) {
  uint16_t i;
  if (x[0] > y[0])
    return SUCCESS;
  if (x[0] < y[0])
    return FAILURE;
  for (i = x[0]; i > 0; i--) {
    if (x[i] > y[i])
      return SUCCESS;
    if (x[i] < y[i])
      return FAILURE;
  }
  /* equal */
  return SUCCESS;
}

uint32_t check_gt(uint16_t* x, uint16_t* y) {
  uint16_t i;
  if (x[0] > y[0])
    return SUCCESS;
  if (x[0] < y[0])
    return FAILURE;
  for (i = x[0]; i > 0; i--) {
    if (x[i] > y[i])
      return SUCCESS;
    if (x[i] < y[i])
      return FAILURE;
  }
  /* equal */
  return FAILURE;
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