/**
  ******************************************************************************
  * @file		  : led_config.h
  * @author  	: AW		Adrian.Wojcik@put.poznan.pl
  * @version 	: 1.3.1
  * @date     : Nov 27, 2022
  * @brief   	: Single-color LED components configuration file
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
#define LED_DIO_AllOn()   LED_DIO_On(&hld1);\
		                      LED_DIO_On(&hld2);\
		                      LED_DIO_On(&hld3)

#define LED_DIO_AllOff()  LED_DIO_Off(&hld1);\
                          LED_DIO_Off(&hld2);\
                          LED_DIO_Off(&hld3)

/* Public variables ----------------------------------------------------------*/
extern LED_DIO_Handle_TypeDef hld1;
extern LED_DIO_Handle_TypeDef hld2;
extern LED_DIO_Handle_TypeDef hld3;

/* Public function prototypes ------------------------------------------------*/

#endif /* INC_LED_CONFIG_H_ */
