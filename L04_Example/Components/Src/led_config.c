/**
  ******************************************************************************
  * @file		  : led_config.c
  * @author  	: AW		Adrian.Wojcik@put.poznan.pl
  * @version  : 1.4.0
  * @date     : Oct 30, 2024
  * @brief   	: LED components configuration file
  *
  ******************************************************************************
  */

/* Private includes ----------------------------------------------------------*/
#include "led.h"
#include "main.h"
#include "tim.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Public variables ----------------------------------------------------------*/
LED_PWM_Handle_TypeDef hldr = LED_PWM_INIT_HANDLE(&htim4, TIM_CHANNEL_1, LED_ON_HIGH);
LED_PWM_Handle_TypeDef hldg = LED_PWM_INIT_HANDLE(&htim4, TIM_CHANNEL_2, LED_ON_HIGH);
LED_PWM_Handle_TypeDef hldb = LED_PWM_INIT_HANDLE(&htim4, TIM_CHANNEL_3, LED_ON_HIGH);

LED_RGB_PWM_Handle_TypeDef hldrgb = { .R = &hldr, .G = &hldg, .B = &hldb };

/* Private function prototypes -----------------------------------------------*/

/* Public function prototypes ------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/* Public functions ----------------------------------------------------------*/
