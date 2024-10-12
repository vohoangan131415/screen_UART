#ifndef CHOICE_SELECTION_DATA_TX_H
#define CHOICE_SELECTION_DATA_TX_H
#include <stdint.h>
#include <stm32f1xx.h>
#define Th0 8
#define Th1 9
#define Th2 10
#define Th3 11

extern UART_HandleTypeDef huart1;
 void choice_selection_data_tx(uint8_t choice, uint8_t *data2);
extern void check_sum2(uint8_t *data, uint8_t length);
extern uint8_t th0_Sent;
extern uint8_t TH0_signal;
extern uint8_t TH1_signal;
extern uint8_t TH2_signal;
extern uint8_t TH3_signal;
extern uint8_t flag;
#endif // CHOICE_SELECTION_DATA_TX_H