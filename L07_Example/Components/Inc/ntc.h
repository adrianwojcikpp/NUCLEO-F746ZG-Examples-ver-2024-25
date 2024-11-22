/**
  ******************************************************************************
  * @file		  : ntc.h
  * @author  	: AW		Adrian.Wojcik@put.poznan.pl
  * @version 	: 1.0.0
  * @date    	: Nov 22, 2024
  * @brief   	: NTC thermistor components driver
  *
  ******************************************************************************
  */

#ifndef INC_NTC_H_
#define INC_NTC_H_

/* Public includes -----------------------------------------------------------*/
#include "aio.h"

/* Public typedef ------------------------------------------------------------*/
/**
 * TODO
 */
typedef struct {
  float R;
  VOLTAGE_DIVIDER_Handle_TypeDef* VoltageDivider;
  float Roffset;
  float A;
  float B;
  float C;
} NTC_SteinhartHart_Handle_TypeDef;

/**
 * TODO
 */
typedef struct {
  float R;
  VOLTAGE_DIVIDER_Handle_TypeDef* VoltageDivider;
  float Roffset;
  float R25degC;
  float beta;
} NTC_Beta_Handle_TypeDef;

/* Public define -------------------------------------------------------------*/

/* Public macro --------------------------------------------------------------*/

/* Public variables ----------------------------------------------------------*/

/* Public function prototypes ------------------------------------------------*/

/**
 * TODO
 */
float NTC_SteinhartHart_ReadTemperature_degC(NTC_SteinhartHart_Handle_TypeDef* hntc, unsigned int voltage);

/**
 * TODO
 */
float NTC_Beta_ReadTemperature_degC(NTC_Beta_Handle_TypeDef* hntc, unsigned int voltage);

#endif /* INC_NTC_H_ */
