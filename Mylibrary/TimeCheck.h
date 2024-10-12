#ifndef TIME_CHECK_H
#define TIME_CHECK_H

#include <stdint.h>

void timeCheck(uint32_t *time1, uint8_t *data);

extern uint8_t success_failed_signal;
extern uint8_t working_signal;
extern uint8_t Working_count;

#endif // TIME_CHECK_H