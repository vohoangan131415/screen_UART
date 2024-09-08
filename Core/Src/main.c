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

/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
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
//setting define for P_M_M block 
#define Position_BIT 0xF8 //5 high bit 1111 1000
#define Map_BIT 0x04 // low bit thu 3   0000 0100
#define Mode_BIT 0x03 // 2 low bit cuoi 0000 0011
//setting define for Other block 
#define Odrive_BIT 0x02 // bit 1 (HIGH) 0000 0010
#define MapO_BIT 0x04 // bit 2 0000 0100
#define NguonDongLuc_BIT 0x08 // bit 3 0000 1000
// setting a place for output (PMM,OTher)
char buffer[100];
//setting for clear and start command
uint8_t send_clear[]={0x97, 0x00, 0x00,0x00,0x00, 0x00};
uint8_t send_start[]={0x98, 0x00,0x00,0x00,0x00,0x00};
////uart received setting
#define DATA_LEN 7
#define DATA_TRANS_LEN 3
static uint8_t data_rx[DATA_LEN];
static uint8_t uart_len = 0;
static uint8_t uart_flag = 0;
static uint8_t receive_flag = 0;
uint8_t data;
// setting define choice (transmitted data) 
#define TH0 8 // (value block = 0x00)
#define TH1 9 // (value block = 0x01)
#define TH2 10 // (value block = 0x02)
#define TH3 11 // (value block = 0x03)
uint8_t data_tx0[3] = {0x99, 0x00, 0x63}; // testcase 1
uint8_t data_tx1[3] = {0x99, 0x01, 0x64};	// testcase 2
uint8_t data_tx2[3] = {0x99, 0x02, 0x65};	// testcase 3
uint8_t data_tx3[3] = {0x99, 0x03, 0x66};	// testcase 4

//===========================VARIABLE===========================================

// transmit data signal (transmitted data)
uint8_t data_transmit_signal = 0;
uint8_t data_transmited_need_change_signal = 0;
// variable for choice(transmitted data)
uint8_t th0_Sent = 0; //decide if TH1 and Th2 can be sent
// variable for P_M_M signal variable (changing due to transmitted data)
uint8_t Th0_signal = 0;
uint8_t TH1_Signal = 0;
uint8_t TH2_Signal = 0;
// variable for error check 
uint8_t error_signal = 0;
//===========================DISPLAY===========================================
// status display 
uint8_t working[] = "WORKING\n";
uint8_t successful[] = "SUCCESSFUL\n";
uint8_t failed[] = "FAILED\n";
uint8_t error[] = "ERROR\n";
uint8_t closed_loop[] = "CLOSED_LOOP\n";
uint8_t idle[] = "IDLE\n";
uint8_t unknown_status[] = "UNKNOWN_STATUS";
uint8_t NA_status[]= "N/A\n";
//  checksum display
uint8_t chuadudulieu[]= "chua_du_du_lieu";
uint8_t dadudulieu[]= "da_du_du_lieu";
// error display
uint8_t errorAnnouce[30];
// Bit display (position)
uint8_t bit[30];
//===========================FUNCTION===========================================
// 											TABLE OF CONTENTS
//	RECEIVE AND STORE DATA (line 143)   (7 BLOCK)
//	STATUS DISPLAY(line 165)						(block 2,3)
//	ERROR ANNOUNCE (line 193)						(block 2,3)
//	STATUS SWITCHING (line 212) 				(block 2,3)
//	DISPLAY INITIAL STATUS (line 243) 	(block 2,3)
//	HANDLE PMM BLOCK(line 259)  				(block 4) 
//	HANDLE OTHER BLOCK (line 303)				(block 5)
//	CHECK SUM	(line 336)								(block 6)
//	command CLEAR (line 361)
// 	TRANSMITTED DATA (407)
// DISPLAY 7 block (line 439)
// CONTINUOUS DATA RECEIVE (line 470)

// FUNCTION FOR RECEIVE AND STORE DATA ====================================================================
void received_data(uint8_t data)
{
	if(data == 0x98 && uart_len == 0)
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
		}
	}
	receive_flag = 0;
		uart_flag = 0;
}
 //FUNCTION FOR STATUS DISPLAY ====================================================================
void status_display(uint8_t status1)
{
	switch(status1){
        
        case SUCCESSFUL:
            HAL_UART_Transmit(&huart1, successful, sizeof(successful), HAL_MAX_DELAY);
            break;
        case FAILED:
            HAL_UART_Transmit(&huart1, failed, sizeof(failed), HAL_MAX_DELAY);
            break;
        case WORKING:
            HAL_UART_Transmit(&huart1, working, sizeof(working), HAL_MAX_DELAY);
            break;
        case ERROR:
            HAL_UART_Transmit(&huart1, error, sizeof(error), HAL_MAX_DELAY);
            break;
        case CLOSED_LOOP:
            HAL_UART_Transmit(&huart1, closed_loop, sizeof(closed_loop), HAL_MAX_DELAY);
            break;
        case IDLE:
            HAL_UART_Transmit(&huart1, idle, sizeof(idle), HAL_MAX_DELAY);
            break;
        case UNKNOWN:
            HAL_UART_Transmit(&huart1, unknown_status, sizeof(unknown_status), HAL_MAX_DELAY);
            break;
}
}
// FUNCTION FOR ERROR ANNOUNCEMENT====================================================================
void check_errors(uint8_t *status, uint8_t size, int *index) {
    for (int i = 0; i < size; i++) {
        if (*status == 0x00) 
					{
						sprintf(errorAnnouce, "object %d: Error ", *index);
						HAL_UART_Transmit(&huart1, errorAnnouce, sizeof(errorAnnouce), HAL_MAX_DELAY);
					}
				// temporary fixing for transmitted data
//				else if(*status == 0x00 && Th0_signal)
//				{
//					sprintf(errorAnnouce, "object %d: N/A ", *index);
//          HAL_UART_Transmit(&huart1, errorAnnouce, sizeof(errorAnnouce), HAL_MAX_DELAY);
//				}
//    }
//		Th0_signal=0;
			error_signal =0;
			}
		}
// FUCNTION FOR STATUS SWITCHING=================================================
void status_changing(uint8_t *data_value)
{
	for(int i = 1; i < 7; i++)
		{
			if(i == 2)
				{
					if(*(data_value+i) == 0x01)
					{
					status_display(IDLE);
					}
					else
					{
					status_display(CLOSED_LOOP);
					}
				}
			else
			{
				if(*(data_value+i) == 0x01)
				{
					status_display(WORKING);
				}
				else if(*(data_value+i) == 0x00)
				{
					status_display(ERROR);
					error_signal = 1;					
				}
			} 
			check_errors(data_rx + i, 1, &i);
		}
	}
// FUNCTION FOR DISPLAY INITIAL STATUS ============================================
void Starting(uint8_t *data)
{
		for(int i = 1; i < 7; i++)
		{
			
			if(data[i] == 0x01)
			{
				status_display(SUCCESSFUL);
			}
			else if(data[i] == 0x00)
			{
				status_display(FAILED);
			}			
		}
}
// FUNCTION FOR HANDLE PMM BLOCK ==================================================
// FUNCTION FOR specific BIT (PMM, OTHER) (thu tu cua bit)
uint8_t check_bit(uint8_t value, uint8_t bit_position) {
    return (value >> bit_position) & 0x01;
}
// FUNCTION FOR Position BIT (position in PMM) 
uint8_t check_bit_position(uint8_t value, uint8_t bit_position) {
    return (value >> bit_position) & 0x1F;
}
void PMM(uint8_t *data_value4){ // 4 represents for the position of the bit
	uint8_t Position = check_bit_position((*(data_value4 + 4) & Position_BIT), 3);
	sprintf(bit, "Bit = %d", Position);
	HAL_UART_Transmit(&huart1, bit , sizeof(bit), HAL_MAX_DELAY);
	sprintf(buffer, "Position = %d\n", Position);
	uint8_t Map = check_bit((*(data_value4 + 4) & Map_BIT), 2);
	if(Map == 0x01)
		{
			strcat(buffer, "Map = San 1\n");
		}
	if(Map == 0x00)
		{
			strcat(buffer, "Map = San 2\n");
		}
	uint8_t Mode = check_bit((*(data_value4 + 4) & Mode_BIT), 0);
	if(Mode == 0x00)
	{
		strcat(buffer, "Mode: Auto \n");
	}
	else if(Mode == 0x01)
	{
		strcat(buffer, "Mode: Manual \n");
	}
	else if(Mode == 0x03)
	{
		strcat(buffer, "Mode: No Mode \n");
	}
// temporary fixing for transmitted data
//	else if(TH1_Signal &&( Mode == 0x00 || Mode == 0x01|| Mode == 0x03))
//	{
//		strcat(buffer, "Mode: Manual \n");
//	}
	HAL_UART_Transmit(&huart1, buffer, strlen(buffer), HAL_MAX_DELAY);
}
// FUNCTION FOR HANDLE OTHER BLOCK==================================================
void Other(uint8_t *data_value5) // 5 represents for the position of the bit
{
	uint8_t Odrive_State = check_bit((*(data_value5 + 5) & Odrive_BIT), 1);
	if(Odrive_State == 0x0)
	{
		strcat(buffer, "Odrive_State: IDLE \n");
	}
	else
	{
		strcat(buffer, "Odrive_State: CLOSED_LOOP \n");
	}
	uint8_t MapO = check_bit((*(data_value5 + 5) & MapO_BIT), 2);
	if(MapO == 0x0)
	{
		strcat(buffer, "MapO: Right \n");
	}
	else
	{
		strcat(buffer, "MapO: Left \n");
	}
	uint8_t NguonDongLuc = check_bit((*(data_value5 + 5) & NguonDongLuc_BIT), 3);
	if(NguonDongLuc == 0x0)
		{
			strcat(buffer, "NguonDongLuc = Error");
		}
	else
		{
			strcat(buffer, "NguonDongLuc = Working");
		}
		HAL_UART_Transmit(&huart1, buffer, strlen(buffer), HAL_MAX_DELAY);
		
}
// FUNCTION FOR CHECK SUM ==================================================
uint8_t calculate_checksum(uint8_t *data, size_t length) // calculate checksum
{ 
    uint8_t checksum = 0;
    for (size_t i = 0; i < length; i++) {
        checksum += data[i];
    }
    return checksum;
		
}
void check_sum(uint8_t *data, uint8_t length) // compare checksum
{

		uint8_t received_checksum = data[length - 1];
		uint8_t calculated_checksum = calculate_checksum(data, length - 1 );
		if(calculated_checksum == received_checksum)
		{
				HAL_UART_Transmit(&huart1,dadudulieu, sizeof(dadudulieu), HAL_MAX_DELAY );
		}
		else
		{
			HAL_UART_Transmit(&huart1,chuadudulieu, sizeof(chuadudulieu), HAL_MAX_DELAY );
		}
}

//Function for command CLEAR==================================================
void Reset_data()
{
	HAL_UART_Transmit(&huart1, send_clear, sizeof(send_clear), HAL_MAX_DELAY);
	 for (int i = 0; i < DATA_LEN; i++) 
			{
					data_rx[i] = 0x00; // reset all values
				HAL_UART_Transmit(&huart1, NA_status, sizeof(NA_status), HAL_MAX_DELAY);
			}
			uart_len = 0;
			uart_flag = 0;
			receive_flag = 0;
			error_signal = 0;
}

//========================================================================================
//																			TRANSMITTED DATA
// CHOICE FOR 0x00, 0x01, 0x02, 0x03
void choice_selection_data_tx(uint8_t choice)
{
	switch(choice)
	{
		case TH0: 
				th0_Sent = 1;
				HAL_UART_Transmit(&huart1, data_tx1, sizeof(data_tx1), HAL_MAX_DELAY);
				Th0_signal = 1;
			break;
		case TH1: 
			if(th0_Sent)
			{
				HAL_UART_Transmit(&huart1, data_tx1, sizeof(data_tx1), HAL_MAX_DELAY);
				TH1_Signal = 1;
			}
			break;
		case TH2: 
			if(th0_Sent)
			{
				HAL_UART_Transmit(&huart1, data_tx2, sizeof(data_tx2), HAL_MAX_DELAY);
				TH2_Signal = 1;
			}
			break;
//		case TH3: 
//			break;
		
	}
}
//FUNCTION FOR value block (0x01 and 0x02)
void closedLoop_clearError(uint8_t *data_send, uint8_t *data_rec)
{
	
		for(int j = 0; j < 3; j++)
			if(j == 1 && Th0_signal)
			{
				Starting(data_rec);
				status_changing(data_rec);
				if(data_send[j] == 0x01)
					{
						TH1_Signal = 1;
					}
					if(data_send[j] == 0x02)
					{
						TH2_Signal = 1;
						Reset_data();
					}
			}
	
}
//FUNCTION FOR value block (0x03) 
void homing_swereve()
{};
//FUNCTION FOR value block (0x00)
void transmitted_data_handle(uint8_t *data_sending, uint8_t *data_rec, uint8_t length1)
{
	
				check_sum(data_rec, length1);
				data_transmit_signal = 1;
				closedLoop_clearError(data_sending, data_rec);
	
}
//===================================================================================================================
// FUNCTION FOR DISPLAY 7 block, value of PMM, OTHER, status, check sum ==================================================
void package_display(uint8_t *data_rec, uint8_t length)
{
		Starting(data_rec);
		status_changing(data_rec);
		PMM(data_rec);
		Other(data_rec);
		check_sum(data_rec, DATA_LEN);
//		temporary fix for the transmitted data
//		if(Th0_signal == 1)
//		{
//		transmitted_data_handle(data_tx0, data_rec, DATA_TRANS_LEN);
//		}
//		
//		if(TH1_Signal == 1)
//		{
//		transmitted_data_handle(data_tx1, data_rec, DATA_TRANS_LEN);
//		}
//		TH1_Signal=0;
//		if(TH2_Signal == 1)
//		{
//		transmitted_data_handle(data_tx2, data_rec, DATA_TRANS_LEN);
//		}
//		TH2_Signal = 0;
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
  /* USER CODE BEGIN 2 */
	 HAL_UART_Receive_IT(&huart1, data_rx, sizeof(data_rx));
	Reset_data();
//choice_selection_data_tx(TH0);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
  while (1)
  {
    /* USER CODE END WHILE */

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
  huart1.Init.BaudRate = 9600;
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
