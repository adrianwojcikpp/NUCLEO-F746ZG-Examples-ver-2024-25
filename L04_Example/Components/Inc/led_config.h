/**
  ******************************************************************************
  * @file		  : led_config.h
  * @author  	: AW		Adrian.Wojcik@put.poznan.pl
  * @version  : 1.4.0
  * @date     : Oct 30, 2024
  * @brief   	: LED components configuration file
  *
  ******************************************************************************
  */
#ifndef INC_LED_CONFIG_H_
#define INC_LED_CONFIG_H_

/* Public includes -----------------------------------------------------------*/
#include "led.h"

/* Public typedef ------------------------------------------------------------*/

/* Public define -------------------------------------------------------------*/

/* Public macro --------------------------------------------------------------*/

/* Public variables ----------------------------------------------------------*/
extern LED_PWM_Handle_TypeDef hldr; //!< Red channel of breadboard RGB THT LED
extern LED_PWM_Handle_TypeDef hldg; //!< Green channel of breadboard RGB THT LED
extern LED_PWM_Handle_TypeDef hldb; //!< Blue channel of breadboard RGB THT LED

extern LED_RGB_PWM_Handle_TypeDef hldrgb;

/* Public function prototypes ------------------------------------------------*/

#endif /* INC_LED_CONFIG_H_ */
