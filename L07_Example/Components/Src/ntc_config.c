/**
  ******************************************************************************
  * @file     : ntc_config.c
  * @author   : AW    Adrian.Wojcik@put.poznan.pl
  * @version  : 1.0.0
  * @date     : Nov 22, 2024
  * @brief    : NTC thermistor configuration file
  *
  ******************************************************************************
  */

/* Private includes ----------------------------------------------------------*/
#include "ntc.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
VOLTAGE_DIVIDER_Handle_TypeDef hvd_ntc1 = {
    .R_up = 50000.0f, .Gain = 1.0f, .PowerSupplyVoltage = 3300.0f
};

/* Public variables ----------------------------------------------------------*/
NTC_SteinhartHart_Handle_TypeDef hntc1_sh = {
    .VoltageDivider = &hvd_ntc1,
    .Roffset = 150,
    .A = 0.001129148,
    .B = 0.000234125,
    .C = 0.0000000876741
};

NTC_Beta_Handle_TypeDef hntc1_beta = {
    .VoltageDivider = &hvd_ntc1,
    .Roffset = 150,
    .R25degC = 8500, /* 7000 */
    .beta = 4200
};

/* Private function prototypes -----------------------------------------------*/

/* Public function prototypes ------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/* Public functions ----------------------------------------------------------*/
