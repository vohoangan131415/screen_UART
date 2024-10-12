#ifndef DISPLAYSTATUS_H
#define DISPLAYSTATUS_H

#include <stdint.h>
#include "stm32f1xx_hal.h"
extern UART_HandleTypeDef huart2;
#define SUCCESSFUL 1
#define FAILED 2
#define WORKING 3
#define ERROR 4
#define CLOSED_LOOP 5
#define IDLE 6
#define UNKNOWN 7
#define NA 8
#define StartSetup 9

typedef struct {
    uint8_t working[23];
    uint8_t successful[23];
    uint8_t failed[23];
    uint8_t error[23];
    uint8_t closed_loop[23];
    uint8_t idle[23];
    uint8_t unknown_status[23];
    uint8_t notEnough[23];
    uint8_t Enough[23];
    uint8_t NAstatus[23];
    uint8_t StartingAnnouce[23];
} displayContentstatus;

extern displayContentstatus status_dis_store;

void status_display(uint8_t status1);
displayContentstatus create_displaystatus(void);

#endif // DISPLAYSTATUS_H
