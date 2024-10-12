#ifndef CHECKSUM_H
#define CHECKSUM_H

#include <stdint.h>
#include <stddef.h>

uint8_t calculate_checksum(uint8_t *data, size_t length);
void check_sum(uint8_t *data, uint8_t length);

extern uint8_t Data_enough_Sent; // Ð?m b?o r?ng b?n dã d?nh nghia bi?n này ? dâu dó

#endif // CHECKSUM_H