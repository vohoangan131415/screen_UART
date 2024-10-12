#include "receivedata.h"

uint8_t data_rx[6];
uint8_t uart_len = 0;
uint8_t uart_flag = 0;
uint8_t receive_flag = 0;

void received_data(uint8_t data)
{
    if ((data == 0x97 || data == 0x98 || data == 0x99) && uart_len == 0)
    {
        receive_flag = 1;
    }
    if (receive_flag)
    {
        if (uart_len == 5)
        {
            data_rx[uart_len] = data;
            receive_flag = 0;
            uart_len = 0;
						uart_flag = 1;
        }
        else
        {
            data_rx[uart_len++] = data;
        }
    }
}
