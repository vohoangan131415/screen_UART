#include "address.h"

// Ð?nh nghia bi?n toàn c?c
int address;

// Ð?nh nghia hàm addressConfirm
void addressConfirm(uint8_t index)
{
    switch(index)
    {
        case 29: 
            address = 0x29; // SENSOR STATE (working/error) (MAIN)
            break;
        case 20:
            address = 0x20; // SenSor state (0)
            break;
        case 21:
            address = 0x21;  // SenSor state (1)
            break;
        case 22:
            address = 0x22;  // SenSor state (2)
            break;
        case 23:
            address = 0x23;   // SenSor state (3)
            break;
        case 24:
            address = 0x24;   // SenSor state (4)
            break;
        case 25: 
            address = 0x25;   // SenSor state (5)
            break;
        case 26: 
            address = 0x26;   // SenSor state (6)
            break;
        case 27: 
            address = 0x27;   // SenSor state (7)
            break;
        case 39:
            address = 0x39; // ODRIVE (WORKING/ ERROR) nam o tren CLOSEDLOOP / IDLE (MAIN)
            break;
        case 30:
            address = 0x30; // Odrive state (0)
            break;
        case 31:
            address = 0x31;  // Odrive state (1)
            break;
        case 32:
            address = 0x32;  // Odrive state (2)
            break;
        case 33:
            address = 0x33;   // Odrive state (3)
            break;
        case 34:
            address = 0x34;   // Odrive state (4)
            break;
        case 35: 
            address = 0x35;   // Odrive state (5)
            break;
        case 36: 
            address = 0x36;   // Odrive state (6)
            break;
        case 37: 
            address = 0x37;   // Odrive state (7)
            break;
        case 49: 
            address = 0x49; // DEVICE STATE (WORKING/ ERROR) //(MAIN)
            break;
        case 40:
            address = 0x40; // Device State (0)
            break;
        case 41:
            address = 0x41;  // Device State (1)
            break;
        case 42:
            address = 0x42;  // Device State (2)
            break;
        case 43:
            address = 0x43;   // Device State (3)
            break;
        case 44:
            address = 0x44;   // Device State (4)
            break;
        case 45: 
            address = 0x45;   // Device State (5)
            break;
        case 46: 
            address = 0x46;   // Device State (6)
            break;
        case 47: 
            address = 0x47;   // Device State (7)
            break;
        case 60: 
            address = 0x60; // Other (IDLE / CLOSED LOOP) hien thi o Odrive nhung nam ben duoi
            break;
        case 59: 
            address = 0x59; // Other (ERROR / WORKING) POWER (MAIN)
			}
}
