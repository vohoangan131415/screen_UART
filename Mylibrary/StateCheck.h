#ifndef STATE_CHECK_H
#define STATE_CHECK_H

#include <stdint.h>



extern uint8_t Working_count; // Ð?m b?o r?ng b?n dã d?nh nghia bi?n này ? dâu dó
extern uint8_t success_failed_signal; // Ð?m b?o r?ng b?n dã d?nh nghia bi?n này ? dâu dó
extern uint8_t working_signal; // Ð?m b?o r?ng b?n dã d?nh nghia bi?n này ? dâu dó
extern uint8_t countState; // Ð?m b?o r?ng b?n dã d?nh nghia bi?n này ? dâu dó
extern int address;
extern uint32_t StartTime;

void StateCheck(uint8_t *StateData);
void timeCheck(uint32_t *time1, uint8_t *data);
void addressConfirm(uint8_t index);
uint8_t check_bit(uint8_t value, uint8_t bit_position);
void status_display(uint8_t status);

#endif // STATE_CHECK_H