/**
  ******************************************************************************
  * @file     : aio.h
  * @author   : AW    Adrian.Wojcik@put.poznan.pl
  * @version  : 1.3.5
  * @date     : Dec 2, 2024
  * @brief    : Analog inputs/outputs components header.
  *
  ******************************************************************************
  */

#ifndef INC_AIO_H_
#define INC_AIO_H_

/* Public includes -----------------------------------------------------------*/
#include "common.h"

/* Public typedef ------------------------------------------------------------*/
typedef struct {
  float R_up;
  float R_down;
  float Gain;
  float PowerSupplyVoltage;
} VOLTAGE_DIVIDER_Handle_TypeDef;

/* Public define -------------------------------------------------------------*/
#define ADC_BIT_RES      12      // [bits]
#define ADC_REG_MAX      (float)((1ul << ADC_BIT_RES) - 1)
#define ADC_VOLTAGE_MAX  3300.0f // [mV]

#define ADC1_NUMBER_OF_CONV  0
#define ADC1_TIMEOUT         1 // [ms]

#define DAC_BIT_RES      12      // [bits]
#define DAC_REG_MAX      (float)((1ul << DAC_BIT_RES) - 1)
#define DAC_VOLTAGE_MAX  3300.0f // [mV]

/* Public macro --------------------------------------------------------------*/

/**
 * @brief ADC data register to voltage in millivolts.
 * @param[in] reg : Data register
 * @return Input voltage in millivolts
 */
#define ADC_REG2VOLTAGE(reg) (__LINEAR_TRANSFORM((float)reg,         \
                                               0.0f, ADC_REG_MAX,    \
                                               0.0f, ADC_VOLTAGE_MAX))

/**
 * @brief ADC voltage in volts to data register.
 * @param[in] vol : Output voltage in millivolts
 * @return Data register
 */
#define ADC_VOLTAGE2REG(vol) (uint16_t)(__LINEAR_TRANSFORM(vol,      \
                                               0.0, ADC_VOLTAGE_MAX, \
                                               0.0, ADC_REG_MAX      ))

/**
 * @brief DAC data register to voltage in millivolts.
 * @param[in] reg : Data register
 * @return Input voltage in millivolts
 */
#define DAC_REG2VOLTAGE(reg) (__LINEAR_TRANSFORM((float)(reg),  \
                                0.0f, DAC_REG_MAX,              \
                                0.0f, DAC_VOLTAGE_MAX))

/**
 * @brief DAC voltage in volts to data register.
 * @param[in] vol : Output voltage in millivolts
 * @return Data register value
 */
#define DAC_VOLTAGE2REG(vol) (uint16_t)(__LINEAR_TRANSFORM((vol), \
                                0.0, DAC_VOLTAGE_MAX,             \
                                0.0, DAC_REG_MAX      ))

/**
 * @brief Saturates DAC voltage in range 0 to 3300 mV
 * @param[in] vol : Voltage in millivolts
 * @return Saturated voltage in millivolts
 */
#define DAC_VOLTAGE_SAT(vol) __SATURATION((vol), 0.0f, DAC_VOLTAGE_MAX)

/* Public variables ----------------------------------------------------------*/

/* Public function prototypes ------------------------------------------------*/
/**
 * @brief Computes pull-down resistance of given voltage divider from output voltage
 * @param[in/out] hvd : Voltage divider handle
 * @param[in] voltage : Output voltage of voltage divider in millivolts
 * @return Pull-down resistance in ohms. 
 */
float VOLTAGE_DIVIDER_Read_R_DOWM(VOLTAGE_DIVIDER_Handle_TypeDef* hvd, float voltage);

/**
 * @brief Computes pull-up resistance of given voltage divider from output voltage
 * @param[in/out] hvd : Voltage divider handle
 * @param[in] voltage : Output voltage of voltage divider in millivolts
 * @return Pull-up resistance in ohms. 
 */
float VOLTAGE_DIVIDER_Read_R_UP(VOLTAGE_DIVIDER_Handle_TypeDef* hvd, float voltage);

#endif /* INC_AIO_H_ */
