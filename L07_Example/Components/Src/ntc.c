/**
  ******************************************************************************
  * @file     : ntc.c
  * @author   : AW    Adrian.Wojcik@put.poznan.pl
  * @version  : 1.0.5
  * @date     : Dec 2, 2024
  * @brief    : NTC thermistor components driver
  *
  ******************************************************************************
  */

/* Private includes ----------------------------------------------------------*/
#include <math.h>
#include "ntc.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Public variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Public function prototypes ------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/* Public functions ----------------------------------------------------------*/
/**
 * @brief Computes temperature from voltage drop on NTC thermistor using Steinhart-Hart equation.
 * @param[in/out] hvd : NTC thermistor (Steinhart-Hart equation) handle
 * @param[in] voltage : Output voltage of voltage divider in millivolts
 * @return Temperature in deg C
 */
float NTC_SteinhartHart_ReadTemperature_degC(NTC_SteinhartHart_Handle_TypeDef* hntc, unsigned int voltage)
{
  hntc->R = VOLTAGE_DIVIDER_Read_R_DOWM(hntc->VoltageDivider, voltage) - hntc->Roffset;
  float lnR = logf(hntc->R);
  float T = (1.0f / (hntc->A + hntc->B*lnR + hntc->C*lnR*lnR*lnR)) - 273.15;
  return T;
}

/**
 * @brief Computes temperature from voltage drop on NTC thermistor using Beta parameter.
 * @param[in/out] hvd : NTC thermistor (Beta parameter) handle
 * @param[in] voltage : Output voltage of voltage divider in millivolts
 * @return Temperature in deg C
 */
float NTC_Beta_ReadTemperature_degC(NTC_Beta_Handle_TypeDef* hntc, unsigned int voltage)
{
  hntc->R = VOLTAGE_DIVIDER_Read_R_DOWM(hntc->VoltageDivider, voltage) - hntc->Roffset;;
  float T = (1.0f / ((1.0f/298.15)+(1/hntc->beta)*logf(hntc->R/hntc->R25degC))) - 273.15;
  return T;
}
