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
//setting define for Other block 
#define Odrive_BIT 0x80 // bit 1 (HIGH) 0000 0010
#define NguonDongLuc_BIT 0x40 // bit 2 0000 0100
// setting a place for output (PMM,OTher)
//setting for clear and start command
uint8_t send_clear[]={0x97, 0x00, 0x00,0x00,0x00, 0x00};
uint8_t send_start[]={0x98, 0x00,0x00,0x00,0x00,0x00};
////uart received setting
#define DATA_LEN 6
#define DATA_LEN2 8
#define DATA_TRANS_LEN 3
uint8_t data_rx[6];
uint8_t uart_len = 0;
uint8_t uart_flag = 0;
uint8_t receive_flag = 0;

static uint8_t data_rx2[DATA_LEN2];
static uint8_t uart_len2 = 0;
static uint8_t uart_flag2 = 0;
static uint8_t receive_flag2 = 0;
uint8_t data ;
uint8_t data2;
uint8_t send_data[8] = {0x5a, 0xa5, 0x05, 0x82, 0x61, 0x00, 0x00, 0x01} ; 
uint8_t send[8] = {0x5a, 0xa5, 0x05, 0x82, 0x62, 0x00, 0x00, 0x02} ; 
#define nochoice 12 // (initial value)
uint8_t data_tx0[3] = {0x99, 0x00, 0x99}; // testcase 1
uint8_t data_tx1[3] = {0x99, 0x01, 0x9A};	// testcase 2
uint8_t data_tx2[3] = {0x99, 0x02, 0x9B};	// testcase 3
uint8_t data_tx3[3] = {0x99, 0x03, 0x9C};	// testcase 4


//===========================VARIABLE===========================================

// transmit data signal (transmitted data)
#define Th0 8
#define Th1 9
#define Th2 10
#define Th3 11
// variable for choice(transmitted data)
uint8_t th0_Sent = 0; //decide if TH1 and Th2 can be sent
uint8_t Data_enough_Sent = 0;
uint8_t th3_sent = 0;
// variable for error check 
uint8_t error_signal = 0;

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
void received_data(uint8_t data)
{
	if(data == 0x98 && uart_len == 0)
	
	{
		receive_flag = 1;
	}
	
	if(receive_flag)
	{
		if(uart_len == 65)
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
	uart_flag = 0;
}

void received_data2(uint8_t data2)
{
	if(data2 == 0x5A && uart_len2 == 0)
	{
		receive_flag2= 1;
	}
	if(receive_flag2)
	{
		if(uart_len2 == 7)
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
		uart_flag2 = 0;
}
 //FUNCTION FOR STATUS DISPLAY ====================================================================
typedef struct
{
	
	uint8_t working[12];
	uint8_t successful[8];
	uint8_t failed[11];
	uint8_t error[10];
	uint8_t closed_loop[16];
	uint8_t idle[9];
	uint8_t unknown_status[12];
	uint8_t notEnough[14];
	uint8_t Enough[11];
	uint8_t NAstatus[7];
}displayContentstatus;
displayContentstatus status_dis_store;
int address;
void addressConfirm(uint8_t index)
{
		
		switch(index)
		{
			case 1:
				address = 0x60; // SenSor state
				break;
			case 2:
				address = 0x61;  // Odrive state (error/working)
				break;
			case 3:
				address = 0x62;  // device state
				break;
			case 4:
				address = 0x63;   //other (Odrive state idle/close loop)
				break;
			case 5:
				address = 0x64;   //other (NguonDongLuc)
				break;
			case 6: 
				address = 0x65;   // checksum
				break;
		}
	
}
displayContentstatus create_displaystatus()
{
	
	displayContentstatus status = {
												{0x5A, 0xA5, 0x12, 0x82, 0x63, 0x57, 0x4F, 0x52, 0x4B, 0x49, 0x4E, 0x47}, //WORKING, HUM
											//	{0x5A, 0xA5, 0x15, 0x82, 0x64,0x53, 0x55, 0x43, 0x43, 0x45, 0x53, 0x53, 0x46, 0x55, 0x4C}, // successful,tem 
													{0x5a, 0xa5, 0x05, 0x82, 0x61, 0x00, 0x00, 0x01},
												{0x5A, 0xA5, 0x11, 0x82, 0x65,0x46, 0x41, 0x49, 0x4C, 0x45, 0x44}, // FAILED, Pres
												{0x5A, 0xA5, 0x10, 0x82, 0x66, 0x45, 0x52, 0x52, 0x4F, 0x52},// ERROR, Dew
												{0x5A, 0xA5, 0x16, 0x82, address,0x43, 0x4C, 0x4F, 0x53, 0x45, 0x44, 0x5F, 0x4C, 0x4F, 0x4F, 0x50}, // Closed_loop
												{0x5A, 0xA5, 0x9, 0x82, address ,0x49, 0x44, 0x4C, 0x45},// IDLE
												{0x5A, 0xA5, 0x12, 0x82, address,0x55, 0x4E, 0x4B, 0x4E, 0x4F, 0x57, 0x4E},// UNKNOWN
												{0x5A, 0xA5, 0x14, 0x82, 0x66,0x6E, 0x6F, 0x74, 0x45, 0x6E, 0x6F, 0x75, 0x67, 0x68},// chua du du lieu ( address #)
												{0x5A, 0xA5, 0x11, 0x82, 0x66,0x45, 0x6E, 0x6F, 0x75, 0x67, 0x68},//  du du lieu ( address #)
												{0x5A, 0xA5, 0x7, 0x82, address,0x4E,0x41}    //  NA 
											};
				return status;
}



void status_display(uint8_t status1)
{
	status_dis_store = create_displaystatus();
	switch(status1){
        
        case SUCCESSFUL:
            //HAL_UART_Transmit(&huart2, status_dis_store.successful, sizeof(status_dis_store.successful), HAL_MAX_DELAY);
						HAL_UART_Transmit(&huart2, send_data, 8, HAL_MAX_DELAY);
            break;
        case FAILED:
            HAL_UART_Transmit(&huart2, status_dis_store.failed, sizeof(status_dis_store.failed), HAL_MAX_DELAY);
            break;
        case WORKING:
            //HAL_UART_Transmit(&huart2, status_dis_store.working, sizeof(status_dis_store.working), HAL_MAX_DELAY);
            HAL_UART_Transmit(&huart2, send, 8, HAL_MAX_DELAY);
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
	}	
	}


// FUCNTION FOR STATUS SWITCHING=================================================
uint8_t statuschangeSignal = 0;
uint8_t newlocationlist = 0;
void handle_new_data(uint8_t *new_data)
{
	 for(int j = 1; j < 6; j++)
    {
				addressConfirm(j);
        if(new_data[j] == 0x00)
        {
						status_display(ERROR);
				}
				
            // Ng?ng ki?m tra ngay khi phát hi?n l?i
        
    }
}
void status_changing(uint8_t *data_value)
{
	
	for(int i = 1; i < 6; i++)
		{
				addressConfirm(i);
				if(*(data_value+i) == 0x01)
				{
					status_display(WORKING);
				}
				   
		}
}

// FUNCTION FOR DISPLAY INITIAL STATUS ============================================

void Starting(uint8_t *data)
{
		uint8_t all_successful = 1;
		
		for(int i = 1; i < 6; i++)
		{
			addressConfirm(i);
			if(data[i] == 0x01)
			{
				status_display(SUCCESSFUL);
			}
			else if(data[i] == 0x00)
			{
				all_successful = 0;
				status_display(FAILED);
				error_signal = 1;
			}			
			 
		}
		if(all_successful)
			{
				statuschangeSignal = 1;
				status_changing(data);
			}
		if(statuschangeSignal)
			{
				handle_new_data(data);
			}
		
}
//// FUNCTION FOR specific BIT (PMM, OTHER) (thu tu cua bit)
uint8_t check_bit(uint8_t value, uint8_t bit_position) {
    return (value >> bit_position) & 0x01;
}
//// FUNCTION FOR HANDLE OTHER BLOCK==================================================

void Other(uint8_t *data_value5) // 5 represents for the position of the bit
{
	for(int i = 0; i < 6; i++)
	{
		addressConfirm(i);
	if(i == 2)
	{
			uint8_t Odrive_State = check_bit((*(data_value5 + 4) & Odrive_BIT), 7);
			if(Odrive_State == 0x00)
			{
				
				status_display(IDLE);
			}
			else if(Odrive_State == 0x01)
			{
				status_display(CLOSED_LOOP);
			}
	}
	}
	uint8_t NguonDongLuc = check_bit((*(data_value5 + 4) & NguonDongLuc_BIT), 6);
			if(NguonDongLuc == 0x00)
			{
				status_display(ERROR);
			}
			else if(NguonDongLuc == 0x01)
			{
				status_display(WORKING);
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
				HAL_UART_Transmit(&huart1, status_dis_store.Enough, sizeof(status_dis_store.Enough), HAL_MAX_DELAY);
				
		}
		else
		{
			HAL_UART_Transmit(&huart1,status_dis_store.notEnough, sizeof(status_dis_store.notEnough), HAL_MAX_DELAY );
		}
}
void check_sum2(uint8_t *data, uint8_t length) // compare checksum (transmit)
{
		status_dis_store = create_displaystatus();
		uint8_t received_transmitted_checksum = data[length - 1];
		uint8_t calculated_checksum = calculate_checksum2(data, length);
		if(calculated_checksum == received_transmitted_checksum)
		{
				Data_enough_Sent = 1;
				HAL_UART_Transmit(&huart2, status_dis_store.Enough, sizeof(status_dis_store.Enough), HAL_MAX_DELAY);
				
		}
		else if(calculated_checksum == received_transmitted_checksum && th3_sent)
		{
			Data_enough_Sent = 1;
			th3_sent = 0;
			HAL_UART_Transmit(&huart2,status_dis_store.Enough, sizeof(status_dis_store.Enough), HAL_MAX_DELAY );
		}
		else
		{
			HAL_UART_Transmit(&huart2,status_dis_store.notEnough, sizeof(status_dis_store.notEnough), HAL_MAX_DELAY );
		}
}

////Function for command CLEAR==================================================
void Reset_data()
{	
			HAL_UART_Transmit(&huart1, send_clear, sizeof(send_clear), HAL_MAX_DELAY);
			for(int i = 0; i < DATA_LEN; i++) 
			{
					data_rx[i] = 0x00; // reset all values
				HAL_UART_Transmit(&huart1, status_dis_store.NAstatus, sizeof(status_dis_store.NAstatus), HAL_MAX_DELAY);
			}
			uart_len = 0;
			uart_flag = 0;
			receive_flag = 0;
			error_signal = 0;
		}

////========================================================================================
////																			TRANSMITTED DATA
// CHOICE FOR 0x00, 0x01, 0x02, 0x03
void choice_selection_data_tx(uint8_t choice, uint8_t *data2)
{
	switch(choice)
	{
		case Th0: 
				check_sum2(data_rx2,DATA_LEN2);
				if(Data_enough_Sent)
					{
							HAL_UART_Transmit(&huart2, data_tx0, sizeof(data_tx0), HAL_MAX_DELAY);
							Data_enough_Sent = 0;
					}
					th0_Sent = 1;
			break;
		case Th1: 
					check_sum2(data_rx2,DATA_LEN2);
					if(Data_enough_Sent && th0_Sent)
					{
							HAL_UART_Transmit(&huart2, data_tx1, sizeof(data_tx1), HAL_MAX_DELAY);
					}
			break;
		case Th2: 
					check_sum2(data_rx2,DATA_LEN2);
					if(Data_enough_Sent && th0_Sent)
					{
							HAL_UART_Transmit(&huart2, data_tx2, sizeof(data_tx2), HAL_MAX_DELAY);
							Data_enough_Sent = 0;
					}
			break;
			case Th3: 
					check_sum2(data_rx2,DATA_LEN2);
					if(Data_enough_Sent)
					{
							HAL_UART_Transmit(&huart2, data_tx3, sizeof(data_tx3), HAL_MAX_DELAY);
							Data_enough_Sent = 0;
					}
			break;
			
	}
}
//===================================================================================================================
// FUNCTION FOR DISPLAY 7 block, value of PMM, OTHER, status, check sum ==================================================
void package_display(uint8_t *data_7Block, uint8_t length)
{
			
			Starting(data_7Block);
			//Other(data_7Block);
			//check_sum(data_7Block, DATA_LEN);
	
}
void package_sending(uint8_t *data2)
{
	for(int i = 0; i < 8; i++)
	{
		if(i == 6)
		{
			if(data2[i] == 0x0){
			choice_selection_data_tx(Th0,data2);
			}
			else if(data2[i] == 0x01 && th0_Sent){
			choice_selection_data_tx(Th1,data2);}
			else if(data2[i] == 0x02 && th0_Sent){
			choice_selection_data_tx(Th2,data2);}
			else if(data2[i] == 0x03 ){
			choice_selection_data_tx(Th3,data2);}
		}
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
		package_display(data_rx, DATA_LEN);
	}
/*	 if(huart->Instance == huart2.Instance)
	{
		uart_flag2 = 1;
		HAL_UART_Receive_IT(&huart2, data_rx2, sizeof(data_rx2));
		received_data2(data2);
		package_sending(data_rx2);
	} */
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
	//HAL_UART_Transmit(&huart2, send_data, 8, HAL_MAX_DELAY);
    /* USER CODE BEGIN 3 */
//		Starting_phase();
		
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
