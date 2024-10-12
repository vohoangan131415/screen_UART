#include "TimeCheck.h"
#include "stm32f1xx.h"
#include "statuschanging.h"
void timeCheck(uint32_t *time1, uint8_t *data)
{
    uint32_t currentTime = HAL_GetTick();

    if(success_failed_signal && (currentTime - *time1 > 5000) && (working_signal == 0))
    {
        working_signal = 1;
        status_changing(data);
        Working_count = 0;
				*time1 = HAL_GetTick();
    }
		}