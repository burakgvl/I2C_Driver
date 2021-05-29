/*
 * I2C_Driver.h
 *
 *  Created on: 7 Mar 2021
 *      Author: Asus
 */

#ifndef MYDRIVER_INC_I2C_DRIVER_H_
#define MYDRIVER_INC_I2C_DRIVER_H_

#include "stm32f4xx.h"

void I2C_Start();
void I2C_SlaveAdress(uint32_t SlaveAddr);
void I2C_Write(uint32_t data);
void I2C_Stop();
void I2C_Read(uint8_t Address, uint8_t *buffer, uint8_t size);

#endif /* MYDRIVER_INC_I2C_DRIVER_H_ */
