#include <stdint.h>
#include "bbs.h"
#include "mp_arithmetic.h"

uint16_t bbs_n[MAX_SIZE] = {0x100, 0xe375, 0x66a4, 0xfa4e, 0xd3b1, 0x6665, 0x78a5, 0x1e68, 0xaff5, 0xfb04, 0x8b7f,
                            0x3444, 0x2071, 0x85f7, 0xa84f, 0x34bd, 0xc8fd, 0x4846, 0xfd04, 0x404f, 0xcfd7, 0x471f,
                            0x4d97, 0xe8ca, 0x8c5e, 0xc2f7, 0x6dd7, 0xd97e, 0x442d, 0x796e, 0xeb49, 0x0597, 0xd58e,
                            0xe680, 0x4964, 0xbc2f, 0xd26a, 0x8927, 0xc22d, 0x5eb4, 0x14ab, 0xc6ee, 0x6c3b, 0x72ea,
                            0x2525, 0x2a9d, 0x09eb, 0x924b, 0x8a09, 0x12e6, 0x080c, 0xa975, 0x6401, 0xaf10, 0x5b31,
                            0xe881, 0x9f4c, 0x5765, 0xdb38, 0xea9b, 0x1302, 0x3370, 0xd56e, 0x7b26, 0x996d, 0x4db1,
                            0x5de8, 0x05a2, 0x9c8a, 0x52a3, 0x4f0b, 0x409f, 0x3876, 0xaf88, 0x1af9, 0x1c2a, 0xecdc,
                            0x3c84, 0x2027, 0x5442, 0x38c0, 0x53b8, 0xae19, 0xfd69, 0xa18e, 0x6c1d, 0x0a91, 0x2cd6,
                            0x3c0a, 0xa1a4, 0x5472, 0x02a0, 0x8f0c, 0x1d0d, 0x64db, 0x7913, 0x3b91, 0x0ca9, 0xcd54,
                            0x0b06, 0x495f, 0x5059, 0xa24f, 0x8cc2, 0xe3a8, 0xcac1, 0x2d2b, 0xf272, 0x82ba, 0x1334,
                            0x492d, 0x4839, 0x014f, 0xf0fc, 0x1b26, 0x208c, 0x536e, 0x5a59, 0x42ed, 0xcae9, 0x3a9f,
                            0x97c9, 0x51cf, 0xdb4a, 0x928f, 0x2725, 0xe747, 0x54c9, 0xb9ff, 0x8671, 0x8229, 0xb9dd,
                            0x3a76, 0x9802, 0xd048, 0x6047, 0x717d, 0xc9d3, 0x3f95, 0x6475, 0xba49, 0xc81f, 0x1b36,
                            0x8e9c, 0xa2b5, 0x95ea, 0x70ef, 0x18de, 0x7cbd, 0x92bb, 0xe35e, 0x4fa0, 0xc75b, 0x95d9,
                            0x6fe3, 0x24f0, 0x7a3b, 0x9582, 0xa808, 0x625a, 0xab2a, 0x3008, 0x0d40, 0xb4d9, 0x3b8b,
                            0x71d6, 0xcc11, 0x6080, 0x7350, 0x7af1, 0xc2ae, 0xb92f, 0x5138, 0x351b, 0xc97d, 0x318f,
                            0x4b9c, 0xb221, 0xb59e, 0xe39a, 0xb239, 0x09f8, 0x7a92, 0xeaf2, 0xf9eb, 0xdc2f, 0x1d28,
                            0x5e0a, 0x1b47, 0x1a51, 0xa4d8, 0xf7f0, 0xfe5c, 0xde46, 0x4b8d, 0x6901, 0xc637, 0x9a93,
                            0xa7fd, 0x522f, 0x00e8, 0x1e0c, 0x08ce, 0x10ab, 0xe188, 0xa1cb, 0xf73b, 0x7e1e, 0xf324,
                            0x34ee, 0x3441, 0xe829, 0xd6a9, 0x606f, 0xadc8, 0x667e, 0x24ee, 0x6127, 0xbb1a, 0xe6f6,
                            0x575a, 0x9072, 0xac56, 0xa623, 0xde00, 0x0f06, 0xf2a4, 0x4494, 0x9637, 0x1e7f, 0x2923,
                            0x8ba2, 0xf0a3, 0x33c8, 0x0e95, 0x90da, 0xcba6, 0x4b20, 0x8d6d, 0xcdca, 0x0915, 0x3e71,
                            0xf884, 0x9090, 0x9df2, 0xbcff, 0xe01b, 0x2323, 0x8f95, 0xd7f6, 0x2ac7, 0x86de, 0xa9dd,
                            0x4055, 0x4344, 0xc84d, 0xdae9};

void bbs(uint16_t *seed, uint16_t len, uint16_t *out) {
    uint16_t xi[MAX_SIZE] = {0};
    uint16_t square[MAX_SIZE] = {0};
    uint16_t temp[MAX_SIZE] = {0};
    uint16_t i;

    mp_square(seed, square);
    mp_div(square, bbs_n, temp, xi);

    for (i = 0; i < (len / 8); i++) {
        mp_square(xi, square);
        mp_div(square, bbs_n, temp, xi);
        /* Take bottom 8 bits instead of single bit.
         * Proven limit of log2(log2(n)) bits (11 in this case) may be taken */
        out[1 + (i / 2)] |= ((xi[1] & 0x00FFu) << ((i & 1u) << 3u));
    }
    out[0] = (uint16_t) (len / 16);
}
