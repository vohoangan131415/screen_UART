#ifndef CHECKSUM2_H
#define CHECKSUM2_H

#include <stdint.h>
#include <stddef.h>

uint8_t calculate_checksum2(uint8_t *data, size_t length);
void check_sum2(uint8_t *data, uint8_t length);

extern uint8_t Data_enough_Sent2;
extern uint8_t th3_sent;

#endif 