/**
  ******************************************************************************
  * @file     : ldr.c
  * @author   : AW    Adrian.Wojcik@put.poznan.pl
  * @version  : 1.0.5
  * @date     : Dec 2, 2024
  * @brief    : LDR / photoresistor components driver
  *
  ******************************************************************************
  */

/* Private includes ----------------------------------------------------------*/
#include <math.h>
#include "ldr.h"

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
 * @brief Computes illuminance from voltage drop on LDR using Gamma parameter.
 * @param[in/out] hvd : LDR (Gamma parameter) handle
 * @param[in] voltage : Output voltage of voltage divider in millivolts
 * @return Illuminance in luxes 
 */
float LDR_Gamma_ReadIlluminance_lx(LDR_Gamma_Handle_TypeDef* hldr, unsigned int voltage)
{
  hldr->R = VOLTAGE_DIVIDER_Read_R_DOWM(hldr->VoltageDivider, voltage) - hldr->Roffset;
  float I = 10*powf(hldr->R10lx, 1.0f/hldr->gamma)/powf(hldr->R, 1.0f/hldr->gamma);
  return I;
}
