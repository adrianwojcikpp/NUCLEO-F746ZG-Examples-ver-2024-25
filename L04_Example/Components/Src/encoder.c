
/**
  ******************************************************************************
  * @file		  : encoder.c
  * @author  	: AW		Adrian.Wojcik@put.poznan.pl
  * @version 	: 1.3.1
  * @date     : Nov 08, 2022
  * @brief   	: Simple rotary encoder driver library
  *
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "encoder.h"

/* Typedef -------------------------------------------------------------------*/

/* Define --------------------------------------------------------------------*/

/* Macro ---------------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Public variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private function ----------------------------------------------------------*/

/* Public function -----------------------------------------------------------*/

#ifdef ENC_HARDWARE_COUNTER

/**
 * @brief Rotary quadrature encoder hardware initialization.
 * @param[in] henc : Encoder handler
 * @return None
 */
void ENC_Init(ENC_Handle_TypeDef* henc)
{
  // TODO
}

/**
 * @brief Reads rotary quadrature encoder hardware counter.
 * @param[in] henc : Encoder handler
 * @return Current counter value
 */
uint32_t ENC_ReadCounter(ENC_Handle_TypeDef* henc)
{
  // TODO
  return 0;
}

/**
 * @brief Writes to rotary quadrature encoder hardware counter.
 * @param[in] henc    : Encoder handler
 * @param[in] counter : Current counter value
 */
void ENC_WriteCounter(ENC_Handle_TypeDef* henc, uint32_t counter)
{
  henc->Counter = counter * henc->TicksPerStep;
  __HAL_TIM_SET_COUNTER(henc->Timer, henc->Counter);
}

#else

/**
 * @brief Rotary quadrature encoder software counter update procedure.
 * @param[in] henc : Encoder handler
 * @return Current counter value
 */
int32_t ENC_UpdateCounter(ENC_Handle_TypeDef* henc)
{
  GPIO_PinState dt = HAL_GPIO_ReadPin(henc->DT_Port, henc->DT_Pin);
  int32_t cnt = henc->Counter;
  if(dt == GPIO_PIN_RESET)
    henc->Counter = (henc->Counter >= henc->CounterMax) ? henc->CounterMax : (henc->Counter + henc->CounterStep);
  else
    henc->Counter = (henc->Counter <= henc->CounterMin) ? henc->CounterMin : (henc->Counter - henc->CounterStep);
  henc->CounterInc = (henc->Counter > cnt);
  henc->CounterDec = (henc->Counter < cnt);
  return henc->Counter;
}

#endif
