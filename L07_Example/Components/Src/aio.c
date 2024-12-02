/**
  ******************************************************************************
  * @file     : aio.c
  * @author   : AW    Adrian.Wojcik@put.poznan.pl
  * @version  : 1.3.5
  * @date     : Dec 2, 2024
  * @brief    : Analog inputs/outputs components.
  *
  ******************************************************************************
  */

/* Public includes -----------------------------------------------------------*/
#include "aio.h"

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
 * @brief Computes pull-down resistance of given voltage divider from output voltage
 * @param[in/out] hvd : Voltage divider handle
 * @param[in] voltage : Output voltage of voltage divider in millivolts
 * @return Pull-down resistance in ohms. 
 */
float VOLTAGE_DIVIDER_Read_R_DOWM(VOLTAGE_DIVIDER_Handle_TypeDef* hvd, float voltage)
{
  if(voltage == 0.0f)
    hvd->R_down = 0;
  else
    hvd->R_down = (hvd->R_up)/(hvd->Gain * hvd->PowerSupplyVoltage / voltage - 1.0f);
  return hvd->R_down;
}

/**
 * @brief Computes pull-up resistance of given voltage divider from output voltage
 * @param[in/out] hvd : Voltage divider handle
 * @param[in] voltage : Output voltage of voltage divider in millivolts
 * @return Pull-up resistance in ohms. 
 */
float VOLTAGE_DIVIDER_Read_R_UP(VOLTAGE_DIVIDER_Handle_TypeDef* hvd, float voltage)
{
  hvd->R_up = (hvd->R_down)*(hvd->Gain * hvd->PowerSupplyVoltage / voltage - 1.0f);
  return hvd->R_up;
}

