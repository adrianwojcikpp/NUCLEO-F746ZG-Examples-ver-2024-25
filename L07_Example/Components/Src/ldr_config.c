/**
  ******************************************************************************
  * @file     : ldr_config.c
  * @author   : AW    Adrian.Wojcik@put.poznan.pl
  * @version  : 1.0.0
  * @date     : Nov 22, 2024
  * @brief    : LDR / photoresistor configuration file
  *
  ******************************************************************************
  */

/* Private includes ----------------------------------------------------------*/
#include "ldr.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
VOLTAGE_DIVIDER_Handle_TypeDef hvd_ldr1 = {
    .R_up = 10000.0f, .Gain = 1.0f, .PowerSupplyVoltage = 3300.0f
};

/* Public variables ----------------------------------------------------------*/
LDR_Gamma_Handle_TypeDef hldr1_gamma = {
    .VoltageDivider = &hvd_ldr1,
    .Roffset = 0,
    .R10lx = 8000,
    .gamma = 0.6
};

/* Private function prototypes -----------------------------------------------*/

/* Public function prototypes ------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/* Public functions ----------------------------------------------------------*/
