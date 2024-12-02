/**
  ******************************************************************************
  * @file		: ntc.h
  * @author  	: AW		Adrian.Wojcik@put.poznan.pl
  * @version  	: 1.0.5
  * @date     	: Dec 2, 2024
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
 * @brief NTC Thermistor model structure base on Steinhart-Hart equation (3rd order).
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
 * @brief NTC Thermistor model structure base on Beta parameter equation (1st order). 
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
 * @brief Computes temperature from voltage drop on NTC thermistor using Steinhart-Hart equation.
 * @param[in/out] hvd : NTC thermistor (Steinhart-Hart equation) handle
 * @param[in] voltage : Output voltage of voltage divider in millivolts
 * @return Temperature in deg C
 */
float NTC_SteinhartHart_ReadTemperature_degC(NTC_SteinhartHart_Handle_TypeDef* hntc, unsigned int voltage);

/**
 * @brief Computes temperature from voltage drop on NTC thermistor using Beta parameter.
 * @param[in/out] hvd : NTC thermistor (Beta parameter) handle
 * @param[in] voltage : Output voltage of voltage divider in millivolts
 * @return Temperature in deg C
 */
float NTC_Beta_ReadTemperature_degC(NTC_Beta_Handle_TypeDef* hntc, unsigned int voltage);

#endif /* INC_NTC_H_ */
