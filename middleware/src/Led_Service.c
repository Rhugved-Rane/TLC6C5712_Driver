/*
 * Led_Control.c
 *
 *  Created on: 01-Aug-2020
 *      Author: 105317135
 */

#include "Led_Service.h"

/*FUNCTION**********************************************************************
 *
 * Function Name : SRVC_initLEDService
 * Description   : Initializes the LED service.
 *
 * This function initializes the LED service module. It also initializes the
 * underlying driver module.
 * Implements : DRV_LEDDriverInit
 *END**************************************************************************/
status_t SRVC_initLEDService(void)
{
	status_t retVal = STATUS_SUCCESS;

	retVal = DRV_LEDDriverInit();

	return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SRVC_setLEDState
 * Description   : Sets the state for a particular LED.
 *
 * This function sets the state for a particular LED by determining the
 * appropriate channel mask values.
 * Implements : SRVC_setLEDChannels
 *END**************************************************************************/
status_t SRVC_setLEDState(DISP_LED_INDEX_E byLedId, DISP_LED_CTRL_TYPE_E CtrlType, DISP_LED_COLOR_CODE_E *ledColor)
{
  uint8_t chEnMask = ALL_CH_ON_MASK;
  status_t retVal = STATUS_SUCCESS;

  switch(byLedId)
  {
  	  case DISP_LED_INDEX_1:
  	  {
  		  if(ledColor->LED_RED)
  		  {
  			  chEnMask |= LED1_RED_MASK;
  		  }
  		  if(ledColor->LED_GREEN)
  		  {
  			  chEnMask |= LED1_GREEN_MASK;
  		  }
  		  if(ledColor->LED_BLUE)
  		  {
  			  chEnMask |= LED1_BLUE_MASK;
  		  }
  		  if(ALL_CH_ON_MASK == chEnMask)
  		  {
  			  chEnMask |= (LED1_RED_MASK | LED1_GREEN_MASK | LED1_BLUE_MASK);
  		  }
  	  }
  	  break;

  	  case DISP_LED_INDEX_2:
  	  {
  		  if(ledColor->LED_RED)
  		  {
  			  chEnMask |= LED2_RED_MASK;
  		  }
  		  if(ledColor->LED_GREEN)
  		  {
  			  chEnMask |= LED2_GREEN_MASK;
  		  }
  		  if(ledColor->LED_BLUE)
  		  {
  			  chEnMask |= LED2_BLUE_MASK;
  		  }
  		  if(ALL_CH_ON_MASK == chEnMask)
  		  {
  			  chEnMask |= (LED2_RED_MASK | LED2_GREEN_MASK | LED2_BLUE_MASK);
  		  }
  	  }
  	  break;

  	  default:
  		  chEnMask = ALL_CH_OFF_MASK;
  		  break;
  }

  retVal = SRVC_setLEDChannels(CtrlType, chEnMask);

  return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : SRVC_setLEDChannels
 * Description   : Sets the appropriate R-G-B channels for an LED.
 *
 * This function sets appropriate R-G-B channels for an LED. It reads the
 * current mask register value, updates it locally, and writes it back.
 * Implements : DRV_writeLedDriverRegister
 *END**************************************************************************/
status_t SRVC_setLEDChannels(DISP_LED_CTRL_TYPE_E CtrlType, uint8_t chEnMask)
{
	uint8_t inValue = 0;
	status_t retVal = STATUS_SUCCESS;

	switch(CtrlType)
	{
		case DISP_LED_ON:
		{
			/* Read current mask value - Clear required channel bit - Write back */
			retVal |= DRV_readLedDriverRegister(READ_CH_ON_MASK0, &inValue);
			inValue &= ~chEnMask;
			retVal |= DRV_writeLedDriverRegister(WRITE_CH_ON_MASK0, inValue);
		}
		break;

		case DISP_LED_OFF:
		{
			/* Read current mask value - Set required channel bit - Write back */
			retVal |= DRV_readLedDriverRegister(READ_CH_ON_MASK0, &inValue);
			inValue |= chEnMask;
			retVal |= DRV_writeLedDriverRegister(WRITE_CH_ON_MASK0, inValue);
		}
		break;

		case DISP_LED_TOG:
		{
			/* Read current mask value - Toggle required channel bit - Write back */
			retVal |= DRV_readLedDriverRegister(READ_CH_ON_MASK0, &inValue);
			inValue ^= chEnMask;
			retVal |= DRV_writeLedDriverRegister(WRITE_CH_ON_MASK0, inValue);
		}
		break;

		default:
			break;
	}

	return retVal;
}
