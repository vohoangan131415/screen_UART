#include "receivedata2.h"

uint8_t data_rx2[9];
uint8_t uart_len2 = 0;
uint8_t uart_flag2 = 0;
uint8_t receive_flag2 = 0;
void received_data2(uint8_t data2)
{
    if(data2 == 0x5A && uart_len2 == 0)
    {
        receive_flag2 = 1;
    }
    if(receive_flag2 && (data2 != 0x83 || data2 != 0x03 || data2 != 0x4B|| data2 != 0x4F))
    {
        if(uart_len2 == 8)
        {
            data_rx2[uart_len2] = data2;
            receive_flag2 = 0;
            uart_len2 = 0;
						uart_flag2 = 1;
           
        }
        else
        {
            data_rx2[uart_len2++] = data2;
       }
    }
    
}
