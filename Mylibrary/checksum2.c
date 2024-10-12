
#include "checksum2.h"

uint8_t Data_enough_Sent2;
uint8_t th3_sent;

uint8_t calculate_checksum2(uint8_t *data, size_t length) // calculate checksum
{
    uint8_t checksum = 0;
    for (size_t i = 0; i < length - 1; i++) {
        checksum += data[i];
    }
    return checksum;
}

void check_sum2(uint8_t *data, uint8_t length) // compare checksum (transmit) // uart2
{
    uint8_t received_transmitted_checksum2 = data[length - 1];
    uint8_t calculated_checksum2 = calculate_checksum2(data, length);
    if(calculated_checksum2 == received_transmitted_checksum2)
    {
        Data_enough_Sent2 = 1;
    }
    else if(calculated_checksum2 == received_transmitted_checksum2 && th3_sent)
    {
        Data_enough_Sent2 = 1;
        th3_sent = 0;
    }
}
