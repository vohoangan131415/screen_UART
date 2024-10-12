#include "checksum.h"

uint8_t Data_enough_Sent = 0; // Ð?nh nghia bi?n toàn c?c

uint8_t calculate_checksum(uint8_t *data, size_t length) // calculate checksum
{ 
    uint8_t checksum = 0;
    for (size_t i = 0; i < length - 1; i++) {
        checksum += data[i];
    }
    return checksum;
}

void check_sum(uint8_t *data, uint8_t length) // compare checksum (received)
{
    uint8_t received_transmitted_checksum = data[length - 1];
    uint8_t calculated_checksum = calculate_checksum(data, length);
    if(calculated_checksum == received_transmitted_checksum)
    {
        Data_enough_Sent = 1;
    }
    else
    {
        // X? lý khi checksum không kh?p n?u c?n
    }
}