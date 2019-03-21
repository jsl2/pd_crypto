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

uint32_t check_zero(uint16_t* x) {
  uint16_t i;  
  if (x[0] == 0)
    return SUCCESS;
  for (i = 1; i <= x[0]; i++) 
    if (x[i] != 0)
      return FAILURE;
  return SUCCESS;
}

uint32_t sign_extend(uint16_t* x, uint16_t zeros) {
  uint16_t i;
  uint16_t neg;
  neg = (x[x[0]] & 0x8000);
  for (i = x[0] + 1; i <= x[0] + zeros; i++) {
    x[i] = neg ? 0xffff : 0;
  }
  x[0] += zeros;
  return SUCCESS;
}

uint32_t mp_add(uint16_t* x, uint16_t* y, uint16_t* w) {
  uint16_t c = 0;
  uint16_t i = 0;
  uint16_t n = 0;
  uint16_t sign;
  if (x[0] != y[0]) {
    if (x[0] > y[0])
      sign_extend(y, x[0] - y[0]);
    else
      sign_extend(x, y[0] - x[0]);
  }  
  sign = (x[x[0]] & 0x8000) || (y[y[0]] & 0x8000);
  n = x[0];  
  for(i = 0; i < n; i++) {
    w[i + 1] = x[i + 1] + y[i + 1] + c;
    c = w[i + 1] < x[i + 1];
  }
  if (c > 0 && !sign) {
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
  uint16_t n;
  /*if (x[0] != y[0])
    return FAILURE;*/ 
  if (x[0] != y[0]) {
    if (x[0] > y[0]) {
      sign_extend(y, x[0] - y[0]);
    } else {
      sign_extend(x, y[0] - x[0]);
    }
  }
  n = x[0];
  if (check_equal(x, y)) {
    w[0] = 0;
    w[1] = 0;
    return SUCCESS;
  }
  for (i = 0; i < n; i++) {
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
  for (i = 0; i < x[0] + y[0]; i++)
    w[i + 1] = 0;
  for (i = 0; i < y[0]; i++) {
    c = 0;
    for (j = 0; j < x[0]; j++) {
      uv = w[i + j + 1] + ((uint32_t)x[j + 1])*((uint32_t)y[i + 1]) + c;
      w[i + j + 1] = uv;
      c = (uv >> 16);
    }
    w[i + x[0] + 1] = (uv >> 16);
  }
  if (uv >> 16) {      
      w[0] = x[0] + y[0];
    } else {
      w[0] = x[0] + y[0] - 1;
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
  for (j = 0; j < x[0]; j++) {
    uv = ((uint32_t)x[j + 1])*((uint32_t)y) + c;
    w[j + 1] = uv;
    c = (uv >> 16);
  }  
  if (c) {    
    w[x[0] + 1] = c;
    w[0] = x[0] + 1;
  } else {
    w[0] = x[0];
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
  uint16_t* T[256] = { 0 };  
  uint16_t temp[256] = {0};
  uint16_t temp2[256] = {0};
  
  int32_t A, B, C, D;
  uint16_t x_, y_, i;
  int32_t q, q_, t;
  if (!check_gteq(x, y))
    return FAILURE;
  for (i = y[0]+1; i <= x[0]; i++)
    y[i] = 0;
    
  while (y[0] > 1) {
    x_ = x[x[0]];
    y_ = y[x[0]];
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
      mp_div(x, y, temp, T);
      mp_copy(y, x);
      mp_copy(T, y);
    } else {
      mp_mult_scalar(x, A, temp);
      mp_mult_scalar(y, B, temp2);
      mp_add(temp, temp2, T);
      
      mp_mult_scalar(x, C, temp);
      mp_mult_scalar(y, D, temp2);
            
      mp_copy(T, x);
      mp_add(temp, temp2, y);      
    }    
  }
  // v = gcd(x, y)
    return v;
  return SUCCESS;
}*/

uint32_t mp_diveq2(uint16_t* x) {
  uint16_t i;  
  uint16_t sign = x[x[0]] & 0x8000;
  for (i = 1; i <= x[0]; i++) {    
    x[i] >>= 1;
    if ((i < x[0] && x[i + 1] & 0x1))
      x[i] |= 0x8000;    
  }
  if (sign)
    x[x[0]] |= 0x8000;
  return SUCCESS;
}

uint32_t mp_muleq2(uint16_t* x) {
  uint16_t i, t;
  uint16_t c = 0;
  for (i = 1; i <= x[0]; i++) {
    t = x[i] & 0x8000;
    x[i] <<= 1;
    if (c)
      x[i] |= 1;
    c = t;
  }
  if (c) {
    x[0]++;
    x[x[0]] = 1;
  }
  return SUCCESS;
}


uint32_t binary_extended_gcd(uint16_t* x, uint16_t* y, uint16_t* a, uint16_t* b, uint16_t* gcd) {
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
  
  g[0] = 1; g[1] = 1;
  while (!(x_[1] & 0x0001) && !(y_[1] & 0x0001)) {
    mp_diveq2(x_);
    mp_diveq2(y_);    
    mp_muleq2(g);    
  }
  mp_copy(x_, u);
  mp_copy(y_, v);
  A[0] = 1; B[0] = 1; C[0] = 1; D[0] = 1;
  A[1] = 1; B[1] = 0; C[1] = 0; D[1] = 1;    
  do {        
    while (!(u[1] & 0x0001)) {      
      mp_diveq2(u);      
      if (!(A[1] & 0x0001) && !(B[1] & 0x0001)) {
	mp_diveq2(A);
	mp_diveq2(B);
      } else {
	mp_add(A, y, temp);
	mp_copy(temp, A);
	mp_diveq2(A);
	mp_sub(B, x, temp);
	mp_copy(temp, B);
	mp_diveq2(B);	
      }
    }
    while (!(v[1] & 0x0001)) {
      mp_diveq2(v);
      if (!(C[1] & 0x0001) && !(D[1] & 0x0001)) {
	mp_diveq2(C);
	mp_diveq2(D);
      } else {
	mp_add(C, y, temp);
	mp_copy(temp, C);
	mp_diveq2(C);
	mp_sub(D, x, temp);
	mp_copy(temp, D);
	mp_diveq2(D);	
      }      
    }
    if (check_gteq(u, v)) {      
      mp_sub(u, v, temp);      
      mp_copy(temp, u);      
      mp_sub(A, C, temp);
      mp_copy(temp, A);
      mp_sub(B, D, temp);
      mp_copy(temp, B);
    } else {
      mp_sub(v, u, temp);
      mp_copy(temp, v);
      mp_sub(C, A, temp);
      mp_copy(temp, C);
      mp_sub(D, B, temp);
      mp_copy(temp, D);
    }    
  } while (!check_zero(u));
  
  mp_copy(C, a);
  mp_copy(D, b);
  mp_mult(g, v, temp);  
  mp_copy(temp, gcd); 
  remove_leading_zeros(gcd);
  return SUCCESS;
}

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
  uint16_t temp[MAX_SIZE] = {0};
  uint16_t temp2[MAX_SIZE] = {0};
  uint16_t temp3[MAX_SIZE] = {0};
  
  mp_copy(x, r);
  if (n < t || t == 0)
    return FAILURE;  
  for (j = 0; j <= n - t; j++)
    q[j + 1] = 0;
  q[0] = n - t + 1;
  lsh_radix(y, n-t, temp2);  
  while (check_gteq(r, temp2)) {    
    q[n - t + 1] += 1;
    mp_sub(r, temp2, temp);
    mp_copy(temp, r);    
  } 
  
  for (i = n; i > t; i--) {
    if (r[i + 1] == y[t + 1])
      q[i - t] = 0xFFFF;    
    else
      q[i - t] = (((uint32_t)r[i + 1] << 16) + r[i]) / y[t + 1];
    while ((uint64_t)q[i - t] * (((uint64_t)(y[t + 1]) << 16) + y[t]) > ((uint64_t)x[i + 1] << 32) + ((uint64_t)x[i] << 16) + ((uint64_t)x[i - 1]))
      q[i - t] -= 1;    
    lsh_radix(y, i - t - 1, temp2);
    mp_mult_scalar(temp2, q[i - t], temp);
    mp_sub(r, temp, temp3);
    
    if (check_gt(temp, r)) {
      mp_add(temp3, temp2, r);
      q[i - t] -= 1;      
    } else {
      mp_copy(temp3, r);
    }
  }
    
  remove_leading_zeros(r);
  return SUCCESS;
}

uint32_t check_gteq(uint16_t* x, uint16_t* y) {
  uint16_t i;
  uint16_t larger = x[0] > y[0] ? x[0] : y[0];
  /*if (x[0] > y[0])
    return SUCCESS;
  if (x[0] < y[0])
    return FAILURE;*/
  for (i = larger; i > 0; i--) {
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