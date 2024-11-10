/**
  ******************************************************************************
  * @file    lamp.c
  * @author  AW       Adrian.Wojcik@put.poznan.pl
  * @version 1.0
  * @date    30-Oct-2020
  * @brief   Simple dimmer (incandescent light bulb controller board) driver
  *          library.
  *
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "lamp.h"

/* Typedef -------------------------------------------------------------------*/

/* Define --------------------------------------------------------------------*/

/* Macro ---------------------------------------------------------------------*/

//! For timer with base frequency (ARR = 0) of 1 MHz (period of 1 us)
#define __LAMP_DEG_TO_MICROSECONDS(A) ((1000000ul*A)/(2ul*LAMP_LINE_FREQ*180ul))

//! Simple software delay
#define __LAMP_SOFT_DELAY(N) for(uint32_t i = 0; i < N; i++){ asm("nop"); }

/* Private variables ---------------------------------------------------------*/

/* Public variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private function ----------------------------------------------------------*/

/* Public function -----------------------------------------------------------*/
/**
 * @brief Start lamp controller firing angle timer.
 * @param[in] hlamp : Lamp handler
 * @return None
 */
void LAMP_StartFiringAngleTimer(LAMP_HandleTypeDef* hlamp)
{
  // Disable EXTI on lamps SYNC line
  HAL_NVIC_DisableIRQ(hlamp->EXTI_IRQn);

  // Clear counter, compute and set timer ARR value
  __HAL_TIM_SET_COUNTER(hlamp->FiringAngleTimer, 0);
  uint32_t CounterPeriod = __LAMP_DEG_TO_MICROSECONDS(hlamp->TriacFiringAngle);
  __HAL_TIM_SET_AUTORELOAD(hlamp->FiringAngleTimer, CounterPeriod);

  // Start timer in non-blocking mode
  HAL_TIM_Base_Start_IT(hlamp->FiringAngleTimer);
}

/**
 * @brief Stop lamp controller firing angle timer.
 * @param[in] hlamp : Lamp handler
 * @return None
 */
void LAMP_StopFiringAngleTimer(LAMP_HandleTypeDef* hlamp)
{
  // Enable EXTI on lamps SYNC line
  HAL_NVIC_EnableIRQ(hlamp->EXTI_IRQn);

  // Stop timer in non-blocking mode
  HAL_TIM_Base_Stop_IT(hlamp->FiringAngleTimer);
}

/**
 * @brief TRIAC firing procedure: starts TRIAC pulse (high for short period >100us).
 * @param[in] hlamp : Lamp handler
 * @return None
 */
void LAMP_StartPulseTimer(LAMP_HandleTypeDef* hlamp)
{
  // Set TRIAC output
  HAL_GPIO_WritePin(hlamp->TRIAC_Port, hlamp->TRIAC_Pin, GPIO_PIN_SET);

  // Clear counter
  __HAL_TIM_SET_COUNTER(hlamp->PulseTimer, 0);

  // Start timer in non-blocking mode
  HAL_TIM_Base_Start_IT(hlamp->PulseTimer);
}

/**
 * @brief TRIAC firing procedure: stop TRIAC pulse.
 * @param[in] hlamp : Lamp handler
 * @return None
 */
void LAMP_StopPulseTimer(LAMP_HandleTypeDef* hlamp)
{
  // Clear TRIAC output to high state
  HAL_GPIO_WritePin(hlamp->TRIAC_Port, hlamp->TRIAC_Pin, GPIO_PIN_RESET);

  // Start timer in non-blocking mode
  HAL_TIM_Base_Stop_IT(hlamp->PulseTimer);
}

/**
 * @brief Incandescent light bulb firing angle control
 * @param[out] hlamp  : Lamp handler
 * @param[in]  anlgle : Light bulb firing angle expressed in degrees <TriacFiringAngleMin-Max>
 * @return None
 */
void LAMP_WriteFiringAngle(LAMP_HandleTypeDef* hlamp, float anlgle)
{
  // Saturate firing angle
  if(anlgle > hlamp->TriacFiringAngleMax)
    hlamp->TriacFiringAngle = hlamp->TriacFiringAngleMax;
  else if(anlgle < hlamp->TriacFiringAngleMin)
    hlamp->TriacFiringAngle = hlamp->TriacFiringAngleMin;
  else
    hlamp->TriacFiringAngle = anlgle;
}

/**
 * @brief Incandescent light bulb brightness / power control
 * @param[out] hlamp  : Lamp handler
 * @param[in]  anlgle : Light bulb brightness expressed in percent <0-100>
 * @return None
 */
void LAMP_WriteBrightness(LAMP_HandleTypeDef* hlamp, float brightness)
{
  static const float brightness2angle_LookUpTable[] = {
    #include "lamp_brightness2angle_LookUpTable.csv"
  };
  static const float LookUpTable_Step = (sizeof(brightness2angle_LookUpTable) / sizeof(brightness2angle_LookUpTable[0]) - 1) / 100.0f;

  if(brightness > 100.0f)
    brightness = 100.0f;
  else if (brightness < 0.0f)
    brightness = 0.0f;

  hlamp->TriacFiringAngle = brightness2angle_LookUpTable[(uint32_t)(brightness * LookUpTable_Step)];
}
