/**
  ******************************************************************************
  * @file	  : led_config.h
  * @author   : AW		Adrian.Wojcik@put.poznan.pl
  * @version  : 1.4.0
  * @date     : Oct 30, 2024
  * @brief    : LED components configuration file
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
extern LED_DIO_Handle_TypeDef hld1;
extern LED_DIO_Handle_TypeDef hld2;
extern LED_DIO_Handle_TypeDef hld3;

/* Public function prototypes ------------------------------------------------*/
/**
 * TODO
 */
void LED_Line_WriteValue(unsigned int n);

#endif /* INC_LED_CONFIG_H_ */
