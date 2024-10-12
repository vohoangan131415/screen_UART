#include "ResetData.h"
#include "address.h"
#include "statusdisplay.h"
void Reset_data(uint8_t *dataaa)
{   
    uint8_t m = 0;
    uint8_t done = 0;

    for(int i = 1; i < 6; i++)
    {
        if(i == 1) { m = 20; }
        else if(i == 2) { m = 30; }
        else if(i == 3) { m = 40; }

        for(int k = 0; k < 8; k++)
        {
            addressConfirm(k + m);
            status_display(NA);
            done++;
        }

        if(clear_flag && done == 8)
        {
            if(i == 1) { address = 0x29; }
            else if(i == 2) { address = 0x39; }
            else if(i == 3) { address = 0x49; }
            else if(i == 4) { address = 0x59; }
            else if(i == 5) { address = 0x60; }
            status_display(NA);
        }

        done = 0;
    }
}