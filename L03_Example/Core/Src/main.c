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
#define TASK 5
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#if TASK == 3
#include <stdio.h>
#endif
#if TASK == 4
#include <stdio.h>
#endif
#if TASK == 5
#include <stdio.h>
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
GPIO_PinState LD1_State;
#endif
#if TASK == 2
GPIO_PinState LD1_State;
#endif
#if TASK == 3
GPIO_PinState LD1_State;
uint32_t TimerAutoreloadRegister;
#endif
#if TASK == 4
GPIO_PinState LD1_State;
GPIO_PinState TRIG_State;
uint32_t TimerAutoreloadRegister;
#endif
#if TASK == 5
GPIO_PinState LD1_State;
GPIO_PinState TRIG_State;
uint32_t TimerAutoreloadRegister;
#endif
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#if TASK == 2
/**
  * @brief  Period elapsed callback in non-blocking mode
  * @param  htim TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim == &htim7)
  {
    HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
    LD1_State = HAL_GPIO_ReadPin(LD1_GPIO_Port, LD1_Pin);
  }
}
#endif
#if TASK == 3
/**
  * @brief  Period elapsed callback in non-blocking mode
  * @param  htim TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim == &htim7)
  {
    HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
    LD1_State = HAL_GPIO_ReadPin(LD1_GPIO_Port, LD1_Pin);
  }
}
#endif
#if TASK == 4
/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == USER_Btn_Pin)
  {
    HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
    LD1_State = HAL_GPIO_ReadPin(LD1_GPIO_Port, LD1_Pin);
    __HAL_TIM_SET_COUNTER(&htim7, 0);
    __HAL_TIM_SET_AUTORELOAD(&htim7, TimerAutoreloadRegister);
    HAL_TIM_Base_Start_IT(&htim7);
  }
}
/**
  * @brief  Period elapsed callback in non-blocking mode
  * @param  htim TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim == &htim7)
  {
    HAL_TIM_Base_Stop_IT(&htim7);
    HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET);
    LD1_State = HAL_GPIO_ReadPin(LD1_GPIO_Port, LD1_Pin);
    HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_SET);
    TRIG_State = HAL_GPIO_ReadPin(TRIG_GPIO_Port, TRIG_Pin);
    HAL_TIM_Base_Start_IT(&htim11);
  }
  if(htim == &htim11)
  {
    HAL_TIM_Base_Stop_IT(&htim11);
    HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_RESET);
    TRIG_State = HAL_GPIO_ReadPin(TRIG_GPIO_Port, TRIG_Pin);
  }
}
#endif
#if TASK == 5
/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == SYNC_Pin)
  {
    HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
    LD1_State = HAL_GPIO_ReadPin(LD1_GPIO_Port, LD1_Pin);
    __HAL_TIM_SET_COUNTER(&htim7, 0);
    __HAL_TIM_SET_AUTORELOAD(&htim7, TimerAutoreloadRegister);
    HAL_TIM_Base_Start_IT(&htim7);
  }
}
/**
  * @brief  Period elapsed callback in non-blocking mode
  * @param  htim TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim == &htim7)
  {
    HAL_TIM_Base_Stop_IT(&htim7);
    HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET);
    LD1_State = HAL_GPIO_ReadPin(LD1_GPIO_Port, LD1_Pin);
    HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_SET);
    TRIG_State = HAL_GPIO_ReadPin(TRIG_GPIO_Port, TRIG_Pin);
    HAL_TIM_Base_Start_IT(&htim11);
  }
  if(htim == &htim11)
  {
    HAL_TIM_Base_Stop_IT(&htim11);
    HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_RESET);
    TRIG_State = HAL_GPIO_ReadPin(TRIG_GPIO_Port, TRIG_Pin);
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
  MX_TIM7_Init();
  MX_TIM11_Init();
  /* USER CODE BEGIN 2 */
  #if TASK == 1
  HAL_TIM_Base_Start(&htim7);
  #endif
  #if TASK == 2
  HAL_TIM_Base_Start_IT(&htim7);
  #endif
  #if TASK == 3
  HAL_TIM_Base_Start_IT(&htim7);
  #endif
  #if TASK == 4
  TimerAutoreloadRegister = __HAL_TIM_GET_AUTORELOAD(&htim7);
  #endif
  #if TASK == 5
  TimerAutoreloadRegister = __HAL_TIM_GET_AUTORELOAD(&htim7);
  #endif
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    #if TASK == 1
    if(__HAL_TIM_GET_FLAG(&htim7, TIM_FLAG_UPDATE))
    {
      __HAL_TIM_CLEAR_FLAG(&htim7, TIM_FLAG_UPDATE);

      HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
      LD1_State = HAL_GPIO_ReadPin(LD1_GPIO_Port, LD1_Pin);
    }
    #endif
    #if TASK == 3
    uint8_t RxData_TimerPeriod_us[] = "0000";
    if(HAL_UART_Receive(&huart3, RxData_TimerPeriod_us, sizeof(RxData_TimerPeriod_us) - 1, HAL_MAX_DELAY) == HAL_OK)
    {
      if(sscanf((char*)RxData_TimerPeriod_us, "%lu", &TimerAutoreloadRegister) == 1)
      {
        if(TimerAutoreloadRegister >= 1000 && TimerAutoreloadRegister <= 9999)
        {
          HAL_TIM_Base_Stop_IT(&htim7);
          __HAL_TIM_SET_COUNTER(&htim7, 0);
          __HAL_TIM_SET_AUTORELOAD(&htim7, TimerAutoreloadRegister);
          HAL_TIM_Base_Start_IT(&htim7);
        }
      }
    }
    #endif
    #if TASK == 4
    uint8_t RxData_TimerPeriod_us[] = "0000";
    if(HAL_UART_Receive(&huart3, RxData_TimerPeriod_us, sizeof(RxData_TimerPeriod_us) - 1, HAL_MAX_DELAY) == HAL_OK)
    {
      uint32_t TimerAutoreloadRegister_Tmp = 0;
      if(sscanf((char*)RxData_TimerPeriod_us, "%lu", &TimerAutoreloadRegister_Tmp) == 1)
      {
        if(TimerAutoreloadRegister_Tmp >= 1000 && TimerAutoreloadRegister_Tmp <= 9999)
        {
          TimerAutoreloadRegister = TimerAutoreloadRegister_Tmp;
        }
      }
    }
    #endif
    #if TASK == 5
    uint8_t RxData_Phase_deg[] = "000\r";
    if(HAL_UART_Receive(&huart3, RxData_Phase_deg, sizeof(RxData_Phase_deg) - 1, HAL_MAX_DELAY) == HAL_OK)
    {
      uint32_t Phase = 0;
      if(sscanf((char*)RxData_Phase_deg, "%lu", &Phase) == 1)
      {
        if(Phase >= 10 && Phase <= 160)
        {
          TimerAutoreloadRegister = 10000.0f*(Phase / 180.0f) - 1;
        }
      }
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
