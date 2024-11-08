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
#define TASK 6
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#if TASK == 1
#include "led_config.h"
#endif
#if TASK == 2
#include "led_config.h"
#endif
#if TASK == 3
#include <stdio.h>
#include "led_config.h"
#endif
#if TASK == 4
#include <stdio.h>
#include "led_config.h"
#endif
#if TASK == 5
#include "encoder_config.h"
#endif
#if TASK == 6
#include <stdio.h>
#include "encoder_config.h"
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
#if TASK == 2
float duty_cycle = 0.0f;
#endif
#if TASK == 5
unsigned int encoder_counter = 0;
#endif
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_USART3_UART_Init();
  MX_TIM4_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
	#if TASK == 1
  LED_RGB_PWM_Init(&hldrgb);
  LED_PWM_WriteDuty(hldrgb.R, 50.0f);
  #endif
  #if TASK == 2
  LED_RGB_PWM_Init(&hldrgb);
  #endif
  #if TASK == 3
  LED_RGB_PWM_Init(&hldrgb);
  #endif
  #if TASK == 4
  LED_RGB_PWM_Init(&hldrgb);
  #endif
  #if TASK == 5
  ENC_Init(&henc1);
  #endif
  #if TASK == 6
  ENC_Init(&henc1);
  #endif
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    #if TASK == 2
    LED_PWM_WriteDuty(hldrgb.R, duty_cycle);
    duty_cycle = (duty_cycle < 100.0f) ? (duty_cycle + 10.0f) : 0.0f;
    HAL_Delay(499);
    #endif
    #if TASK == 3
    uint8_t RxData_DutyCycle[] = "X000";
    if(HAL_UART_Receive(&huart3, RxData_DutyCycle, sizeof(RxData_DutyCycle) - 1, HAL_MAX_DELAY) == HAL_OK)
    {
      char Channel = '\0';
      unsigned int DutyCycle = 0;
      if(sscanf((char*)RxData_DutyCycle, "%c%d", &Channel, &DutyCycle) == 2)
      {
        if(Channel == 'R')
          LED_PWM_WriteDuty(hldrgb.R, DutyCycle);
      }
    }
    #endif
    #if TASK == 4
    uint8_t RxData_DutyCycle[] = "X000";
    if(HAL_UART_Receive(&huart3, RxData_DutyCycle, sizeof(RxData_DutyCycle) - 1, HAL_MAX_DELAY) == HAL_OK)
    {
      char Channel = '\0';
      unsigned int DutyCycle = 0;
      if(sscanf((char*)RxData_DutyCycle, "%c%d", &Channel, &DutyCycle) == 2)
      {
        switch(Channel)
        {
          case 'R':
            LED_PWM_WriteDuty(hldrgb.R, DutyCycle);
            break;
          case 'G':
            LED_PWM_WriteDuty(hldrgb.G, DutyCycle);
            break;
          case 'B':
            LED_PWM_WriteDuty(hldrgb.B, DutyCycle);
            break;
          default: break;
        }
      }
    }
    #endif
    #if TASK == 5
    encoder_counter = ENC_ReadCounter(&henc1);
    HAL_Delay(0);
    #endif
    #if TASK == 6
    unsigned char TxData_EncoderCounter[128];
    int TxData_EncoderCounter_Len = snprintf((char*)TxData_EncoderCounter, sizeof(TxData_EncoderCounter), "Encoder counter: %lu\r\n", ENC_ReadCounter(&henc1));
    HAL_UART_Transmit(&huart3, TxData_EncoderCounter, TxData_EncoderCounter_Len, 100);
    HAL_Delay(999);
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
