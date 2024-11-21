/**
  ******************************************************************************
  * @file     : aio.c
  * @author   : AW    Adrian.Wojcik@put.poznan.pl
  * @version  : 1.0.0
  * @date     : Nov 21, 2024
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
 * @brief TODO
 */
float VOLTAGE_DIVIDER_Read_R_DOWM(VOLTAGE_DIVIDER_Handle_TypeDef* hvd, float voltage)
{
  if(voltage == 0.0f)
    hvd->R_down = 0;
  else
    hvd->R_down = (hvd->R_up)/(hvd->Gain * hvd->PowerSupplyVoltage / voltage - 1.0f);
  return hvd->R_down;
}


