#ifndef RECEIVED_DATA2_H
#define RECEIVED_DATA2_H

#include <stdint.h>

extern uint8_t data_rx2[9];
extern uint8_t uart_len2;
extern uint8_t uart_flag2;
extern uint8_t receive_flag2;

void received_data2(uint8_t data2);

#endif // RECEIVED_DATA2_H
