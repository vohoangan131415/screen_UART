#ifndef PACKAGE_SENDING_H
#define PACKAGE_SENDING_H
#include "stm32f1xx.h"
#include <stdint.h>
extern UART_HandleTypeDef huart2;

void package_sending(uint8_t *data2);
#endif // PACKAGE_SENDING_H