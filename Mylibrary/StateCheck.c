#include "StateCheck.h"
#include "statusdisplay.h"
 uint32_t StartTime=0;

uint8_t check_bit(uint8_t value, uint8_t bit_position) {
    return (value >> bit_position) & 0x01;
}
void StateCheck(uint8_t *StateData)
{
    for(int i = 1; i < 4; i++)
    {
        uint8_t j;
        uint8_t o;
        uint8_t stateEach;
        if(i == 1)
        {
            j = 20;
            o = 20;
        }
        else if(i == 2)
        {
            j = 30;
            o = 30;
        }
        else if(i == 3)
        {
            j = 40;
            o = 40;
        }
        //else if (i == 4) {address = 0x61; Other(StateData);}

        for(int k = 0; k < 8; k++)
        {
            addressConfirm(k + j);
            stateEach = check_bit(*(StateData + i), k);
            if(stateEach == 0x01) 
            {   
                status_display(SUCCESSFUL);
                countState++;
                address = 0; 
            }
            else if(stateEach == 0x00)
            {
                status_display(FAILED);
                address = 0;
            }
        } 

        if(countState == 8 && success_failed_signal)
        {
            if(i == 1) { address = 0x29; }
            else if(i == 2) { address = 0x39;}
            else if(i == 3) { address = 0x49;}

            if(*StateData == 0x98 && (working_signal == 0))
            {
                status_display(SUCCESSFUL);
            }
            else if(*StateData == 0x99 && (working_signal == 1))
            {
                status_display(WORKING);
            }
            Working_count++;
        }

        if(countState != 8 && success_failed_signal)
        {
            if(i == 1) { address = 0x29; }
            else if(i == 2) { address = 0x39; }
            else if(i == 3) { address = 0x49; }

            if(*StateData == 0x98 && (working_signal == 0))
            {
                status_display(FAILED);
            }
            else if(*StateData == 0x99 && (working_signal == 1))
            {
                status_display(ERROR);
            }
        }

        countState = 0;

        if(Working_count == 4)
        {
            StartTime = HAL_GetTick();
        }
    }
}