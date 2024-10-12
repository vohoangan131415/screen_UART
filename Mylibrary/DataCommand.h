#ifndef DATA_COMMAND_H
#define DATA_COMMAND_H
#include <stdint.h>


void DataCommand(uint8_t *dataCommand);
void package_display(uint8_t *data_7Block, uint8_t length);
#endif // DATA_COMMAND_H