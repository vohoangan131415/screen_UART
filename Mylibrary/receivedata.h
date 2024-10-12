#ifndef RECEIVED_DATA_H
#define RECEIVED_DATA_H

#include <stdint.h>

extern uint8_t data_rx[6];
extern uint8_t uart_len;
extern uint8_t uart_flag;
extern uint8_t receive_flag;

void received_data(uint8_t data);

#endif // RECEIVED_DATA_H
