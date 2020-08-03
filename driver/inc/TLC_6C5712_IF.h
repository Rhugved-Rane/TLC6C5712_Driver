/*
 * TLC_6C5712_IF.h
 *
 *  Created on: 01-Aug-2020
 *      Author: 105317135
 */

#ifndef LEDIF_TLC_6C5712_IF_H_
#define LEDIF_TLC_6C5712_IF_H_

#include <stdint.h>
#include <status.h>
#include "LED_SPI_Com2.h"
#include "Led_Service.h"

#define WRITE_CORR0 			0x46U
#define WRITE_CORR1 			0x47U
#define WRITE_CORR2 			0x48U
#define WRITE_CORR3 			0x49U
#define WRITE_CORR4 			0x4AU
#define WRITE_CORR5 			0x4BU
#define WRITE_CORR6 			0x4CU
#define WRITE_CORR7 			0x4DU
#define WRITE_CORR8 			0x4EU
#define WRITE_CORR9 			0x4FU
#define WRITE_CORR10 			0x50U
#define WRITE_CORR11 			0x51h

#define WRITE_CH_ON_MASK0		0x52U
#define WRITE_CH_ON_MASK1		0x53U

#define READ_CH_ON_MASK0		0x92U
#define READ_CH_ON_MASK1		0x93U

#define CH00_CH06_MASK			0x01U
#define CH01_CH07_MASK			0x02U
#define CH02_CH08_MASK			0x04U
#define CH03_CH09_MASK			0x08U
#define CH04_CH10_MASK			0x10U
#define CH05_CH11_MASK			0x20U

#define ALL_CH_ON_MASK			0x00U
#define ALL_CH_OFF_MASK			(CH00_CH06_MASK|CH01_CH07_MASK|CH02_CH08_MASK|CH03_CH09_MASK|CH04_CH10_MASK|CH05_CH11_MASK)

#define DEFAULT_CH_BRIGHTNESS	0x7FU

/*!
 * @brief Read LED driver register over SPI bus.
 *
 * This function reads value from a particular register of LED driver.
 *
 * @param addr Address of LED driver register
 * @param valPtr Address to store the read value
 * @return Read operation status
 */
status_t DRV_readLedDriverRegister(uint8_t addr, uint8_t *valPtr);

/*!
 * @brief Write LED driver register over SPI bus.
 *
 * This function writes a given value at a giver register of the LED driver.
 * It also reads back the value from the corrosponding read register
 * (which is 0x40 addresses apart for this LED driver chip), compares it,
 * and returns the result.
 *
 * @param addr Address of LED driver register
 * @param val Value to be written
 * @return Write operation status
 */
status_t DRV_writeLedDriverRegister(uint8_t addr, uint8_t val);

/*!
 * @brief Initializes LED driver registers.
 *
 * This function initializes basic register configurations of the LED driver.
 * It also sets current value to maximum for all used channels.
 *
 * @return Initialization operation status
 */
status_t DRV_LEDDriverInit(void);

#endif /* LEDIF_TLC_6C5712_IF_H_ */
