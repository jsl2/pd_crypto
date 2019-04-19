#include <stdint.h>
#include "circular_buffer.h"

/* Append 15 bytes to buffer, copy 16 bytes to out data and advance out_ptr by 16 bytes if data ready.
 * Returns whether data in out_data is valid (Will be valid for 15 out of 16 steps of full cycle) */
uint8_t circular_buffer_append_15(uint16_t *in_data, uint8_t *circular_buffer, uint8_t **in_ptr, uint8_t **out_ptr,
                                  uint16_t *out_data) {
    uint8_t i;
    uint8_t *data_c = (uint8_t *) in_data;
    uint8_t data_rdy = 0;
    uint8_t *output_data_bytes = (uint8_t *) out_data;

    for (i = 0; i < 15; i++)
        (*in_ptr)[i] = data_c[i];

    for (i = 0; i < 16; i++)
        output_data_bytes[i] = (*out_ptr)[i];
    *in_ptr += 15;
    if (*in_ptr > (circular_buffer + 15))
        *out_ptr += 16;

    data_rdy = (uint8_t) (*out_ptr > circular_buffer);
    if (*in_ptr == (circular_buffer + CIRCULAR_BUFFER_LEN)) {
        *in_ptr = circular_buffer;
        *out_ptr = circular_buffer;
    }

    return data_rdy;
}

/* Returns whether there is data in the circular buffer which has not been output */
uint8_t circular_buffer_is_residual(uint8_t *circular_buffer, uint8_t *in_ptr) {
    return (uint8_t) (in_ptr != circular_buffer);
}

/* Gets residual of circular buffer, output in zero padded 128 bits (16 bytes).
 * Important: this function is for the input buffer (buffering from encoding for encryption).
 * There is no similar function for the output buffer. */
void circular_buffer_get_residual(uint8_t *circular_buffer, uint8_t *in_ptr, uint8_t *out_ptr, uint16_t *out_data) {
    uint8_t i;
    uint8_t step = (uint8_t) ((in_ptr - circular_buffer) / 15);
    uint8_t *output_data_bytes = (uint8_t *) out_data;
    for (i = 0; i < 16 - step; i++)
        output_data_bytes[i] = out_ptr[i];
    for (; i < 16; i++)
        output_data_bytes[i] = 0;
}

/* Append 16 bytes to buffer, copy 15 bytes to out data and advance out_ptr by 15 bytes.
 * Returns whether data in out_data is duplicate, i.e. 30 bytes rather than 15.
 * (Will be occur for 1 out of 16 steps of full cycle) */
uint8_t circular_buffer_append_16(uint16_t *in_data, uint8_t *circular_buffer, uint8_t **in_ptr, uint8_t **out_ptr,
                                  uint16_t *out_data) {
    uint8_t i;
    uint8_t *data_c = (uint8_t *) in_data;
    uint8_t duplicate_data = 0;
    uint8_t *output_data_bytes = (uint8_t *) out_data;
    for (i = 0; i < 16; i++)
        (*in_ptr)[i] = data_c[i];

    for (i = 0; i < 15; i++)
        output_data_bytes[i] = (*out_ptr)[i];
    output_data_bytes[15] = 0;

    *in_ptr += 16;
    *out_ptr += 15;

    if (*in_ptr == (circular_buffer + CIRCULAR_BUFFER_LEN)) {
        for (i = 0; i < 15; i++)
            output_data_bytes[i + 16] = (*out_ptr)[i];
        output_data_bytes[31] = 0;

        *in_ptr = circular_buffer;
        *out_ptr = circular_buffer;
        duplicate_data = 1;
    }
    return duplicate_data;
}