#include "other.h"
#include "statusdisplay.h" // Ch?c ch?n r?ng b?n dã bao g?m thu vi?n status_display
#include "address.h" // N?u c?n s? d?ng address t? file khác
#include "StateCheck.h"
#define Odrive_BIT 0x80 // bit 1 (HIGH) 0000 0010
#define NguonDongLuc_BIT 0x40 // bit 2 0000 0100
uint8_t Odrive_State;
uint8_t NguonDongLuc;
uint8_t Working_count = 0; 
void Other(uint8_t *OtherData) // 5 represents for the position of the bit
{
    for(int i = 0; i < 2; i++)
    {
        if(i == 0) 
        {
            address = 0x60;
            Odrive_State = check_bit((*(OtherData + 4) & Odrive_BIT), 7); // 0x80
            if(Odrive_State == 0x00)
            {
                status_display(IDLE);
            }
            else if(Odrive_State == 0x01)
            {
                status_display(CLOSED_LOOP);
            }
        }
        else if(i == 1) 
        {
            address = 0x59;
            NguonDongLuc = check_bit((*(OtherData + 4) & NguonDongLuc_BIT), 6);
            if(NguonDongLuc == 0x00)
            {
                status_display(ERROR);
            }
            else if(NguonDongLuc == 0x01)
            {
                status_display(WORKING);
                Working_count++;
            }
        }
		}
}