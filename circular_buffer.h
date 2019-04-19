#ifndef CRYPTO_CIRCULAR_BUFFER_H
#define CRYPTO_CIRCULAR_BUFFER_H

uint8_t circular_buffer_append_15(uint16_t *in_data, uint8_t *circular_buffer, uint8_t **in_ptr, uint8_t **out_ptr,
                                  uint16_t *out_data);

uint8_t circular_buffer_is_residual(uint8_t *circular_buffer, uint8_t *in_ptr);

void circular_buffer_get_residual(uint8_t *circular_buffer, uint8_t *in_ptr, uint8_t *out_ptr, uint16_t *out_data);

uint8_t circular_buffer_append_16(uint16_t *in_data, uint8_t *circular_buffer, uint8_t **in_ptr, uint8_t **out_ptr,
                                  uint16_t *out_data);

#endif //CRYPTO_CIRCULAR_BUFFER_H
