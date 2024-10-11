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
#define TASK 1
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#if TASK == 5
typedef struct {
  _Bool EdgeDetected;
  _Bool PullUp;
  GPIO_TypeDef *Port;
  uint16_t Pin;
} Button_TypeDef;
#endif // TASK #5
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#if TASK == 1
#define LD_N 3
#endif // TASK #2
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#if TASK == 5
#define BTN_EdgeDetected(hbtn)  ((HAL_GPIO_ReadPin((hbtn)->Port, (hbtn)->Pin)) == (hbtn)->PullUp)
#endif // TASK #5
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
#if TASK == 1
GPIO_PinState LD_State[LD_N] = { GPIO_PIN_RESET, };
uint8_t LD_Cnt = 0;
const uint16_t LD_Pins[LD_N] = {
    LD1_Pin,
    LD2_Pin,
    LD3_Pin
};
GPIO_TypeDef* LD_GPIO_Port = GPIOB;
#endif // TASK #1
#if TASK == 2
GPIO_PinState USER_Btn_State = GPIO_PIN_RESET;
GPIO_PinState USER_Btn_StatePrev = GPIO_PIN_RESET;
GPIO_PinState LD1_State = GPIO_PIN_RESET;
#endif // TASK #2
#if TASK == 3
volatile _Bool USER_Btn_RisingEdgeDetected = 0;
GPIO_PinState LD1_State = GPIO_PIN_RESET;
#endif // TASK #3
#if TASK == 4
volatile _Bool ON_Btn_FallingEdgeDetected = 0;
GPIO_PinState LD4_State = GPIO_PIN_RESET;
#endif // TASK #4
#if TASK == 5
Button_TypeDef ON_Btn  = { 0, 0, ON_Btn_GPIO_Port,  ON_Btn_Pin  };
Button_TypeDef OFF_Btn = { 0, 1, OFF_Btn_GPIO_Port, OFF_Btn_Pin };
GPIO_PinState LD4_State = GPIO_PIN_RESET;
#endif // TASK #5
#if TASK == 6
volatile _Bool ENC_CLK_EdgeDetected = 0;
int ENC_Cnt = 0;
#endif // TASK #6
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#if TASK == 3
/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == USER_Btn_Pin)
    USER_Btn_RisingEdgeDetected = 1;
}
#endif // TASK #3
#if TASK == 4
/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == ON_Btn_Pin)
    ON_Btn_FallingEdgeDetected = 1;
}
#endif // TASK #4
#if TASK == 5
/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == ON_Btn.Pin)
    ON_Btn.EdgeDetected = 1;
  if(GPIO_Pin == OFF_Btn.Pin)
    OFF_Btn.EdgeDetected = 1;
}
#endif // TASK #5
#if TASK == 6
/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == ENC_CLK_Pin)
    ENC_CLK_EdgeDetected = 1;
}
#endif // TASK #6
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
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    #if TASK == 1
    LD_Cnt = (LD_Cnt+1) % LD_N;

    for(int i = 0; i < LD_N; i++)
      HAL_GPIO_WritePin(LD_GPIO_Port, LD_Pins[i], GPIO_PIN_RESET);

    HAL_GPIO_WritePin(LD_GPIO_Port, LD_Pins[LD_Cnt], GPIO_PIN_SET);

    for(int i = 0; i < LD_N; i++)
      LD_State[i] = HAL_GPIO_ReadPin(LD_GPIO_Port, LD_Pins[i]);

    HAL_Delay(100);
    #endif // TASK #1
    #if TASK == 2
    USER_Btn_State = HAL_GPIO_ReadPin(USER_Btn_GPIO_Port, USER_Btn_Pin);
    if((USER_Btn_State == GPIO_PIN_SET) && (USER_Btn_StatePrev == GPIO_PIN_RESET))
    {
      HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
      LD1_State = HAL_GPIO_ReadPin(LD1_GPIO_Port, LD1_Pin);
    }
    USER_Btn_StatePrev = USER_Btn_State;
    HAL_Delay(10);
    #endif // TASK #2
    #if TASK == 3
    if(USER_Btn_RisingEdgeDetected)
    {
      HAL_Delay(10);
      USER_Btn_RisingEdgeDetected = 0;
      if(HAL_GPIO_ReadPin(USER_Btn_GPIO_Port, USER_Btn_Pin) == GPIO_PIN_SET)
      {
        HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
        LD1_State = HAL_GPIO_ReadPin(LD1_GPIO_Port, LD1_Pin);
      }
    }
    #endif // TASK #3
    #if TASK == 4
    if(ON_Btn_FallingEdgeDetected)
    {
      HAL_Delay(10);
      ON_Btn_FallingEdgeDetected = 0;
      if(HAL_GPIO_ReadPin(ON_Btn_GPIO_Port, ON_Btn_Pin) == GPIO_PIN_RESET)
      {
        HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin | LD5_Pin | LD6_Pin);
        LD4_State = HAL_GPIO_ReadPin(LD4_GPIO_Port, LD4_Pin);
      }
    }
    #endif // TASK #4
    #if TASK == 5
    if(ON_Btn.EdgeDetected)
    {
      HAL_Delay(10);
      ON_Btn.EdgeDetected = 0;
      if(BTN_EdgeDetected(&ON_Btn))
      {
        HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_SET);
        LD4_State = HAL_GPIO_ReadPin(LD4_GPIO_Port, LD4_Pin);
      }
    }
    if(OFF_Btn.EdgeDetected)
    {
      HAL_Delay(10);
      OFF_Btn.EdgeDetected = 0;
      if(BTN_EdgeDetected(&OFF_Btn))
      {
        HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET);
        LD4_State = HAL_GPIO_ReadPin(LD4_GPIO_Port, LD4_Pin);
      }
    }
    #endif // TASK #5
    #if TASK == 6
    if(ENC_CLK_EdgeDetected)
    {
      HAL_Delay(0);
      ENC_CLK_EdgeDetected = 0;
      if(HAL_GPIO_ReadPin(ENC_CLK_GPIO_Port, ENC_CLK_Pin) == GPIO_PIN_RESET)
      {
        if(HAL_GPIO_ReadPin(ENC_DT_GPIO_Port, ENC_DT_Pin) == GPIO_PIN_RESET)
          ENC_Cnt++;
        else
          ENC_Cnt--;

      }
    }

    /// TODO: Add LED blinking routing with variable frequency

    #endif // TASK #6
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
