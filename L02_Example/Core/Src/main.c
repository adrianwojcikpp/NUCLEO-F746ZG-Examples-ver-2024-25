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
#define TASK 7
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#if TASK == 1
#include "led_config.h"
#endif
#if TASK == 2
#include <stdio.h>
#include "btn_config.h"
#include "led_config.h"
#endif
#if TASK == 3
#include "led_config.h"
#endif
#if TASK == 4
#include "led_config.h"
#endif
#if TASK == 5
#include "serial_api_config.h"
#endif
#if TASK == 6
#include "serial_api_config.h"
#endif
#if TASK == 7
#include <stdio.h>
#include "led_config.h"
#include "serial_api_config.h"
#endif
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

/* USER CODE BEGIN PV */
#if TASK == 4
unsigned char Message;
#endif
#if TASK == 5
unsigned char Message[SERIAL_API_LED_MSG_LEN+1];
#endif
#if TASK == 6
unsigned int MessageIndex = 0;
unsigned char Message[SERIAL_API_LED_MSG_LEN+1];
_Bool MessageReceived = 0;
#endif
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#if TASK == 4
/**
  * @brief Tx Transfer completed callback.
  * @param huart UART handle.
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart == &huart3)
  {
    LED_DIO_Off(&hld1);
    HAL_UART_Receive_IT(&huart3, &Message, 1);
  }
}

/**
  * @brief  Rx Transfer completed callback.
  * @param  huart UART handle.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart == &huart3)
  {
    LED_DIO_On(&hld1);
    HAL_UART_Transmit_IT(&huart3, &Message, 1);
  }
}
#endif
#if TASK == 5
/**
  * @brief  Rx Transfer completed callback.
  * @param  huart UART handle.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart == &huart3)
  {
    unsigned int hleds_n = sizeof(hleds) / sizeof(hleds[0]);
    SERIAL_API_LED_ReadMsg((char*)Message, hleds, hleds_n);
    for(int i = 0; i < hleds_n; i++)
      LED_DIO_Write(hleds[i].Led, hleds[i].State);
    HAL_UART_Receive_IT(&huart3, Message, SERIAL_API_LED_MSG_LEN);
  }
}
#endif
#if TASK == 6
/**
  * @brief  Rx Transfer completed callback.
  * @param  huart UART handle.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart == &huart3)
  {
    MessageIndex++;
    if(MessageIndex == SERIAL_API_LED_MSG_LEN || Message[MessageIndex-1] == '\r')
    {
      MessageReceived = 1;
      MessageIndex = 0;
    }
    else
      HAL_UART_Receive_IT(&huart3, &Message[MessageIndex], 1);
  }
}
#endif
#if TASK == 7
int _write(int file, char *ptr, int len)
{
  return (HAL_UART_Transmit(&huart3, (uint8_t*)ptr, len, HAL_MAX_DELAY) == HAL_OK) ? len : -1;
}

int _read(int file, char *ptr, int len)
{
  return (HAL_UART_Receive(&huart3, (uint8_t*)ptr, len, HAL_MAX_DELAY) == HAL_OK) ? len : -1;
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
  #if TASK == 7
  setvbuf(stdin, NULL, _IONBF, 0);
  #endif
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
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  #if TASK == 1
  unsigned char Message[] = "Hello, Nucleo!\r\n";
  HAL_StatusTypeDef USART3_TxStatus = HAL_UART_Transmit(&huart3, Message, sizeof(Message) - 1, 10);
  if(USART3_TxStatus == HAL_OK)
    LED_DIO_On(&hld1);
  else
    LED_DIO_On(&hld3);
  #endif
  #if TASK == 4
  HAL_UART_Receive_IT(&huart3, &Message, 1);
  #endif
  #if TASK == 5
  HAL_UART_Receive_IT(&huart3, Message, SERIAL_API_LED_MSG_LEN);
  #endif
  #if TASK == 6
  HAL_UART_Receive_IT(&huart3, &Message[MessageIndex], 1);
  #endif
  #if TASK == 7
  puts("Hello, Nucleo!");
  #endif
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    #if TASK == 2
    if(BTN_DIO_EdgeDetected(&husrbtn) == BTN_PRESSED_EDGE)
    {
      static unsigned int USER_Btn_Cnt = 0;
      USER_Btn_Cnt++;

      unsigned char Message[32];
      int MessageLen = snprintf((char*)Message, sizeof(Message), "Hello, Nucleo: #%d\r\n", USER_Btn_Cnt);

      HAL_StatusTypeDef USART3_TxStatus = HAL_UART_Transmit(&huart3, Message, MessageLen, 10);

      LED_DIO_AllOff();
      if(USART3_TxStatus == HAL_OK)
        LED_DIO_On(&hld1);
      else
        LED_DIO_On(&hld3);
    }
    HAL_Delay(50);
    #endif
    #if TASK == 3
    unsigned char Message = '\0';
    HAL_StatusTypeDef USART3_RxStatus = HAL_UART_Receive(&huart3, &Message, 1, 100);
    LED_DIO_AllOff();

    if(USART3_RxStatus == HAL_OK)
    {
      LED_DIO_On(&hld1);
      HAL_UART_Transmit(&huart3, &Message, 1, 10);
    }
    else if(USART3_RxStatus == HAL_TIMEOUT)
      LED_DIO_On(&hld2);
    else
      LED_DIO_On(&hld3);
    #endif
    #if TASK == 6
    if(MessageReceived)
    {
      MessageReceived = 0;
      unsigned int hleds_n = sizeof(hleds) / sizeof(hleds[0]);
      SERIAL_API_LED_ReadMsg((char*)Message, hleds, hleds_n);
      for(int i = 0; i < hleds_n; i++)
        LED_DIO_Write(hleds[i].Led, hleds[i].State);
      HAL_UART_Receive_IT(&huart3, &Message[MessageIndex], 1);
    }
    #endif
    #if TASK == 7
    char Message[SERIAL_API_LED_MSG_LEN+2]; // CR + Null character
    int ReadoutStatus = scanf("%s",  Message);
    if(ReadoutStatus == 1)
    {
      unsigned int hleds_n = sizeof(hleds) / sizeof(hleds[0]);
      SERIAL_API_LED_ReadMsg((char*)Message, hleds, hleds_n);
      for(int i = 0; i < hleds_n; i++)
        LED_DIO_Write(hleds[i].Led, hleds[i].State);
    }
    #endif
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

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
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
