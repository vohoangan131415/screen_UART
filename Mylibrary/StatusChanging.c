#include "StatusChanging.h"
#include "statusdisplay.h"

void status_changing(uint8_t *data_value)
{
    if(working_signal)
    {
        for(int i = 1; i < 5; i++)
        {
            if(i == 1) { address = 0x29; }
            else if(i == 2) { address = 0x39; }
            else if(i == 3) { address = 0x49; }
            else if(i == 4) { address = 0x59; }

            if(success_failed_signal)
            {
                status_display(WORKING);
            }
        }
    }
		}
