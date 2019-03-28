import sympy
import random
import math
from mpmath import mp

a_ = 0x67e7c070a5407fe4a5032d6037674867
b_ = 0x4c4ce5662fb9b0c7abbe2293495278f9
p = 0x16b857b4865fd97ab33df583ba01da84
q = 0x213edef59055df4ca8f98a925c6bcbfa
r = 0x43e37ca71212bd13b86c6b1bfe13d639

def print_c_format(large_int):
    len_i = 1
    x = large_int
    while x >= 2**16:
        x //= 2**16
        len_i += 1
    print("{{{0}, ".format(hex(len_i)), end='')
    for i in range(len_i - 1):
        print("{0}, ".format(hex(large_int % (2**16))), end='')
        large_int //= 2**16
    print("{0}}};".format(hex(large_int)))


def mont_mult(x, y, m, m_prime):
    assert(0 <= x < m and 0 <= y < m)
    b = (2 ** 16)
    x_parts = []
    x_ = x
    m_ = m
    A = 0
    y0 = y % b
    while m_ > 0:
        x_parts.append(x_ % b)
        m_ //= b
        x_ //= b
    # print("A")
    for xi in x_parts:
        a0 = A % b
        # ui = ((a0 + xi * y0)*m_prime) % b
        ui = (a0 + xi * y0) % b
        ui *= (m_prime % b)
        ui = ui % b
        temp = xi * y
        temp2 = ui * m
        A = (A + temp + temp2) // b
        # print_c_format(A)
    # print()
    # print()
    if A >= m:
        A -= m
    return A


def mp_mult(x, y, limit=4096, lsb=True):
    b = 2 ** 16
    x_parts = []
    y_parts = []
    x_ = x
    y_ = y
    if y < 0:
        y_size = int(math.ceil(math.log(-y, 2 ** 16)))
        y_ = y + (2**16)**y_size
    while x_ > 0:
        x_parts.append(x_ % b)
        x_ //= b
    while y_ > 0:
        y_parts.append(y_ % b)
        y_ //= b

    w = [0] * (len(x_parts) + len(y_parts) + 1)
    uv = 0
    for i in range(len(y_parts)):
        c = 0
        for j in range(len(x_parts)):
            if (lsb and i + j >= limit) or (not lsb and i + j < limit):
                break
            uv = w[i + j] + x_parts[j] * y_parts[i] + c
            w[i + j] = uv % b
            c = uv // b
        if i + len(x_parts) > limit:
            continue
        w[i + len(x_parts)] = uv // b
    w_ = 0
    for i in range(limit if not lsb else len(w)):
        w_ += w.pop()
        if len(w) > 0:
            w_ *= b
    return w_


def monpro(x, y, m, mprime):
    t = x*y
    #t = mp_mult(x, y)
    #m_ = (t * mprime) % (2 ** 4096)
    m_ = mp_mult(t,mprime, limit=256)

    u = (t + (m_ * m)) // (2**4096)

    if u >= m:
        return u - m
    else:
        return u

count = 0
def mont_exp(m, R, mprime, e, x):
    global count
    assert (1 <= x < m)
    bits = [int(c) for c in bin(e)[2:]]
    x_ = (x << 4096) % m
    A = (2 ** 4096) % m

    for bit in bits:
        # print_c_format(A)
        A = monpro(A, A, m, mprime)
        # A = mont_mult(A, A, m, mprime)
        # print_c_format(A)
        count += 1
        if bit == 1:
            A = monpro(A, x_, m, mprime)
            # A = mont_mult(A, x_, m, mprime)


    A = monpro(A, 1, m, mprime)
    return A


def mp_div(x, y):
    b = 2 ** 16
    x_parts = []
    y_parts = []
    x_ = x
    y_ = y
    while x_ > 0:
        x_parts.append(x_ % b)
        x_ //= b
    while y_ > 0:
        y_parts.append(y_ % b)
        y_ //= b
    n = len(x_parts) - 1
    t = len(y_parts) - 1
    q = [0] * (n - t + 1)
    ybnt = y*(b ** (n-t))
    while x >= ybnt:
        q[n - t] += 1
        x -= ybnt

    x_parts = []
    y_parts = []
    x_ = x
    y_ = y
    while x_ > 0:
        x_parts.append(x_ % b)
        x_ //= b
    while y_ > 0:
        y_parts.append(y_ % b)
        y_ //= b

    for i in range(n, t, -1):
        if x_parts[i] == y_parts[t]:
            q[i - t - 1] = b - 1
        else:
            q[i - t - 1] = ((x_parts[i]*b + x_parts[i-1])//y_parts[t]) % b
        p1 = q[i - t - 1] * (y_parts[t]*b + y_parts[t-1])
        p2 = x_parts[i] * (b**2) + x_parts[i - 1] * b + x_parts[i - 2]
        while p1 > p2:
            q[i - t - 1] -= 1
            p1 = q[i - t - 1] * (y_parts[t] * b + y_parts[t - 1])
            p2 = x_parts[i] * (b ** 2) + x_parts[i - 1] * b + x_parts[i - 2]

        x = 0
        for j in range(len(x_parts)):
            x += x_parts.pop()
            if len(x_parts) > 0:
                x *= b
        y = 0
        for j in range(len(y_parts)):
            y += y_parts.pop()
            if len(y_parts) > 0:
                y *= b

        x -= q[i - t - 1] * y * (b ** (i - t - 1))
        if x < 0:
            x += y * (b ** (i - t - 1))
            q[i - t - 1] -= 1

        x_parts = []
        y_parts = []
        x_ = x
        y_ = y
        while x_ > 0:
            x_parts.append(x_ % b)
            x_ //= b
        while y_ > 0:
            y_parts.append(y_ % b)
            y_ //= b

    q_ = 0
    for i in range(len(q)):
        q_ += q.pop()
        if len(q) > 0:
            q_ *= b
    return q_, x



def barrett_reduce(x, m, mew):
    k = 1
    x_ = m
    while x_ > 2 ** 16:
        x_ //= 2 ** 16
        k += 1

    q1 = x >> (16 * (k - 1))
    q2 = q1 * mew
    q3 = q2 >> (16 * (k + 1))

    r1 = x % ((2**16)**(k+1))
    r2 = (q3 * m) % ((2**16)**(k+1))
    r = r1 - r2
    if r < 0:
        r += ((2**16)**(k+1))
    while r >= m:
        r -= m
    return r


def large_random(words):
    return random.randint(0, (2**16)**words - 1)


def ext_binary_gcd(a,b):
    """Extended binary GCD.
    Given input a, b the function returns d, s, t
    such that gcd(a,b) = d = as + bt."""
    u, v, s, t, r = 1, 0, 0, 1, 0
    while (a % 2 == 0) and (b % 2 == 0):
        a, b, r = a//2, b//2, r+1
    alpha, beta = a, b
    # from here on we maintain a = u * alpha + v * beta
    # and b = s * alpha + t * beta
    #
    while a % 2 == 0:
        a = a//2
        if (u % 2 == 0) and (v % 2 == 0):
            u, v = u//2, v//2
        else:
            u, v = (u + beta)//2, (v - alpha)//2
    while a != b:
        if b % 2 == 0:
            b = b//2
            #
            # Commentary: note that here, since b is even,
            # (i) if s, t are both odd then so are alpha, beta
            # (ii) if s is odd and t even then alpha must be even, so beta is odd
            # (iii) if t is odd and s even then beta must be even, so alpha is odd
            # so for each of (i), (ii) and (iii) s + beta and t - alpha are even
            #
            if (s % 2 == 0) and (t % 2 == 0):
                s, t = s//2, t//2
            else:
                s, t = (s + beta)//2, (t - alpha)//2
        elif a >= b:
            a, b, u, v, s, t = b, a, s, t, u, v
        else:
            b, s, t = b - a, s - u, t - v
    return (2 ** r) * a, s, t


def mulinv(a, b):
    """return x such that (x * a) % b == 1"""
    g, x, _ = ext_binary_gcd(a, b)
    if g == 1:
        return x % b
    return x


def main():
    mp.dps = 10000
    prime = 2**4096 - 2**4032 - 1 + 2**64 * (int(2**3966 * mp.pi) + 240904)

    n = 8
    R = (2 ** 16) ** 256
    rprime = R + (~mulinv(prime, R) + 1)
    # print_c_format(R)
    #rprime = 1
    large_rand = 0x7e889eed8dfc8ebfe4360f1b44b1701ce3a7080b213bb1fb955e0123605ff381
    # print_c_format(large_rand)
    # print_c_format(rprime)
    # print_c_format(prime)
    # print_c_format(R)
    # print_c_format(R % prime)
    # print_c_format(pow(2, large_rand, prime))
    # print_c_format(rprime)
    # y_size = int(math.ceil(math.log(-rprime, 2 ** 16)))
    # y_ = rprime + (2 ** 16) ** y_size
    # print_c_format(y_)
    print_c_format(mont_exp(prime, R, rprime, large_rand, 2))
    # r = large_random(n)
    # gcd_, _, _ = ext_binary_gcd(r, 2 ** 16)
    # while gcd_ != 1:
    #     r = large_random(n)
    #     gcd_, _, _ = ext_binary_gcd(r, 2 ** 16)
    # p = large_random(n)
    # q = large_random(n)
    # while p >= r:
    #     p = large_random(n)
    # while q >= r:
    #     q = large_random(n)
    # mew = ((2 ** 16) ** n) // q
    # R = (2**16) ** n
    # r2modm = (R*R) % r
    # print_c_format((R*R) // r)
    # quotient, rem = mp_div(R*R, r)
    # print_c_format(quotient)
    # rprime = -mulinv(r, 2**16)
    #
    # print_c_format(pow(p, q, r))
    # print_c_format(mont_exp(r, R, rprime, q, p))
    # print_c_format(p)
    # print_c_format(q)
    # print_c_format(r)
    # print_c_format(rprime)
    # print_c_format(r2modm)
    # print_c_format(p * p)
    # print_c_format(p % q)
    # mont = mont_mult(p, q, r, rprime)
    # res = mont_mult(r2modm, mont, r, rprime)
    # print_c_format(res)
    # print_c_format((p*q) % r)
    #
    # m = large_random(8)
    # gcd_, _, _ = ext_binary_gcd(m, 2**16)
    # while gcd_ != 1:
    #     large_int = large_random(8)
    #     gcd_, _, _ = ext_binary_gcd(m, 2 ** 16)
    #
    # x = large_random(8)
    # y = large_random(8)
    #
    # m_prime = -sympy.mod_inverse(m, 2**16)
    # m_prime_ = -mulinv(m, 2**16)
    # assert(m_prime == m_prime_)
    #
    # print_c_format(m)

    # a_inv = sympy.mod_inverse(a_, b_)
    # b_ = mulinv(a_, b_)
    # print(hex(b_+16**40))
    # print(hex(a_inv))

main()