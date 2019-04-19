import math
import random
from Crypto.Hash import SHA256
from Crypto.Util.number import getPrime, GCD, inverse
from mpmath import mp


def print_c_format(large_int):
    len_i = 1
    x = large_int
    while x >= 2 ** 16:
        x //= 2 ** 16
        len_i += 1
    print("{{{0}, ".format(hex(len_i)), end='')
    for i in range(len_i - 1):
        word = large_int % (2 ** 16)
        if word == 0:
            print("000000, ", end='')
        else:
            print("{:#06x}, ".format(word), end='')
        large_int //= 2 ** 16
    if large_int == 0:
        print("000000};")
    else:
        print("{:#06x}}};".format(large_int))


def c_format_to_int(c_str):
    numbers = c_str[1:-2]
    numbers = numbers.split(', ')
    size = int(numbers[0], 16)
    res = 0
    for i in range(size - 1):
        res += int(numbers[size - i], 16)
        res <<= 16
    res += int(numbers[1], 16)
    return res


def mont_mult(x, y, m, m_prime):
    assert (0 <= x < m and 0 <= y < m)
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
        y_ = y + (2 ** 16) ** y_size
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
            if i + j + 1 > limit:
                break
            uv = w[i + j] + x_parts[j] * y_parts[i] + c
            w[i + j] = uv % b
            c = uv // b
        if i + len(x_parts) + 1 <= limit:
            w[i + len(x_parts)] = uv // b
    w_ = 0
    for i in range(len(w)):
        w_ += w.pop()
        if len(w) > 0:
            w_ *= b
    return w_


def monpro(x, y, m, mprime):
    t = x * y
    # t = mp_mult(x, y)
    # m_ = (t * mprime) % (2 ** 4096)
    m_ = mp_mult(t, mprime, limit=256)
    u = (t + (m_ * m)) // (2 ** 4096)
    # print_c_format(u)
    if u >= m:
        return u - m
    else:
        return u


count = 0



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
    ybnt = y * (b ** (n - t))
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
            q[i - t - 1] = ((x_parts[i] * b + x_parts[i - 1]) // y_parts[t]) % b
        p1 = q[i - t - 1] * (y_parts[t] * b + y_parts[t - 1])
        p2 = x_parts[i] * (b ** 2) + x_parts[i - 1] * b + x_parts[i - 2]
        while p1 > p2:
            q[i - t - 1] -= 1
            p1 = q[i - t - 1] * (y_parts[t] * b + y_parts[t - 1])
            p2 = x_parts[i] * (b ** 2) + x_parts[i - 1] * b + x_parts[i - 2]
        print(q[i - t - 1])
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


def mont_exp(m, mprime, e, x):
    assert (1 <= x < m)
    bits = [int(c) for c in bin(e)[2:]]

    x_ = (x << 4096) % m
    q_, x2 = mp_div(x<<4096, m)
    print(x<<4096)
    print(m)
    print(x2)
    A = pow(2, 4096, m)
    for bit in bits:
        A = monpro(A, A, m, mprime)
        # A = mont_mult(A, A, m, mprime)
        if bit == 1:
            A = monpro(A, x_, m, mprime)
            # A = mont_mult(A, x_, m, mprime)
    A = monpro(A, 1, m, mprime)
    return A


def barrett_reduce(x, m, mew):
    k = 1
    x_ = m
    while x_ > 2 ** 16:
        x_ //= 2 ** 16
        k += 1

    q1 = x >> (16 * (k - 1))
    q2 = q1 * mew
    q3 = q2 >> (16 * (k + 1))

    r1 = x % ((2 ** 16) ** (k + 1))
    r2 = (q3 * m) % ((2 ** 16) ** (k + 1))
    r = r1 - r2
    if r < 0:
        r += ((2 ** 16) ** (k + 1))
    while r >= m:
        r -= m
    return r


def large_random(words):
    return random.randint(0, (2 ** 16) ** words - 1)


def ext_binary_gcd(a, b):
    """Extended binary GCD.
    Given input a, b the function returns d, s, t
    such that gcd(a,b) = d = as + bt."""
    u, v, s, t, r = 1, 0, 0, 1, 0
    while (a % 2 == 0) and (b % 2 == 0):
        a, b, r = a // 2, b // 2, r + 1
    alpha, beta = a, b
    # from here on we maintain a = u * alpha + v * beta
    # and b = s * alpha + t * beta
    #
    while a % 2 == 0:
        a = a // 2
        if (u % 2 == 0) and (v % 2 == 0):
            u, v = u // 2, v // 2
        else:
            u, v = (u + beta) // 2, (v - alpha) // 2
    while a != b:
        if b % 2 == 0:
            b = b // 2
            #
            # Commentary: note that here, since b is even,
            # (i) if s, t are both odd then so are alpha, beta
            # (ii) if s is odd and t even then alpha must be even, so beta is odd
            # (iii) if t is odd and s even then beta must be even, so alpha is odd
            # so for each of (i), (ii) and (iii) s + beta and t - alpha are even
            #
            if (s % 2 == 0) and (t % 2 == 0):
                s, t = s // 2, t // 2
            else:
                s, t = (s + beta) // 2, (t - alpha) // 2
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


def test_monpro():
    a = "{0x100, 0x7f7e, 0x62ab, 0x313d, 0x9a22, 0xaf69, 0xdb9c, 0x3d08, 0xdf6c, 0xb8b7, 0xbb51, 0x91f6, 0xe0c7, 0xbc14, 0xa799, 0xc6f2, 0x8244, 0x0218, 0x4fdd, 0xa852, 0x094c, 0x5333, 0x87a5, 0xed68, 0x3e4d, 0x140a, 0x5e14, 0x8fbe, 0xba63, 0xa030, 0x4c55, 0xac04, 0x5721, 0x4b22, 0xe85f, 0x7c12, 0x8d2f, 0x07b1, 0xdf44, 0xb008, 0x980d, 0x1365, 0x8c91, 0x5401, 0xf2f8, 0x426f, 0x606f, 0xee44, 0xe376, 0xe7c2, 0x1864, 0xfdc0, 0x95fc, 0xc86b, 0x4114, 0x01b0, 0x55c0, 0xe8ee, 0x2655, 0xc53b, 0x6f60, 0x5c86, 0xd192, 0xbe27, 0x7ac5, 0xb97e, 0x84a2, 0x649c, 0x20fb, 0xbb08, 0xd73d, 0xa570, 0x71f0, 0xe4b8, 0x98da, 0x2336, 0xa439, 0x9d28, 0xf9d1, 0x24d7, 0x60c6, 0x4dcc, 0xb08c, 0x4d79, 0x341b, 0xe5af, 0xe58e, 0xb191, 0x9357, 0x7dfb, 0xd6fc, 0x92b9, 0x1e84, 0x5ce8, 0x6112, 0x6ee1, 0x46e5, 0x6e5d, 0x1652, 0x0af9, 0xa0bd, 0xfb1d, 0x2730, 0x8b72, 0x7701, 0xec2b, 0xe60d, 0x9b11, 0x8132, 0xb038, 0x8dda, 0x12b7, 0x7793, 0x1900, 0x2347, 0xbcd3, 0x237a, 0x3661, 0x9cd7, 0xa167, 0x3950, 0xff35, 0x14bb, 0x12e7, 0x7967, 0x1e01, 0x542c, 0x73fd, 0xc31b, 0xa37a, 0x4033, 0x094b, 0xfde5, 0x834f, 0x8583, 0x2306, 0x2105, 0x3624, 0x18d8, 0x6357, 0xf048, 0x7ee0, 0xeda5, 0x2b65, 0x8289, 0xc1a7, 0x70be, 0xeb24, 0x436c, 0x878e, 0x9983, 0xfbdc, 0xcbab, 0xea37, 0xba9f, 0xde51, 0x43c2, 0xf506, 0xda33, 0xd184, 0x83ea, 0x3acc, 0xdeb0, 0x497f, 0x226e, 0x51b2, 0xa9d1, 0xec24, 0x9fee, 0x7d43, 0xd9a8, 0x09be, 0xc01b, 0x8197, 0x8baf, 0xd2a8, 0x22cd, 0x9f7b, 0x8849, 0xeb45, 0x5d87, 0xbc5f, 0x5ab7, 0xbbe5, 0x9a22, 0xb930, 0x7683, 0x4b25, 0xb913, 0x4f6c, 0x57f6, 0xe6ad, 0x1fff, 0x21d3, 0xe5bb, 0x3f1e, 0x17bd, 0xf8a6, 0x0e56, 0xcd0f, 0x33ef, 0x016e, 0xe6c4, 0x1817, 0xf32a, 0xb62b, 0x7905, 0xb2fa, 0x1a5c, 0xc95b, 0x4f1f, 0x7d57, 0xbe2d, 0x7233, 0xef92, 0x4fc5, 0xe9c0, 0xec89, 0x8274, 0xe68d, 0x4adb, 0x9020, 0x9464, 0xf772, 0xea63, 0x341c, 0xb5c5, 0x3310, 0xf069, 0x4c42, 0x5ed8, 0x66cb, 0xf22a, 0x8e77, 0xb7ab, 0x3f0e, 0xe843, 0x75e9, 0x33f6, 0xc053, 0x75cc, 0x6a20, 0xa707, 0xffd8, 0x59bb, 0x67a4, 0x695c, 0x468c, 0x5924, 0xfef6, 0x5bf4, 0xc086, 0xb5d4, 0x5b1c, 0xd70e, 0xae5a, 0x74c5};"
    n_B = "{0x100, 0x953b, 0xa4c5, 0x7344, 0x586e, 0x2341, 0xa201, 0x3cb2, 0x8261, 0x069e, 0x5a1f, 0x8de7, 0x03e1, 0xe816, 0x743a, 0x26ed, 0xea8d, 0x19b5, 0x8aab, 0xb894, 0x1a4e, 0x826e, 0x0196, 0x0abe, 0xe069, 0xab88, 0x88f0, 0x8be6, 0x4537, 0x5ced, 0x7665, 0x9719, 0x5622, 0x10d5, 0x116b, 0x380c, 0xc4c7, 0x90ae, 0x168f, 0x86c0, 0xf038, 0xbba1, 0x8e3c, 0xac56, 0x02c9, 0x5125, 0xf108, 0xf76b, 0x5389, 0x474d, 0xa04e, 0xbdfa, 0xed73, 0x7130, 0xe965, 0xb65f, 0xed2e, 0x6c71, 0x3952, 0xf3d2, 0xaa5f, 0x994b, 0xb529, 0x3f80, 0x4b9c, 0xd574, 0xe365, 0x0b91, 0xf5c5, 0xb28d, 0xc0cb, 0x8eed, 0xf19b, 0x4944, 0xb02a, 0xb701, 0x1485, 0xd168, 0x4592, 0x0aba, 0xf658, 0xfa61, 0xfd98, 0xbbde, 0x412a, 0xbb62, 0x7607, 0x6be7, 0xe3b5, 0x78bc, 0x3763, 0xb564, 0xabf3, 0x0e14, 0x8511, 0xf3e5, 0x3e0f, 0x549e, 0x4d67, 0x562e, 0x25c6, 0xbd38, 0x390a, 0xf64b, 0xa3f0, 0xb10e, 0x3e7c, 0x7252, 0x29de, 0x264f, 0x2d3e, 0x68b1, 0x1880, 0x6a19, 0x83d9, 0x18c7, 0x8a53, 0x224d, 0x265d, 0x42fe, 0xbccd, 0x10a3, 0x4d16, 0x7841, 0xd620, 0x25cb, 0xe383, 0x9b8c, 0xa063, 0xbc17, 0x2ff0, 0xfe08, 0x4f54, 0x4256, 0xb24b, 0xe6e3, 0x0553, 0x45c5, 0xf5e0, 0x535a, 0xd77a, 0xd752, 0x95db, 0x3374, 0x3cea, 0xa9d0, 0xfc3b, 0x5c19, 0x48f8, 0xdb01, 0xc222, 0xf05a, 0xae9e, 0x1bc9, 0x8ae4, 0x3301, 0x45ba, 0xc6b5, 0x5d3c, 0x19fd, 0xf8f3, 0x205c, 0xb040, 0x0f3e, 0x7286, 0x3099, 0x9bcf, 0xbc4d, 0x0a12, 0x82c8, 0x39e1, 0x2d57, 0xa40a, 0xd9df, 0xb4ae, 0x9caf, 0x5f74, 0xc2df, 0xf728, 0x6947, 0xcec5, 0xca56, 0x62ee, 0xf9da, 0x5cd1, 0x59c2, 0xb4a8, 0x0146, 0xbd2b, 0x01a5, 0x4b72, 0x2bb2, 0x0aac, 0xa18f, 0xb07a, 0x7a87, 0x19f7, 0x4571, 0x101e, 0x945a, 0xcfe8, 0x30aa, 0x3504, 0xddd9, 0xd222, 0x43b0, 0x1694, 0x1be0, 0x53c1, 0xd6e4, 0xad14, 0xfc5a, 0xa5f0, 0x1484, 0xe6a4, 0xecd5, 0x976e, 0xe8fb, 0x5049, 0x26ca, 0xce87, 0xca20, 0xed01, 0x363b, 0xb143, 0x0d0e, 0x5b6b, 0xa535, 0x4a85, 0x5de2, 0xc37c, 0xc359, 0x440f, 0x4347, 0x324b, 0x869a, 0xc904, 0x97cd, 0x817f, 0xe0b5, 0x55f5, 0xbf68, 0xbc66, 0xa772, 0xbe14, 0xea66, 0xf242, 0xeaca, 0x4d88, 0x2077, 0xffc1, 0x1dfa, 0x6e9d, 0x3109, 0xb623, 0xf559, 0x7641};"
    n_B_prime = "{0x100, 0x840d, 0x9ebf, 0x088b, 0x6dae, 0x2858, 0x467d, 0x43ff, 0xf924, 0x89c2, 0x1866, 0xcc0c, 0x1db4, 0x2bb6, 0x01a6, 0x8ee8, 0xa75d, 0xa69e, 0x4ba3, 0x8c2e, 0xe3c9, 0xbf20, 0x5a15, 0x037e, 0x698c, 0xa006, 0x2f84, 0xada6, 0x7667, 0xdcad, 0x2b0c, 0xa491, 0xc8ff, 0x5a8a, 0xa4fa, 0x2b6a, 0xed96, 0xa3ba, 0xdd7d, 0x8352, 0x7e0c, 0x8830, 0x3b00, 0x8ef4, 0xc034, 0xe600, 0xba8c, 0x479f, 0x0877, 0x257a, 0x164d, 0x828a, 0xe8bc, 0xbaef, 0x409f, 0xa519, 0xfda0, 0xc856, 0x0a90, 0xcb2e, 0xb2b8, 0xbe98, 0xa416, 0x51c4, 0x9598, 0x6d41, 0xc13c, 0xa5f0, 0xa74a, 0x97ec, 0x753d, 0xcfb5, 0x1245, 0x3a69, 0xd988, 0x0c31, 0xb9e5, 0x8b4a, 0x6a5d, 0xbe0d, 0xd24d, 0xcce5, 0x1621, 0x0005, 0x0bf5, 0x9814, 0x7286, 0xcfe9, 0x7d91, 0xd8ad, 0x906b, 0xa797, 0xa710, 0x81bc, 0x3ac1, 0xa65b, 0x046e, 0x6915, 0x0d84, 0x2eff, 0x6c72, 0x40cf, 0xf6da, 0x0e35, 0x4965, 0xa3fd, 0x21ed, 0x042e, 0x1d82, 0x2df8, 0x575b, 0x1110, 0x8c18, 0xe020, 0x25bf, 0xdb7e, 0x53bd, 0x083d, 0x1e46, 0xe587, 0x86a6, 0xe864, 0xc6e0, 0xe0a0, 0x00c3, 0x573b, 0x1526, 0xb3b7, 0xe788, 0x8ad7, 0x08f3, 0x4c1d, 0x9c9d, 0x7bdd, 0x0469, 0x5e27, 0xa95c, 0x0217, 0xe107, 0xdd47, 0x5ebe, 0x3fe3, 0xc51f, 0x60e7, 0xbf02, 0xd1bd, 0xc069, 0x6985, 0xb5ca, 0x723c, 0x5844, 0xe237, 0x8ddc, 0x9965, 0x916b, 0xad47, 0xee06, 0x05a6, 0xaaae, 0xfec5, 0x086a, 0x5a3c, 0xe956, 0x4cb4, 0x65a3, 0xa2e2, 0x30dd, 0xd5ca, 0xc3d7, 0x3eb7, 0xdcee, 0x6097, 0x4d91, 0x8ce8, 0xd37c, 0x52fb, 0xf98e, 0x8466, 0xbae9, 0x2e98, 0x83c9, 0xae73, 0x1d6f, 0x1650, 0xfc39, 0xad02, 0x29e2, 0xcb00, 0x30cc, 0xee97, 0xb931, 0x0b10, 0xe149, 0x7e8d, 0x75c6, 0x6866, 0x8bb3, 0x8e20, 0xa526, 0xead1, 0xa9d2, 0x15de, 0xe5d5, 0x0823, 0xdc07, 0xb486, 0x40ce, 0xa37b, 0x8dfb, 0xb0f6, 0xa0c9, 0x5b5a, 0x199f, 0xaee0, 0xd990, 0x8d1a, 0x0c75, 0xeb5f, 0x05b3, 0x9935, 0x9db7, 0x73d1, 0x0b0a, 0x0f20, 0xcb63, 0xd895, 0xe2ca, 0x02c1, 0x54ec, 0x0f74, 0xc058, 0x0529, 0xe075, 0x6247, 0x528f, 0x96f3, 0x65b0, 0xd767, 0x9e10, 0xe581, 0xaf5e, 0xee35, 0x9381, 0xfb59, 0x878c, 0x9142, 0x44d0, 0x1221, 0xbe94, 0xb8f7, 0xb35a, 0x3e79, 0x4d4b, 0x72ee, 0xc2af, 0xbb65, 0x77f8};"
    a = c_format_to_int(a)
    n_B = c_format_to_int(n_B)
    n_B_prime = c_format_to_int(n_B_prime)
    print_c_format(monpro(a, a, n_B, n_B_prime))


def gen_bbs_test():
    # p = getStrongPrime(2048)
    # while p % 4 != 3:
    #     p = getStrongPrime(2048)
    # q = getStrongPrime(2048)
    # while q % 4 != 3:
    #     q = getStrongPrime(2048)
    # n = p * q
    # print(n)
    n = 893087989247984330160661859130556076395507248907886025603738425481091709434340729338068415541131896203350120862439347397048634727244686039395317500526512348576172462802887781298349137731033511885669447438586347835342499757534839685392237955181900258840893452516889713219856858770042230219262439468255498395823503539792763205321262050295314366994685417572258174269943218634851246467819022503650450602476427169873520283899322892398424411243228607872733212613462345371828814821444140118507116759392897948904307863340516652325203970379598801667841449348279481983500117752944569967787188949693144698673378259183068137727614733488087428773743715292084288600618516369429546145677775553376798602240337385397268000293738587005512214265150597402337696313259348394689526331942172753048885621910605252621379224151129282968950309617433208663251860648188303301325344663271110281540412019361176030903606538250417890757956505880366920424534391928034906799673300570403506330316273317391646368119142138722363540689504591831821224275645367983749570455509895867414878799346266544929863257268952430592688935624603109696474823141766050439894160201893337601661164136502666476739553985357113026567711053839401585540994529779720546203088428567031643537466229
    print("uint16_t bbs_n[MAX_SIZE] = ", end='')
    print_c_format(n)

    # seed = random.getrandbits(128)
    # while math.gcd(seed, n) != 1:
    #     seed = random.getrandbits(128)
    seed = 145363402900758475023351086533584197681
    print("uint16_t seed[MAX_SIZE] = ", end='')
    print_c_format(seed)

    l = 256
    xi = pow(seed, 2, n)

    z = 0
    for i in range(l // 8):
        xi = pow(xi, 2, n)
        z |= ((xi & 0x00FF) << (i * 8))
    print("uint16_t bbs_expected[MAX_SIZE] = ", end='')
    print_c_format(z)


def gen_sts_test():
    p4096 = 2 ** 4096 - 2 ** 4032 - 1 + 2 ** 64 * (int(2 ** 3966 * mp.pi) + 240904)
    g = 2
    R = (2 ** 16) ** 256
    p4096_prime = R + (~mulinv(p4096, R) + 1)

    print()
    print("uint16_t p4096[MAX_SIZE] = ", end='')
    print_c_format(p4096)
    print("uint16_t p4096_g[MAX_SIZE] = ", end='')
    print_c_format(g)
    print("uint16_t p4096_prime[MAX_SIZE] = ", end='')
    print_c_format(p4096_prime)
    print("uint16_t R[MAX_SIZE] = ", end='')
    print_c_format(R)

    p = getPrime(2046)
    q = getPrime(2050)
    n_A = p * q
    n_A_prime = R + (~mulinv(n_A, R) + 1)
    phi = (p - 1) * (q - 1)
    e_A = 3
    while GCD(e_A, phi) != 1:
        e_A += 1
    d_A = inverse(e_A, phi)

    print()
    print("uint16_t n_A[MAX_SIZE] = ", end='')
    print_c_format(n_A)
    print("uint16_t n_A_prime[MAX_SIZE] = ", end='')
    print_c_format(n_A_prime)
    print("uint16_t e_A[MAX_SIZE] = ", end='')
    print_c_format(e_A)
    print("uint16_t d_A[MAX_SIZE] = ", end='')
    print_c_format(d_A)

    p = getPrime(2046)
    q = getPrime(2050)
    n_B = p * q
    n_B_prime = R + (~mulinv(n_B, R) + 1)
    phi = (p - 1) * (q - 1)
    e_B = 3
    while GCD(e_B, phi) != 1:
        e_B += 1
    d_B = inverse(e_B, phi)

    print()
    print("uint16_t n_B[MAX_SIZE] = ", end='')
    print_c_format(n_B)
    print("uint16_t n_B_prime[MAX_SIZE] = ", end='')
    print_c_format(n_B_prime)
    print("uint16_t e_B[MAX_SIZE] = ", end='')
    print_c_format(e_B)
    print("uint16_t d_B[MAX_SIZE] = ", end='')
    print_c_format(d_B)

    x = random.getrandbits(256)
    y = random.getrandbits(256)
    print()
    print("uint16_t x[MAX_SIZE] = ", end='')
    print_c_format(x)
    print("uint16_t y[MAX_SIZE] = ", end='')
    print_c_format(y)

    two_pow_x = pow(2, x, p4096)
    two_pow_y = pow(2, y, p4096)
    ss_A = pow(two_pow_x, y, p4096)
    ss_B = pow(two_pow_y, x, p4096)
    assert(ss_A == ss_B)
    bytes_ = ss_A.to_bytes(512, 'little')
    key = SHA256.new(bytes_)

    print()
    print("uint16_t two_pow_x_expected[MAX_SIZE] = ", end='')
    print_c_format(two_pow_x)
    print("uint16_t two_pow_y_expected[MAX_SIZE] = ", end='')
    print_c_format(two_pow_y)
    print("unsigned char key_expected[65] = \"{0}\";".format(key.hexdigest()))


def main():
    #test_monpro()
    gen_bbs_test()
    #gen_sts_test()


main()
