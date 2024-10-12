#include "choice_selection.h"
#include "checksum2.h"


uint8_t data_tx0[3] = {0x99, 0x00, 0x99}; // testcase 1
uint8_t data_tx1[3] = {0x99, 0x01, 0x9A};	// testcase 2
uint8_t data_tx2[3] = {0x99, 0x02, 0x9B};	// testcase 3
uint8_t data_tx3[3] = {0x99, 0x03, 0x9C};	// testcase 4


void choice_selection_data_tx(uint8_t choice, uint8_t *data2)
{
    switch(choice)
    {
				th0_Sent = 1;
        case Th0: 
           check_sum2(data_tx0, 3);
            if(Data_enough_Sent2 && data2[8]== 0x01)
            {
                HAL_UART_Transmit(&huart1, data_tx0, sizeof(data_tx0), HAL_MAX_DELAY);
                Data_enough_Sent2 = 0;
                TH1_signal = 1;
            }
            else if(Data_enough_Sent2 && data2[8]== 0x00)
            {
                Data_enough_Sent2 = 0;
                TH1_signal = 0;
            }
            break;
        case Th1: 
            check_sum2(data_tx1, 3);
            if(Data_enough_Sent2 && th0_Sent && *(data2 + 8)== 1)
            {
                HAL_UART_Transmit(&huart1, data_tx1, sizeof(data_tx1), HAL_MAX_DELAY);
                Data_enough_Sent2 = 0;
                TH1_signal = 1;
            }
            else if(Data_enough_Sent2 && th0_Sent && *(data2 + 8) == 0)
            {
                Data_enough_Sent2 = 0;
                TH1_signal = 0;
            }
            break;
        case Th2: 
            check_sum2(data_tx2, 3);
            if(Data_enough_Sent2 && th0_Sent && *(data2 + 8)== 1)
            {
                HAL_UART_Transmit(&huart1, data_tx2, sizeof(data_tx2), HAL_MAX_DELAY);
                Data_enough_Sent2 = 0;
                TH2_signal = 1;
            }
            else if(Data_enough_Sent2 && th0_Sent && *(data2 + 8) == 0)
            {
                Data_enough_Sent2 = 0;
                TH2_signal = 0;
            }
            break;
        case Th3: 
            check_sum2(data_tx3, 3);
            if(Data_enough_Sent2 && *(data2 + 8) == 1)
            {
                HAL_UART_Transmit(&huart1, data_tx3, sizeof(data_tx3), HAL_MAX_DELAY);
            }
            else if(Data_enough_Sent2 && *(data2 + 8) == 0)
            {
                Data_enough_Sent2 = 0;
                TH3_signal = 0;
            }
            break;
    }
}
