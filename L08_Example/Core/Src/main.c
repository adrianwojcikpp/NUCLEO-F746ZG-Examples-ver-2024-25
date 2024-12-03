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
#include "dac.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#if TASK == 1
#include "aio.h"
#endif
#if TASK == 2
#define _USE_MATH_DEFINES
#include <math.h>
#include "aio.h"
#endif
#if TASK == 3
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "aio.h"
#endif
#if TASK == 5
#include "sine_wave_buffer.h"
#endif
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#if TASK == 2 || TASK == 3
/**
 * @brief  Harmonic signal parameters
 */
typedef struct {
  float Amplitude;  /* mV  */
  float Phase;      /* rad */
  float Mean;       /* Hz  */
  float Frequency;  /* mV  */
  float SampleTime; /* s   */
} SINE_WAVE_Handle_TypeDef;
#endif
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
float v_out = 1000.0f;
#endif
#if TASK == 2
const SINE_WAVE_Handle_TypeDef hsine = {
    .Amplitude = 1000,  /* mV  */
    .Phase = 0*M_PI,    /* rad */
    .Frequency = 10,    /* Hz  */
    .Mean = 1000,       /* mV  */
    .SampleTime = 0.001 /* s   */
};
unsigned long int time = 0; /* ms */
float v_out = 0.0f;         /* mV */
#endif
#if TASK == 3
SINE_WAVE_Handle_TypeDef hsine = {
    .Amplitude = 1000,  /* mV  */
    .Phase = 0*M_PI,    /* rad */
    .Frequency = 10,    /* Hz  */
    .Mean = 1000,       /* mV  */
    .SampleTime = 0.001 /* s   */
};
unsigned long int time = 0; /* ms */
float v_out = 0.0f;         /* mV */
uint8_t rx_data[] = "A0000";
#endif
#if TASK == 6
//extern unsigned char rawData[39915];
extern unsigned char rawData[79830];
#endif
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
#if TASK == 1 || TASK == 2 || TASK == 3
/**
 * @brief Writes to DAC data holding register (Channel #1, 12 bits, right alignment).
 * @param[in] voltage : Voltage expressed in millivolts, in range <0 - 3300> mV
 * @retval None
 */
void DAC_SetVoltage(float voltage);
#endif
#if TASK == 2 || TASK == 3
/**
 * @brief Computes value of given sine wave at given discrete time.
 * @param[in] sine_wave     : Structure with sine wave parameters.
 * @param[in] discrete_time : Discrete time (sample number).
 * @retval Sine wave value at discrete time.
 */
float SINE_WAVE_GetValue(const SINE_WAVE_Handle_TypeDef* sine_wave, unsigned int discrete_time);
#endif
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#if TASK == 3
/**
  * @brief  Rx Transfer completed callback.
  * @param  huart UART handle.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart == &huart3)
  {
    char param = '\0';
    int value;
    if(sscanf((char*)rx_data, "%c%d", &param, &value) == 2)
    {
      switch(param)
      {
      case 'A':
      case 'a':
        hsine.Amplitude = __SATURATION(value, 0, DAC_VOLTAGE_MAX/2);
        break;
      case 'm':
      case 'M':
        hsine.Mean = __SATURATION(value, 0, DAC_VOLTAGE_MAX);
        break;
      case 'f':
      case 'F':
        hsine.Frequency = __SATURATION(value, 1, 100);
        break;
      default:
        break;
      }
    }
    HAL_UART_Receive_IT(&huart3, rx_data, sizeof(rx_data) - 1);
  }
}
#endif

/**
  * @brief  Conversion complete callback in non-blocking mode for Channel1
  * @param  hdac pointer to a DAC_HandleTypeDef structure that contains
  *         the configuration information for the specified DAC.
  * @retval None
  */
void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef *hdac)
{
  HAL_DAC_Stop_DMA(hdac, DAC_CHANNEL_1);
  HAL_TIM_Base_Stop(&htim7);
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
  MX_DAC_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */
	#if TASK == 1
  HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
	DAC_SetVoltage(v_out);
	#endif
  #if TASK == 2
  HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
  HAL_TIM_Base_Start(&htim7);
  #endif
  #if TASK == 3
  HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
  HAL_TIM_Base_Start(&htim7);
  HAL_UART_Receive_IT(&huart3, rx_data, sizeof(rx_data) - 1);
  #endif
  #if TASK == 5
  HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*)sine_wave_buffer, SINE_WAVE_SIZE, DAC_ALIGN_12B_R);
  HAL_TIM_Base_Start(&htim7);
  #endif
  #if TASK == 6
  HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*)rawData, sizeof(rawData)/2, DAC_ALIGN_12B_L);
  HAL_TIM_Base_Start(&htim7);
  #endif
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    #if TASK == 2 || TASK == 3
    if(__HAL_TIM_GET_FLAG(&htim7, TIM_FLAG_UPDATE))
    {
      __HAL_TIM_CLEAR_FLAG(&htim7, TIM_FLAG_UPDATE);
      v_out = SINE_WAVE_GetValue(&hsine, time);
      DAC_SetVoltage(v_out);
      time++;
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
#if TASK == 1 || TASK == 2 || TASK == 3
/**
 * @brief Writes to DAC data holding register (Channel #1, 12 bits, right alignment).
 * @param[in] voltage : Voltage expressed in millivolts, in range <0 - 3300> mV
 * @retval None
 */
void DAC_SetVoltage(float voltage)
{
	// TODO
  float voltage_sat_mV = DAC_VOLTAGE_SAT(voltage);
  uint16_t dac_reg = DAC_VOLTAGE2REG(voltage_sat_mV);
  HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dac_reg);
}
#endif
#if TASK == 2 || TASK == 3
/**
 * @brief Computes value of given sine wave at given discrete time.
 * @param[in] sine_wave     : Structure with sine wave parameters.
 * @param[in] discrete_time : Discrete time (sample number).
 * @retval Sine wave value at discrete time.
 */
float SINE_WAVE_GetValue(const SINE_WAVE_Handle_TypeDef* sine_wave, unsigned int discrete_time)
{
  float time = sine_wave->SampleTime*discrete_time;
  float value = (sine_wave->Amplitude)*sinf(2.0f*M_PI*sine_wave->Frequency*time + sine_wave->Phase) + sine_wave->Mean;
  return value;
}
#endif
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
