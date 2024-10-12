#ifndef STATUS_CHANGING_H
#define STATUS_CHANGING_H

#include <stdint.h>

void status_changing(uint8_t *data_value);

extern uint8_t working_signal;
extern uint8_t success_failed_signal;
extern int address;
extern void status_display(uint8_t status);

#endif // STATUS_CHANGING_H