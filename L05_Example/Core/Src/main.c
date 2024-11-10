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
#define TASK 3
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#if TASK == 3
#include "bh1750_config.h"
#endif
#if TASK == 4
#include <stdio.h>
#include "bh1750_config.h"
#endif
#if TASK == 5
#include <stdio.h>
#include "led_config.h"
#include "bh1750_config.h"
#endif
#if TASK == 7
#include <stdio.h>
#include "lamp_config.h"
#include "bh1750_config.h"
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
#if TASK == 3
int illuminance_mlx = 0;
#endif
#if TASK == 4
int illuminance_mlx = 0;
#endif
#if TASK == 5
int duty_cycle;
int illuminance_mlx = 0;
#endif
#if TASK == 7
int input_value;
int illuminance_mlx = 0;
#endif
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#if TASK == 4
int _write(int file, char *ptr, int len)
{
  return (HAL_UART_Transmit(&huart3, (uint8_t*)ptr, len, HAL_MAX_DELAY) == HAL_OK) ? len : -1;
}
#endif
#if TASK == 5
int _write(int file, char *ptr, int len)
{
  return (HAL_UART_Transmit(&huart3, (uint8_t*)ptr, len, HAL_MAX_DELAY) == HAL_OK) ? len : -1;
}

int _read(int file, char *ptr, int len)
{
  int msg_len = 0;
  while(msg_len <= len)
  {
    if(HAL_UART_Receive(&huart3, (uint8_t*)&ptr[msg_len], 1, HAL_MAX_DELAY) == HAL_OK)
    {
      msg_len++;
      if(ptr[msg_len-1] == '\r')
        break;
    }
  }
  return msg_len;
}
#endif
#if TASK == 7
int _write(int file, char *ptr, int len)
{
  return (HAL_UART_Transmit(&huart3, (uint8_t*)ptr, len, HAL_MAX_DELAY) == HAL_OK) ? len : -1;
}

int _read(int file, char *ptr, int len)
{
  int msg_len = 0;
  while(msg_len <= len)
  {
    if(HAL_UART_Receive(&huart3, (uint8_t*)&ptr[msg_len], 1, HAL_MAX_DELAY) == HAL_OK)
    {
      msg_len++;
      if(ptr[msg_len-1] == '\r')
        break;
    }
  }
  return msg_len;
}

/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == hlamp1.SYNC_Pin)
  {
    LAMP_StartFiringAngleTimer(&hlamp1);
  }
}
/**
  * @brief  Period elapsed callback in non-blocking mode
  * @param  htim TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim == hlamp1.FiringAngleTimer)
  {
    LAMP_StopFiringAngleTimer(&hlamp1);
    LAMP_StartPulseTimer(&hlamp1);
  }
  if(htim == hlamp1.PulseTimer)
  {
    LAMP_StopPulseTimer(&hlamp1);
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
  MX_I2C1_Init();
  MX_TIM9_Init();
  MX_TIM7_Init();
  MX_TIM11_Init();
  /* USER CODE BEGIN 2 */
  #if TASK == 3
	BH1750_Init(&hbh1750A);
	#endif
  #if TASK == 4
  BH1750_Init(&hbh1750A);
  #endif
  #if TASK == 5
  LED_PWM_Init(&hld1);
  BH1750_Init(&hbh1750A);
  #endif
  #if TASK == 7
  BH1750_Init(&hbh1750B);
  #endif
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    #if TASK == 3
    illuminance_mlx = 1000*BH1750_ReadIlluminance_lux(&hbh1750A);
    HAL_Delay(200);
    #endif
    #if TASK == 4
    illuminance_mlx = 1000*BH1750_ReadIlluminance_lux(&hbh1750A);
    printf("{\"id\":1,\"value\":%d.%03d}\r\n", illuminance_mlx / 1000, illuminance_mlx % 1000);
    HAL_Delay(200);
    #endif
    #if TASK == 5
    char Command;
    scanf("%c%d", &Command, &duty_cycle);
    if(Command == 'R' || Command == 'r')      // Read
    {
      illuminance_mlx = 1000*BH1750_ReadIlluminance_lux(&hbh1750A);
      printf("{\"id\":1,\"value\":%d.%03d}\r\n", illuminance_mlx / 1000, illuminance_mlx % 1000);
    }
    else if(Command == 'W' || Command == 'w') // Write
    {
      LED_PWM_WriteDuty(&hld1, duty_cycle);
    }
    #endif
    #if TASK == 7
    char Command;
    scanf("%c%d", &Command, &input_value);
    if(Command == 'R' || Command == 'r')      // Read
    {
      illuminance_mlx = 1000*BH1750_ReadIlluminance_lux(&hbh1750B);
      printf("{\"id\":2,\"value\":%d.%03d}\r\n", illuminance_mlx / 1000, illuminance_mlx % 1000);
    }
    else if(Command == 'W' || Command == 'w') // Write
    {
      LAMP_WriteFiringAngle(&hlamp1, input_value);
    }
    else if(Command == 'L' || Command == 'L') // Linearize
    {
      LAMP_WriteBrightness(&hlamp1, input_value);
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
