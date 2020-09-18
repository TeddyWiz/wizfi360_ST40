/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "COM_Queue.h"
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
UART_HandleTypeDef huart6;

/* USER CODE BEGIN PV */
COM_Buffer UART_1;
COM_Buffer UART_2;
COM_Buffer UART_6;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART6_UART_Init(void);
/* USER CODE BEGIN PFP */

 #ifdef KEIL
      #ifdef __GNUC__
      //With GCC, small printf (option LD Linker->Libraries->Small printf
      //set to 'Yes') calls __io_putchar()
         #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
	  #else
		 #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
	  #endif /* __GNUC__*/
    #if 1
    PUTCHAR_PROTOTYPE
    {
//	  HAL_UART_Transmit(&huart6, (uint8_t *)&ch, 1, 0xFFFF);
	  HAL_UART_Transmit(&huart6, (uint8_t *)&ch, 1, 0xFFFF);
      return ch;
    }
	
		int ferror(FILE *f)
	{
	  /* Your implementation of ferror(). */
	  return 0;
	}
    #endif
  #endif			 

  #ifdef True_STD
  int _write(int fd, char *str, int len)
  {
    for(int i=0; i<len; i++)
    {
      HAL_UART_Transmit(&huart1, (uint8_t *)&str[i], 1, 0xFFFF);
    }
    return len;
  }
 #endif
	
	
#define UART_RX_BUF_SIZE    1024
//volatile uint8_t data;
//volatile uint8_t data6;	
uint8_t RxBuf1[UART_RX_BUF_SIZE];
uint8_t RxBuf2[UART_RX_BUF_SIZE];
volatile uint8_t uart_buffer[UART_RX_BUF_SIZE+1];
volatile int  uart_index=0;
 uint8_t *rxBuf;
  uint8_t test[4] = "TEST";
uint8_t flag_rx = 0;
uint8_t i= 0;


typedef struct 
{
  uint8_t buffer[UART_RX_BUF_SIZE];
  uint8_t temp;
  uint8_t rxd;
  volatile uint16_t i_p;
  volatile uint16_t o_p;
} uart_rx_type;
uart_rx_type uart_hal_rx;
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void uart_hal_rx_buffer_init(void)
{
  uart_hal_rx.i_p = 0;
  uart_hal_rx.o_p =0;

}
#if 1
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	int size=0;
	if(huart->Instance == USART6){
	//	HAL_UART_Receive_IT(&huart1, (uint8_t*) &data, 2);
		uart_hal_rx.buffer[uart_hal_rx.i_p++] = uart_hal_rx.temp;
    
    if(uart_hal_rx.i_p >=UART_RX_BUF_SIZE)
    {
      uart_hal_rx.i_p=0;
    }
    HAL_UART_Receive_IT(&huart6,&uart_hal_rx.temp,1);
     //   uart_buffer[uart_index] = '\0';
		 if(uart_hal_rx.temp == '\n') {
			 size = uart_hal_rx.i_p - uart_hal_rx.o_p;
			 if(size <0)
				  size= UART_RX_BUF_SIZE-uart_hal_rx.i_p;
      //for(i = 0;i<size;i++){

      HAL_UART_Transmit_IT(&huart1, &uart_hal_rx.buffer[uart_hal_rx.o_p], size);
      uart_hal_rx.o_p+=size;
      if(uart_hal_rx.o_p >=UART_RX_BUF_SIZE)
      {
        uart_hal_rx.o_p=0;
      }

      // uart_hal_rx.o_p+size;
      // if(uart_hal_rx.o_p >=UART_RX_BUF_SIZE)
      // {
      //   uart_hal_rx.o_p-=UART_RX_BUF_SIZE;
      // }
    //  }
      uart_index = 0;
    
    
	 }
    HAL_GPIO_TogglePin(GPIOB,LED1_Pin);
	}
	
}
#else
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	flag_rx = 1;
	   if(huart->Instance == huart1.Instance){
			HAL_UART_Receive_IT(&huart1,rxBuf,5);
		    *rxBuf =0;
				HAL_GPIO_TogglePin(GPIOB,LED1_Pin);
	   }
}

#endif


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	 //char *test ="AT\r\n";
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
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_GPIO_WritePin(W_RST_GPIO_Port,W_RST_Pin,0);
  HAL_Delay(1000);
  HAL_GPIO_WritePin(W_RST_GPIO_Port,W_RST_Pin,1);
  
  HAL_NVIC_EnableIRQ(USART1_IRQn);
  HAL_NVIC_EnableIRQ(USART6_IRQn);
  HAL_UART_Receive_IT(&huart1,&uart_hal_rx.temp,1);
  HAL_UART_Receive_IT(&huart6,&uart_hal_rx.temp,1);
  uart_hal_rx_buffer_init();
  printf("AT+GMR\r\n");
  HAL_Delay(1000);
  printf("AT+CWMODE_CUR=1\r\n");
  printf("AT+CIPMUX=0\r\n");
  HAL_Delay(1000);
  printf("AT+CWDHCP_CUR=1,1\r\n");
  HAL_Delay(1000);
  printf("AT+CWLAP\r\n");
  HAL_Delay(4000);
  printf("AT+CWJAP_CUR=\"Teddy_AP\",\"123456789\"\r\n");
  HAL_Delay(5000);
  printf("AT+CIPSTART=\"TCP\",\"192.168.0.9\",5000 \r\n");
  HAL_Delay(1000);

//  
//  HAL_UART_Transmit(&huart6, (uint8_t *)&test, 4, 0xFFFF);
  HAL_GPIO_TogglePin(GPIOB,LED0_Pin);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  huart2.Init.HwFlowCtl = UART_HWCONTROL_RTS;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED0_Pin|LED1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : W_RST_Pin */
  GPIO_InitStruct.Pin = W_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(W_RST_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED0_Pin LED1_Pin */
  GPIO_InitStruct.Pin = LED0_Pin|LED1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void UART1_enqueue(char data)
{
	COM_Enqueue(&UART_1, data);
}
void UART2_enqueue(char data)
{
	COM_Enqueue(&UART_2, data);
}
void UART6_enqueue(char data)
{
	COM_Enqueue(&UART_6, data);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
