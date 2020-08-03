/*
 * TLC_6C5712_IF.c
 *
 *  Created on: 01-Aug-2020
 *      Author: 105317135
 */

#include "TLC_6C5712_IF.h"

/*FUNCTION**********************************************************************
 *
 * Function Name : DRV_readLedDriverRegister
 * Description   : Read LED driver register over SPI bus.
 *
 * This function reads value from a particular register of LED driver.
 * Implements : LPSPI_DRV_MasterTransferBlocking
 *END**************************************************************************/
status_t DRV_readLedDriverRegister(uint8_t addr, uint8_t *valPtr)
{
	status_t retVal = STATUS_SUCCESS;
	uint8_t arrTx[4U] = {0x00U, addr};
	uint8_t arrRx[4U] = {0x00U, 0x00U};

	retVal  = LPSPI_DRV_SetPcs(LED_SPI_COM2, LPSPI_PCS0, LPSPI_ACTIVE_LOW);
	retVal |= LPSPI_DRV_MasterTransferBlocking(LED_SPI_COM2, arrTx, arrRx, 2U, 200U);
	retVal |= LPSPI_DRV_SetPcs(LED_SPI_COM2, LPSPI_PCS0, LPSPI_ACTIVE_HIGH);

	retVal  = LPSPI_DRV_SetPcs(LED_SPI_COM2, LPSPI_PCS0, LPSPI_ACTIVE_LOW);
	retVal |= LPSPI_DRV_MasterTransferBlocking(LED_SPI_COM2, arrTx, arrRx, 2U, 200U);
	retVal |= LPSPI_DRV_SetPcs(LED_SPI_COM2, LPSPI_PCS0, LPSPI_ACTIVE_HIGH);

	*valPtr = arrRx[0U];

	return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : DRV_writeLedDriverRegister
 * Description   : Write LED driver register over SPI bus.
 *
 * This function writes a given value at a giver register of the LED driver.
 * It also reads back the value from the corrosponding read register
 * (which is 0x40 addresses apart for this LED driver chip), compares it,
 * and returns the result.
 * Implements : LPSPI_DRV_MasterTransferBlocking
 *END**************************************************************************/
status_t DRV_writeLedDriverRegister(uint8_t addr, uint8_t val)
{
	status_t retVal = STATUS_SUCCESS;
	uint8_t readBackReg = 0U;
	uint8_t arrTx[2U] = {val, addr};

	retVal  = LPSPI_DRV_SetPcs(LED_SPI_COM2, LPSPI_PCS0, LPSPI_ACTIVE_LOW);
	retVal |= LPSPI_DRV_MasterTransferBlocking(LED_SPI_COM2, arrTx, NULL, 2U, 100U);
	retVal |= LPSPI_DRV_SetPcs(LED_SPI_COM2, LPSPI_PCS0, LPSPI_ACTIVE_HIGH);

	/* Read back value from address (addr + 0x40) */
	retVal |= DRV_readLedDriverRegister((addr + 0x40U), &readBackReg);

	/* Check if SPI transfers failed OR write operation failed */
	retVal = (retVal || (val != readBackReg));

	return retVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : DRV_LEDDriverInit
 * Description   : Initializes LED driver registers.
 *
 * This function initializes basic register configurations of the LED driver.
 * It also sets current value to maximum for all used channels.
 * Implements : LPSPI_DRV_MasterTransferBlocking
 *END**************************************************************************/
status_t DRV_LEDDriverInit(void)
{
	status_t retVal = STATUS_SUCCESS;
	uint8_t buffVal;

	/* Initialize LPSPI2 (LED_SPI) HW port */
	retVal  = LPSPI_DRV_MasterInit(LED_SPI_COM2, &LED_SPI_Com2State, &LED_SPI_Com2_MasterConfig0);

	/* Set all fault mask registers value HIGH to mask all faults */
	retVal |= DRV_writeLedDriverRegister(0x66U, 0x3FU);

	/* Write the [FORCE_ERR] register to check the ERR feedback circuit connection */
	retVal |= DRV_writeLedDriverRegister(0x67U, 0x02U);
	/* Clear the [FORCE_ERR] register to check the ERR feedback circuit connection */
	retVal |= DRV_writeLedDriverRegister(0x67U, 0x00U);

	/* Clear the [RESET_POR] POR flag */
	DRV_writeLedDriverRegister(0x61U, 0x69U);
	/* Reset the fault status [RESET_STATUS] registers */
	DRV_writeLedDriverRegister(0x62U, 0x66U);

	/* Check the status registers to confirm no faults */
	retVal |= DRV_readLedDriverRegister(0xA2U, &buffVal);
	retVal |= DRV_readLedDriverRegister(0xA3U, &buffVal);

	/* Unmask ERROR_MASK */
	retVal |= DRV_writeLedDriverRegister(0x66U, 0x00U);

	/* Turn on or turn off output channels by writing the [WRITE_CH_ON_MASKx] registers */
	retVal |= DRV_writeLedDriverRegister(WRITE_CH_ON_MASK0, ALL_CH_OFF_MASK);
	retVal |= DRV_writeLedDriverRegister(WRITE_CH_ON_MASK1, ALL_CH_OFF_MASK);

	/* Configure current values to maximum for all used channels */
	retVal |= DRV_writeLedDriverRegister(WRITE_CORR0, DEFAULT_CH_BRIGHTNESS);
	retVal |= DRV_writeLedDriverRegister(WRITE_CORR1, DEFAULT_CH_BRIGHTNESS);
	retVal |= DRV_writeLedDriverRegister(WRITE_CORR2, DEFAULT_CH_BRIGHTNESS);
	retVal |= DRV_writeLedDriverRegister(WRITE_CORR3, DEFAULT_CH_BRIGHTNESS);
	retVal |= DRV_writeLedDriverRegister(WRITE_CORR4, DEFAULT_CH_BRIGHTNESS);
	retVal |= DRV_writeLedDriverRegister(WRITE_CORR5, DEFAULT_CH_BRIGHTNESS);

	return retVal;
}
