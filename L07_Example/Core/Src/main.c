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
#define TASK 2
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#if TASK == 1
#include "aio.h"
#endif
#if TASK == 2
#include "aio.h"
#include "led_config.h"
#endif
#if TASK == 3
#include <stdio.h>
#include "aio.h"
#include "btn_config.h"
#endif
#if TASK == 41
#include "aio.h"
#endif
#if TASK == 42
#include "aio.h"
#endif
#if TASK == 43
#include "aio.h"
#endif
#if TASK == 5
#include "ntc_config.h"
#include "ldr_config.h"
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
#if TASK == 1
unsigned int pot1_mV;
#endif
#if TASK == 2
unsigned int pot1_mV;
#endif
#if TASK == 3
unsigned int pot1_mV;
#endif
#if TASK == 41
unsigned int pot1_mV, pot2_mV;
#endif
#if TASK == 42
unsigned int pot1_mV, pot2_mV;
#endif
#if TASK == 43
unsigned int pot1_mV, pot2_mV;
uint16_t adc1_conv_buffer[ADC1_NUMBER_OF_CONV];
#endif
#if TASK == 5
unsigned int sen1_mV;

float sen1_temp_sh;
float sen1_temp_beta;

float sen1_light_gamma;

#endif
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#if TASK == 3
int _write(int file, char *ptr, int len)
{
  return (HAL_UART_Transmit(&huart3, (uint8_t*)ptr, len, HAL_MAX_DELAY) == HAL_OK) ? len : -1;
}
#endif
#if TASK == 42
/**
  * @brief  Regular conversion complete callback in non blocking mode
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *         the configuration information for the specified ADC.
  * @retval None
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  if(hadc == &hadc1)
  {
    if(hadc1.NbrOfCurrentConversionRank == 1)
      pot1_mV = ADC_REG2VOLTAGE(HAL_ADC_GetValue(&hadc1));

    else if(hadc1.NbrOfCurrentConversionRank == 2)
      pot2_mV = ADC_REG2VOLTAGE(HAL_ADC_GetValue(&hadc1));

    hadc1.NbrOfCurrentConversionRank = 1 + (hadc1.NbrOfCurrentConversionRank % ADC1_NUMBER_OF_CONV);
  }
}
#endif
#if TASK == 43
/**
  * @brief  Regular conversion complete callback in non blocking mode
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *         the configuration information for the specified ADC.
  * @retval None
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  if(hadc == &hadc1)
  {
    pot1_mV = ADC_REG2VOLTAGE(adc1_conv_buffer[0]);
    pot2_mV = ADC_REG2VOLTAGE(adc1_conv_buffer[1]);
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
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  #if TASK == 42
  hadc1.NbrOfCurrentConversionRank = 1;
  #endif
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    #if TASK == 1
    HAL_ADC_Start(&hadc1);
    if(HAL_ADC_PollForConversion(&hadc1, ADC1_TIMEOUT) == HAL_OK)
      pot1_mV = ADC_REG2VOLTAGE(HAL_ADC_GetValue(&hadc1));
    HAL_Delay(10);
    #endif
    #if TASK == 2
    HAL_ADC_Start(&hadc1);
    if(HAL_ADC_PollForConversion(&hadc1, ADC1_TIMEOUT) == HAL_OK)
    {
      pot1_mV = ADC_REG2VOLTAGE(HAL_ADC_GetValue(&hadc1));
      LED_Line_WriteValue(pot1_mV / 1000);
    }
    HAL_Delay(10);
    #endif
    #if TASK == 3
    if(BTN_DIO_EdgeDetected(&husrbtn) == BTN_PRESSED_EDGE)
    {
      HAL_ADC_Start(&hadc1);
      if(HAL_ADC_PollForConversion(&hadc1, ADC1_TIMEOUT) == HAL_OK)
      {
        pot1_mV = ADC_REG2VOLTAGE(HAL_ADC_GetValue(&hadc1));
        printf("{\"id\":\"POT1\",\"voltage\":%4d }\r\n", pot1_mV);
      }
    }
    HAL_Delay(10);
    #endif
    #if TASK == 41

    HAL_ADC_Start(&hadc1);
    if(HAL_ADC_PollForConversion(&hadc1, ADC1_TIMEOUT) == HAL_OK)
      pot1_mV = ADC_REG2VOLTAGE(HAL_ADC_GetValue(&hadc1));

    HAL_ADC_Start(&hadc1);
    if(HAL_ADC_PollForConversion(&hadc1, ADC1_TIMEOUT) == HAL_OK)
      pot2_mV = ADC_REG2VOLTAGE(HAL_ADC_GetValue(&hadc1));

    HAL_Delay(10);
    #endif
    #if TASK == 42
    HAL_ADC_Start_IT(&hadc1);
    HAL_Delay(10);
    #endif
    #if TASK == 43
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc1_conv_buffer, ADC1_NUMBER_OF_CONV);
    HAL_Delay(10);
    #endif
    #if TASK == 5
    HAL_ADC_Start(&hadc1);
    if(HAL_ADC_PollForConversion(&hadc1, ADC1_TIMEOUT) == HAL_OK)
    {
      sen1_mV = ADC_REG2VOLTAGE(HAL_ADC_GetValue(&hadc1));

      // If NTC
      sen1_temp_sh = NTC_SteinhartHart_ReadTemperature_degC(&hntc1_sh, sen1_mV);
      sen1_temp_beta = NTC_Beta_ReadTemperature_degC(&hntc1_beta, sen1_mV);

      // If LDR
      sen1_light_gamma = LDR_Gamma_ReadIlluminance_lx(&hldr1_gamma, sen1_mV);
    }

    HAL_Delay(10);
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
