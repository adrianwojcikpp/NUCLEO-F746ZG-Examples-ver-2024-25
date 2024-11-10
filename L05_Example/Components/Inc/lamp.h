/**
  ******************************************************************************
  * @file    lamp.h
  * @author  AW       Adrian.Wojcik@put.poznan.pl
  * @version 1.0
  * @date    30-Oct-2020
  * @brief   Simple dimmer (incandescent light bulb controller board) driver
  *          library.
  *
  ******************************************************************************
  */
#ifndef INC_LAMP_H_
#define INC_LAMP_H_

/* Config --------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Typedef -------------------------------------------------------------------*/
#define LAMP_TimerType TIM_HandleTypeDef*
#define LAMP_PortType GPIO_TypeDef*
#define LAMP_PinType uint16_t

typedef struct {
  LAMP_TimerType FiringAngleTimer;
  LAMP_TimerType PulseTimer;
  LAMP_PortType SYNC_Port;
  LAMP_PinType SYNC_Pin;
  LAMP_PortType TRIAC_Port;
  LAMP_PinType TRIAC_Pin;
  uint32_t EXTI_IRQn;
  float TriacFiringAngle;
  float TriacFiringAngleMin;
  float TriacFiringAngleMax;
} LAMP_HandleTypeDef; 

/* Define --------------------------------------------------------------------*/
#define LAMP_LINE_FREQ   50ul  // [Hz]
#define LAMP_TRIAC_DELAY 2500  // [ticks]

/* Macro ---------------------------------------------------------------------*/

/* Public variables ----------------------------------------------------------*/

/* Public function prototypes ------------------------------------------------*/
/**
 * @brief Start lamp controller firing angle timer.
 * @param[in] hlamp : Lamp handler
 * @return None
 */
void LAMP_StartFiringAngleTimer(LAMP_HandleTypeDef* hlamp);

/**
 * @brief Stop lamp controller firing angle timer.
 * @param[in] hlamp : Lamp handler
 * @return None
 */
void LAMP_StopFiringAngleTimer(LAMP_HandleTypeDef* hlamp);

/**
 * @brief TRIAC firing procedure: starts TRIAC pulse (high for short period >100us).
 * @param[in] hlamp : Lamp handler
 * @return None
 */
void LAMP_StartPulseTimer(LAMP_HandleTypeDef* hlamp);

/**
 * @brief TRIAC firing procedure: stop TRIAC pulse.
 * @param[in] hlamp : Lamp handler
 * @return None
 */
void LAMP_StopPulseTimer(LAMP_HandleTypeDef* hlamp);

/**
 * @brief Incandescent light bulb firing angle control
 * @param[out] hlamp  : Lamp handler
 * @param[in]  anlgle : Light bulb firing angle expressed in degrees <TriacFiringAngleMin-Max>
 * @return None
 */
void LAMP_WriteFiringAngle(LAMP_HandleTypeDef* hlamp, float anlgle);

/**
 * @brief Incandescent light bulb brightness / power control
 * @param[out] hlamp  : Lamp handler
 * @param[in]  anlgle : Light bulb brightness expressed in percent <0-100>
 * @return None
 */
void LAMP_WriteBrightness(LAMP_HandleTypeDef* hlamp, float brightness);

#endif /* INC_LAMP_H_ */
