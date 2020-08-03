/*
 * Led_Control.h
 *
 *  Created on: 01-Aug-2020
 *      Author: 105317135
 */

#ifndef LED_LED_CONTROL_H_
#define LED_LED_CONTROL_H_

#include <stdint.h>
#include <status.h>
#include "TLC_6C5712_IF.h"

#define MAX_NO_LEDS				2U
#define LED_OFF					{0U, 0U, 0U}
#define LED_COLOR1				{255U, 0U, 0U}
#define LED_COLOR2				{0U, 255U, 0U}
#define LED_COLOR3				{0U, 0U, 255U}
#define LED_COLOR4				{255U, 255U, 0U}
#define LED_COLOR5				{255U, 0U, 255U}

#define LED1_RED_MASK			CH00_CH06_MASK
#define LED1_GREEN_MASK			CH01_CH07_MASK
#define LED1_BLUE_MASK			CH02_CH08_MASK

#define LED2_RED_MASK			CH03_CH09_MASK
#define LED2_GREEN_MASK			CH04_CH10_MASK
#define LED2_BLUE_MASK			CH05_CH11_MASK

typedef enum
{
    DISP_MODE_ON = 0U,
    DISP_MODE_OFF,
    DISP_MODE_BLINK,
    DISP_MODE_TOG,
    DISP_MODE_MAX
}DISP_MODE_E;

typedef enum
{
    DISP_LED_ON = 0U,
    DISP_LED_OFF,
    DISP_LED_TOG
}DISP_LED_CTRL_TYPE_E;

typedef enum
{
    DISP_LED_INDEX_1 = 0U,
    DISP_LED_INDEX_2,
    DISP_LED_NUM
}DISP_LED_INDEX_E;

typedef enum
{
	Sleep_Ind = 0,
	Standby_Ind,
	Charge_InProg_Ind,
	Charging_Comp_Ind,
	Error_Ind,
	FOD_Ind,
	Device_Disp_Ind,
	MAX_MODE
}DISP_LED_INDC;

typedef struct
{
	uint8_t LED_RED;
	uint8_t LED_GREEN;
	uint8_t LED_BLUE;
}DISP_LED_COLOR_CODE_E;

/*!
 * @brief Initializes the LED service.
 *
 * This function initializes the LED service module. It also initializes the
 * underlying driver module.
 *
 * @return Initialization operation status
 */
status_t SRVC_initLEDService(void);

/*!
 * @brief Sets the state for a particular LED.
 *
 * This function sets the state for a particular LED by determining the
 * appropriate channel mask values.
 *
 * @param byLedId
 * @param CtrlType
 * @param ledColor
 * @return
 */
status_t SRVC_setLEDState(DISP_LED_INDEX_E byLedId, DISP_LED_CTRL_TYPE_E CtrlType, DISP_LED_COLOR_CODE_E *ledColor);

/*!
 * @brief Sets the appropriate R-G-B channels for an LED.
 *
 * This function sets appropriate R-G-B channels for an LED. It reads the
 * current mask register value, updates it locally, and writes it back.
 *
 * @param CtrlType
 * @param chEnMask
 * @return
 */
status_t SRVC_setLEDChannels(DISP_LED_CTRL_TYPE_E CtrlType, uint8_t chEnMask);

#endif /* LED_LED_CONTROL_H_ */
