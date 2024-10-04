/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
#include "stdint.h"
#include "stdio.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//===========================SETTING, TESTCASE, PLACE ===========================================
//setting define for status_display function 
#define STARTING 0
#define SUCCESSFUL 1
#define FAILED 2
#define WORKING 3
#define ERROR 4
#define CLOSED_LOOP 5
#define IDLE 6
#define UNKNOWN 7
#define NA 11
#define StartSetup 12
//setting define for Other block 
#define Odrive_BIT 0x80 // bit 1 (HIGH) 0000 0010
#define NguonDongLuc_BIT 0x40 // bit 2 0000 0100
// setting a place for output (PMM,OTher)
//setting for clear and start command
////uart received setting
#define DATA_LEN 6
#define DATA_LEN2 9
#define DATA_TRANS_LEN 3
uint8_t data_rx[DATA_LEN];
uint8_t uart_len = 0;
uint8_t uart_flag = 0;
uint8_t receive_flag = 0;
uint8_t countState = 0;
uint32_t time_hoatdong;
 uint8_t data_rx2[DATA_LEN2];
//uint8_t data_rx2[9];
 uint8_t uart_len2 = 0;
 uint8_t uart_flag2 = 0;
 uint8_t receive_flag2 = 0;
static uint32_t StartTime=0;
uint32_t First_time = 0;
uint8_t data;
uint8_t data2;
uint8_t count = 0;
uint8_t success_failed_signal = 0;

//uint32_t *time;
#define nochoice 12 // (initial value)
uint8_t data_tx0[3] = {0x99, 0x00, 0x99}; // testcase 1
uint8_t data_tx1[3] = {0x99, 0x01, 0x9A};	// testcase 2
uint8_t data_tx2[3] = {0x99, 0x02, 0x9B};	// testcase 3
uint8_t data_tx3[3] = {0x99, 0x03, 0x9C};	// testcase 4

uint8_t work_change_signal = 0;
uint8_t error_change_signal = 0;
//===========================VARIABLE===========================================

// transmit data signal (transmitted data)
#define Th0 8
#define Th1 9
#define Th2 10
#define Th3 11
// variable for choice(transmitted data)
uint8_t th0_Sent = 0; //decide if TH1 and Th2 can be sent
uint8_t Data_enough_Sent = 0;
uint8_t Data_enough_Sent2 = 0;
uint8_t th3_sent = 0;
uint8_t Working_count = 0;
// variable for error check 

uint8_t working_signal = 0;
uint8_t countClear = 0;

uint8_t count_start = 0;

uint8_t clear_flag= 0;
//===========================DISPLAY===========================================
// error display
uint8_t errorAnnouce[20];

//===========================FUNCTION===========================================
// 											TABLE OF CONTENTS
//	RECEIVE AND STORE DATA for huart1 (line 136)   
//  RECEIVE AND STORE DATA for huart2 (line 158)
//	STATUS DISPLAY(line 180-270)						
//	STATUS SWITCHING (line 272-302) 				
//	DISPLAY INITIAL STATUS (line 305) 	
//	HANDLE OTHER BLOCK (line 341)				
//	CHECK SUM	(line 370)								
//	command CLEAR (line 365)
// 	TRANSMITTED DATA (443-514)


// FUNCTION FOR RECEIVE AND STORE DATA ====================================================================

void received_data2(uint8_t data2)
{
	
	if(data2 == 0x5A && uart_len2 == 0)
	{
		receive_flag2= 1;
	}
	if(receive_flag2)
	{
		if(uart_len2 == 8)
		{
			data_rx2[uart_len2] = data2;
			uart_flag2 = 1;
		}
		else
		{
			data_rx2[uart_len2++] = data2;
		}
	}
		receive_flag2 = 0;
	
}
void received_data(uint8_t data)
{
	if((data == 0x97 || data == 0x98 || data == 0x99) && uart_len == 0 )
	
	{
		receive_flag = 1;
	}
	if(receive_flag)
	{
		if(uart_len == 7)
		{
			data_rx[uart_len] = data;
			uart_flag = 1;
		}
		else
		{
			data_rx[uart_len++] = data;
			uart_len++;
		}
	}
	
	receive_flag = 0;
	
}

void resetForSending(uint8_t *dataSend)
{
	for(int i =0; i < 9; i++)
	{
		dataSend[i] = 0x00;
		
	}
	
	uart_flag2 = 0;
	//uart_flag = 0;
} 

 //FUNCTION FOR STATUS DISPLAY ====================================================================
typedef struct
{
	
	uint8_t working[23];
	uint8_t successful[23];
	uint8_t failed[23];
	uint8_t error[23];
	uint8_t closed_loop[23];
	uint8_t idle[23];
	uint8_t unknown_status[23];
	uint8_t notEnough[23];
	uint8_t Enough[23];
	uint8_t NAstatus[23];
	uint8_t StartingAnnouce[23];
}displayContentstatus;
displayContentstatus status_dis_store;
int address;
void addressConfirm(uint8_t index)
{
		
		switch(index)
		{
			case 29: 
				address = 0x29;			//SENSOR STATE (working/error) (MAIN)
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
				address = 0x23;   //SenSor state (3)
				break;
			case 24:
				address = 0x24;   //SenSor state  (4)
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
				address = 0x33;   //Odrive state (3)
				break;
			case 34:
				address = 0x34;   //Odrive state (4)
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
			case 40:
				address = 0x40; //  Device State (0)
				break;
			case 41:
				address = 0x41;  // Device State (1)
				break;
			case 42:
				address = 0x42;  // Device State (2)
				break;
			case 43:
				address = 0x43;   // 	Device State (3)
				break;
			case 44:
				address = 0x44;   //	Device State (4)
				break;
			case 45: 
				address = 0x45;   //	Device State (5)
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
				address = 0x59; // Other (ERROR / WORKING) POWER  (MAIN)
			break;
		}
	
}
displayContentstatus create_displaystatus()
{
	
	displayContentstatus status = {
												{0x5a, 0xa5, 0x14, 0x82, address, 0x00, 0x77, 0x6f, 0x72, 0x6b, 0x69, 0x6e, 0x67, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, //working
												{0x5a, 0xa5, 0x14, 0x82, address, 0x00, 0x73, 0x75, 0x63, 0x63, 0x65, 0x73, 0x73, 0x66, 0x75, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // successful
												{0x5a, 0xa5, 0x14, 0x82, address, 0x00, 0x66, 0x61, 0x69, 0x6c, 0x65, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // failed
												{0x5a, 0xa5, 0x14, 0x82, address, 0x00, 0x65, 0x72, 0x72, 0x6f, 0x72, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // error
												{0x5a, 0xa5, 0x14, 0x82, address, 0x00, 0x63, 0x6c, 0x6f, 0x73, 0x65, 0x64, 0x6c, 0x6f, 0x6f, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},// Closed_loop
												{0x5a, 0xa5, 0x14, 0x82, address, 0x00, 0x69, 0x64, 0x6c, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},// IDLE
												{0x5a, 0xa5, 0x14, 0x82, address, 0x00, 0x75, 0x6e, 0x6b, 0x6e, 0x6f, 0x77, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},// UNKNOWN
												{0x5a, 0xa5, 0x14, 0x82, address, 0x00, 0x6e, 0x6f, 0x74, 0x65, 0x6e, 0x6f, 0x75, 0x67, 0x68, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},// chua du du lieu ( address #)
												{0x5a, 0xa5, 0x14, 0x82, address, 0x00, 0x65, 0x6e, 0x6f, 0x75, 0x67, 0x68, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},//  du du lieu ( address #)
												{0x5a, 0xa5, 0x14, 0x82, address, 0x00, 0x4E, 0x2F, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},    //  NA 
												{0x5a, 0xa5, 0x14, 0x82, address, 0x00, 0x77, 0x61, 0x69, 0x74, 0x69, 0x6E, 0x67, 0x20, 0x74, 0x6F, 0x20, 0x73, 0x65, 0x74, 0x20, 0x75, 0x70}  //  starting annouce
   
											};
				return status;
}



void status_display(uint8_t status1)
{
	status_dis_store = create_displaystatus();
	switch(status1){
        
        case SUCCESSFUL:
            HAL_UART_Transmit(&huart2, status_dis_store.successful, sizeof(status_dis_store.successful), HAL_MAX_DELAY);
						//HAL_UART_Transmit(&huart2, send, sizeof(send), HAL_MAX_DELAY);
            break;
        case FAILED:
            HAL_UART_Transmit(&huart2, status_dis_store.failed, sizeof(status_dis_store.failed), HAL_MAX_DELAY);
            break;
        case WORKING:
            HAL_UART_Transmit(&huart2, status_dis_store.working, sizeof(status_dis_store.working), HAL_MAX_DELAY);
            //HAL_UART_Transmit(&huart2, send_data, sizeof(send_data), HAL_MAX_DELAY);
						break;
        case ERROR:
            HAL_UART_Transmit(&huart2, status_dis_store.error, sizeof(status_dis_store.error), HAL_MAX_DELAY);
            break;
        case CLOSED_LOOP:
            HAL_UART_Transmit(&huart2, status_dis_store.closed_loop, sizeof(status_dis_store.closed_loop), HAL_MAX_DELAY);
            break;
        case IDLE:
            HAL_UART_Transmit(&huart2, status_dis_store.idle, sizeof(status_dis_store.idle), HAL_MAX_DELAY);
            break;
        case UNKNOWN:
            HAL_UART_Transmit(&huart2, status_dis_store.unknown_status, sizeof(status_dis_store.unknown_status), HAL_MAX_DELAY);
            break;
				 case NA:
            HAL_UART_Transmit(&huart2, status_dis_store.NAstatus, sizeof(status_dis_store.NAstatus), HAL_MAX_DELAY);
            break;
				 case StartSetup:
            HAL_UART_Transmit(&huart2, status_dis_store.StartingAnnouce, sizeof(status_dis_store.StartingAnnouce), HAL_MAX_DELAY);
            break;
	}	
	}

uint8_t check_bit(uint8_t value, uint8_t bit_position) {
    return (value >> bit_position) & 0x01;
}
// FUCNTION FOR STATUS SWITCHING=================================================


void Other(uint8_t *OtherData) // 5 represents for the position of the bit
{
	uint8_t Odrive_State;
	uint8_t NguonDongLuc;
	for(int i = 0; i < 2; i++)
	{
		if( i == 0 ) 
		{
			address = 0x60;
			Odrive_State = check_bit((*(OtherData + 4) & Odrive_BIT), 7); //0x80
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
void Reset_data(uint8_t *dataaa)
{	
		
		uint8_t m = 0;
		
		uint8_t done = 0;
	for(int i = 1; i <6; i++)
	{
		
		if(i == 1) {m = 20;}
		else if(i == 2) {m = 30;}
		else if(i == 3) {m = 40;}
		for(int k = 0; k < 8; k++)
		{
			addressConfirm(k+m);
			status_display(NA);
			done++;
		}
		
		if(clear_flag && done == 8)
			{
							if(i == 1) {address = 0x29; }
							else if(i== 2) {address = 0x39;}
							else if(i == 3) {address = 0x49;}
							else if(i == 4) {address = 0x59;}
							else if(i == 5) {address = 0x60;}
							status_display(NA);
						}
		done = 0;
						
	}
	
	
}
//// FUNCTION FOR HANDLE OTHER BLOCK==================================================


	

void status_changing(uint8_t *data_value)
{
	
	if(working_signal){
		for(int i = 1; i < 5; i++)
		{
				if(i == 1){address = 0x29;}
				else if(i == 2){address = 0x39;}
				else if(i == 3){address = 0x49;}
				else if(i == 4){address = 0x59;}
			
				if(success_failed_signal)
				{
					
					status_display(WORKING);
					work_change_signal = 0;
				
				}
			}
		}
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
		else if( i == 2){j = 30; o = 30;}
		else if( i == 3){j = 40;	o = 40;}
		//else if ( i == 4) {address = 0x61; Other(StateData);}
		
		for(int k = 0; k < 8; k++)
		{
				//countState = 1;
				
				addressConfirm(k + j);
				stateEach = check_bit(*(StateData +i), k);
					if(stateEach == 0x01) 
						{	
							//addressConfirm(k + j);
							status_display(SUCCESSFUL);
							countState++;
							address = 0; 
								
						}
							else if(stateEach == 0x00)
						{
							//addressConfirm(k + j);
							status_display(FAILED);
							address = 0;
						}

				
		} 
					if(countState == 8 && success_failed_signal)
						{
							//addressConfirm(o + 9);
							if(i == 1){address = 0x29;}
								else if (i == 2){address = 0x39;}
								else if (i== 3){address = 0x49;}
							if(*StateData == 0x98 && (working_signal == 0))
								{
								status_display(SUCCESSFUL);
								}
							else if(*StateData == 0x99 && (working_signal == 1))
							{
//								work_change_signal =1;
									status_display(WORKING);
							}
//							else if(*StateData == 0x98 && clear_flag)
//							{
//								status_display(SUCCESSFUL);
//								working_signal = 0;
//								clear_flag = 0;
//								Working_count = 0;
//							}
							Working_count++;
								
						}
							
				if(countState != 8 && success_failed_signal)
									{
								if(i == 1){address = 0x29;}
								else if (i == 2){address = 0x39;}
								else if (i== 3){address = 0x49;}
										//addressConfirm(o + 9);
									if(*StateData == 0x98 && (working_signal == 0))
										{
											status_display(FAILED);
										}
									else if(*StateData == 0x99 && (working_signal == 1))
									{
										//error_change_signal = 1 ;
										status_display(ERROR);
									}
//										else if(*StateData == 0x98 && clear_flag)
//											{
//												status_display(SUCCESSFUL);
//												working_signal = 0;
//												clear_flag = 0;
//												Working_count = 0;
//											}
									}
					
		
		// MAIN		
		countState = 0;

	}

	if(Working_count == 4){
	StartTime = HAL_GetTick();

}
		
}

void timeCheck(uint32_t *time1, uint8_t *data)
{
			uint32_t currentTime = HAL_GetTick();
	
		 if(success_failed_signal && (currentTime - *time1 > 2000) && (working_signal == 0))
		{
			
					working_signal =1;
					status_changing(data);
				Working_count = 0;

		 }
		}		
// FUNCTION FOR CHECK SUM ==================================================
uint8_t calculate_checksum(uint8_t *data, size_t length) // calculate checksum
{ 
    uint8_t checksum = 0;
    for (size_t i = 0; i < length - 1; i++) {
        checksum += data[i];
    }
    return checksum;
}
uint8_t calculate_checksum2(uint8_t *data, size_t length) // calculate checksum
{ 
    uint8_t checksum = 0;
    for (size_t i = 5; i < length - 1; i++) {
        checksum += data[i];
    }
    return checksum;
}

void check_sum(uint8_t *data, uint8_t length) // compare checksum (received)
{
		status_dis_store = create_displaystatus();
		uint8_t received_transmitted_checksum = data[length - 1];
		uint8_t calculated_checksum = calculate_checksum(data, length);
		if(calculated_checksum == received_transmitted_checksum)
		{
				Data_enough_Sent = 1;
				//HAL_UART_Transmit(&huart1, status_dis_store.Enough, sizeof(status_dis_store.Enough), HAL_MAX_DELAY);
				
		}
		else
		{
			//HAL_UART_Transmit(&huart1,status_dis_store.notEnough, sizeof(status_dis_store.notEnough), HAL_MAX_DELAY );
		}
}
void check_sum2(uint8_t *data, uint8_t length) // compare checksum (transmit) // uart2
{
		status_dis_store = create_displaystatus();
		uint8_t received_transmitted_checksum = data[length - 1];
		uint8_t calculated_checksum = calculate_checksum2(data, length);
		if(calculated_checksum == received_transmitted_checksum)
		{
				Data_enough_Sent2 = 1;
				//HAL_UART_Transmit(&huart1, status_dis_store.Enough, sizeof(status_dis_store.Enough), HAL_MAX_DELAY);
				
		}
		else if(calculated_checksum == received_transmitted_checksum && th3_sent)
		{
			Data_enough_Sent2 = 1;
			th3_sent = 0;
			//HAL_UART_Transmit(&huart1,status_dis_store.Enough, sizeof(status_dis_store.Enough), HAL_MAX_DELAY );
		}
		else
		{
			//HAL_UART_Transmit(&huart1,status_dis_store.notEnough, sizeof(status_dis_store.notEnough), HAL_MAX_DELAY );
		}
}

////Function for command CLEAR==================================================


////========================================================================================
////																			TRANSMITTED DATA
// CHOICE FOR 0x00, 0x01, 0x02, 0x03 UART2
uint8_t TH0_signal = 0;
uint8_t TH1_signal = 0;
uint8_t TH2_signal = 0;
uint8_t TH3_signal = 0;


void choice_selection_data_tx(uint8_t choice, uint8_t *data2)
{
	switch(choice)
	{
		case Th0: 
				check_sum2(data_rx2,DATA_LEN2);
		
				if(Data_enough_Sent2 && data2[8] == 1)
					{
							HAL_UART_Transmit(&huart1, data_tx0, sizeof(data_tx0), HAL_MAX_DELAY);
							Data_enough_Sent2 = 0;
							TH0_signal = 1;
						//
					}
					else if(Data_enough_Sent2 && data2[8] == 0)
					{
							Data_enough_Sent2 = 0;
							TH0_signal = 0;
					}
					th0_Sent = 1;
			break;
		case Th1: 
					check_sum2(data_rx2,DATA_LEN2); 
					if(Data_enough_Sent2 && th0_Sent && data2[8] == 1)
					{
							HAL_UART_Transmit(&huart1, data_tx1, sizeof(data_tx1), HAL_MAX_DELAY);
							Data_enough_Sent2 = 0;
							TH1_signal =1;
					}
					else if(Data_enough_Sent2 && th0_Sent && data2[8] == 0)
					{
							Data_enough_Sent2 = 0;
							TH1_signal =0;
					}
			break;
		case Th2: 
					check_sum2(data_rx2,DATA_LEN2);
					if(Data_enough_Sent2 && th0_Sent && data2[8] == 1)
					{
							HAL_UART_Transmit(&huart1, data_tx2, sizeof(data_tx2), HAL_MAX_DELAY);
							Data_enough_Sent2 = 0;
							TH2_signal = 1;
					}
					else if(Data_enough_Sent2 && th0_Sent && data2[8] == 0)
					{
							Data_enough_Sent2 = 0;
							TH2_signal =0;
					}
			break;
			case Th3: 
					check_sum2(data_rx2,DATA_LEN2);
					if(Data_enough_Sent2 && data2[8] == 1)
					{
							HAL_UART_Transmit(&huart1, data_tx3, sizeof(data_tx3), HAL_MAX_DELAY);
							
					}
					else if( Data_enough_Sent2 && data2[8] == 0)
					{
							Data_enough_Sent2 = 0;
							TH3_signal = 0;
					}
			break;
			
	}
}

//===================================================================================================================
// FUNCTION FOR DISPLAY 7 block, value of PMM, OTHER, status, check sum ==================================================
void package_display(uint8_t *data_7Block, uint8_t length)
{
			
				if(Data_enough_Sent)
				{
								Data_enough_Sent = 0;
								Other(data_7Block);
								StateCheck(data_7Block);
								
					
				}
}

void package_sending(uint8_t *data2)  // UART2
{
	for(int i = 0; i < 8; i++)
	{
		if(data2[0] == 0x5A && (data2[8] == 1 || data2[8] ==0))
		{
			if(i == 4)
				{
					if(data2[i] == 0x65){
					choice_selection_data_tx(Th0,data2);
					}
					else if(data2[i] == 0x66&& th0_Sent){
					choice_selection_data_tx(Th1,data2);}
					else if(data2[i] == 0x67 && th0_Sent){
					choice_selection_data_tx(Th2,data2);}
					else if(data2[i] == 0x68 ){
					choice_selection_data_tx(Th3,data2);}
				}
	}
}
}
void DataCommand(uint8_t *dataCommand)
{

	for(int l = 0; l < 5; l++)
	{

		if(dataCommand[0] == 0x97 && dataCommand[l + 1] == 0x00)
		{
			clear_flag = 1;
			Reset_data(data_rx);
			time_hoatdong = 0;
			working_signal = 0;
		}
		 if(dataCommand[0] == 0x98)
		{
			success_failed_signal = 1;
			package_display(data_rx, DATA_LEN);
		}
		 if(dataCommand[0] == 0x99)
		{
			
			if(working_signal)
			{
					StateCheck(data_rx);
					Other(data_rx);
			}
			
		}
		
	}
	if(dataCommand[0] == 0x5A)
		{
			
			package_sending(data_rx2);
		}
}

// FUNCTION FOR CONTINUOUS DATA==================================================
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == huart1.Instance)
	{
		uart_flag = 1;
		
		HAL_UART_Receive_IT(&huart1, data_rx, sizeof(data_rx));
		received_data(data);
		Working_count = 0;
		
	}
	  if(huart->Instance == huart2.Instance)
	{
		
		uart_flag2 = 1;
		
		HAL_UART_Receive_IT(&huart2, data_rx2,9);
		received_data2(data2);
		
	}
	  
}



/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

HAL_UART_Receive_IT(&huart1, data_rx, sizeof(data_rx));
HAL_UART_Receive_IT(&huart2, data_rx2, sizeof(data_rx2));


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		//	time_hoatdong = HAL_GetTick();
		
	
		
			if(uart_flag)
			{
				
				check_sum(data_rx, DATA_LEN);
					
				DataCommand(data_rx);
				countClear =0;
				//memset(data_rx2, 0x00, 9);
				uart_flag = 0;
				
				
			}
			
			  else if(uart_flag2 )
			{
					uart_flag2 = 0;
				DataCommand(data_rx2);
				

			}
			if(Working_count == 4)
			{
				timeCheck(&StartTime, data_rx);
			}
			}

	  
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
