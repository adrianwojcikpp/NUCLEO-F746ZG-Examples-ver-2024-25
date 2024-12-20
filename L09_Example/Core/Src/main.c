/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "pid_controller_config.h"
#include "led_config.h"
#include "aio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct {
  float measurement;
  float reference;
  float control;
} SWV_TypeDef;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define CLOSED_LOOP
#define CNT_MAX 900
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t rx_buffer[32];
uint16_t msg_len;

float duty = 50.0f;              // [%]
float current = 0.0f;            // [mA]
float current_ref = 0.0f;        // [mA]
const float resistance = 100.0f; // [Ohm]
uint32_t cnt = 0;
const uint32_t cnt_dec = 10;
SWV_TypeDef swv;

#ifdef CNT_MAX
uint8_t txBuffer[2][CNT_MAX * sizeof(swv)];
_Bool txActiveBuffer = 0;
_Bool txFlag = 0;
#endif
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/**
  * @brief  Rx Transfer completed callback.
  * @param  huart UART handle.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart == &huart3)
  {
    if(rx_buffer[0] == 'R')
    {
#ifdef CLOSED_LOOP
      sscanf((char*)&rx_buffer[1], "%f", &current_ref);

#else
      sscanf((char*)&rx_buffer[1], "%f", &duty);
      LED_PWM_WriteDuty(&hld1, duty);
#endif
#ifdef CNT_MAX
      txFlag = 1;
#endif
    }
    else
    {
      uint8_t tx_buffer[256];
      int resp_len = sprintf((char*)tx_buffer, "{ \"I1\":{\"value\":%f,\"unit\":\"mA\"}, \"I1REF\":{\"value\":%f,\"unit\":\"mA\"}, \"D1\":{\"value\":%f,\"unit\":\"%%\"} }\r", current, current_ref, duty);
      HAL_UART_Transmit_DMA(&huart3, tx_buffer, resp_len);
    }
    HAL_UART_Receive_IT(&huart3, rx_buffer, msg_len);
  }
}

/**
  * @brief  Period elapsed callback in non-blocking mode
  * @param  htim TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim == hld1.Output.Timer)
  {
    HAL_GPIO_WritePin(DEBUG1_GPIO_Port, DEBUG1_Pin, GPIO_PIN_SET);
    HAL_ADC_Start_IT(&hadc1);
  }
}

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
    float voltage = ADC_REG2VOLTAGE(HAL_ADC_GetValue(hadc)); // [mV]
    // Ohm's law
    current = voltage / resistance;                          // [mA]

#ifdef CLOSED_LOOP
    duty = PID_GetOutput(&hpid1, current_ref, current);  // [%]
    LED_PWM_WriteDuty(&hld1, duty);
#endif

    HAL_GPIO_WritePin(DEBUG1_GPIO_Port, DEBUG1_Pin, GPIO_PIN_RESET);

    if(cnt % cnt_dec == 0)
    {
      swv.measurement = current;
      swv.reference = current_ref;
      swv.control = duty;

#ifdef CNT_MAX
      memcpy(&txBuffer[txActiveBuffer][(cnt / cnt_dec) * sizeof(swv)], &swv, sizeof(swv));
#endif
    }

#ifdef CNT_MAX
      cnt = (cnt < cnt_dec * CNT_MAX - 1) ? (cnt + 1) : 0;
#else
      cnt++;
#endif

#ifdef CNT_MAX
    if(cnt == 0 && txFlag)
    {
      HAL_UART_Transmit_DMA(&huart3, txBuffer[txActiveBuffer], CNT_MAX*sizeof(swv));
      txActiveBuffer ^= 1;
    }
#endif

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
  MX_DMA_Init();
  MX_USART3_UART_Init();
  MX_TIM2_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  PID_Init(&hpid1);
  LED_PWM_Init(&hld1);
  HAL_TIM_Base_Start_IT(hld1.Output.Timer);

#ifdef CLOSED_LOOP
  msg_len = strlen("R00.0mA\r");
#else
  msg_len = strlen("R000.0%\r");
#endif

  HAL_UART_Receive_IT(&huart3, rx_buffer, msg_len);
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
