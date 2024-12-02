/**
  ******************************************************************************
  * @file		: ldr.h
  * @author  	: AW		Adrian.Wojcik@put.poznan.pl
  * @version  	: 1.0.5
  * @date     	: Dec 2, 2024
  * @brief   	: LDR / photoresistor components driver
  *
  ******************************************************************************
  */

#ifndef INC_LDR_H_
#define INC_LDR_H_

/* Public includes -----------------------------------------------------------*/
#include "aio.h"

/* Public typedef ------------------------------------------------------------*/
/**
 * @brief Photoresistor (LDR) model structure base on Gamma parameter equation (1st order). 
 */
typedef struct {
  float R;
  VOLTAGE_DIVIDER_Handle_TypeDef* VoltageDivider;
  float Roffset;
  float R10lx;
  float gamma;
} LDR_Gamma_Handle_TypeDef;

/* Public define -------------------------------------------------------------*/

/* Public macro --------------------------------------------------------------*/

/* Public variables ----------------------------------------------------------*/

/* Public function prototypes ------------------------------------------------*/
/**
 * @brief Computes illuminance from voltage drop on LDR using Gamma parameter.
 * @param[in/out] hvd : LDR (Gamma parameter) handle
 * @param[in] voltage : Output voltage of voltage divider in millivolts
 * @return Illuminance in luxes 
 */
float LDR_Gamma_ReadIlluminance_lx(LDR_Gamma_Handle_TypeDef* hldr, unsigned int voltage);

#endif /* INC_LDR_H_ */
