/*
 * I2C_Driver.c
 *
 *  Created on: 7 Mar 2021
 *      Author: Asus
 */

#include "I2C_Driver.h"

void I2C_Start(){
	I2C1->CR1 |= (1<<10);  				// Enable the ACK
	I2C1->CR1 |= 1 << 8;				// Start bit 1
	while(!(I2C1->SR1 & (1<<0)));		//Wait SB bit to set
}

void I2C_SlaveAdress(uint32_t SlaveAddr){
	volatile int ClearStatus;
	//while (!(I2C1->SR1 & (1<<7))); 				 //TxE bitinin 1 olmasını bekle(Data register empty)
	I2C1->DR = SlaveAddr;					//Slave adresi yaz
	while(!(I2C1->SR1 & (1<<1)));  				//Addr biti 1 olana kadar bekle
	ClearStatus = I2C1->SR1 | I2C1->SR2;	//SR1 ve SR2 okunarak ADDR biti temizlenir.
	while(!(I2C1->SR2 & (1<<0)));				// Master bitini bekle
}

void I2C_Write(uint32_t data){
	while (!(I2C1->SR1 & (1<<7))); 				 //TxE bitinin 1 olmasını bekle(Data register empty)
	I2C1->DR = data;					//Slave adresi yaz
	while(!(I2C1->SR1 & (1<<2)));				//BTF bitinin 1 olmasını bekle(Bayt transfer bitisi)
}

void I2C_Stop(){
	I2C1->CR1 |= (1<<9);
}


void I2C_Read(uint8_t Address, uint8_t *buffer, uint8_t size ){
	int remaining = size;
	volatile int temp;
	/**** STEP 1 ****/
		if (size == 1)
		{
			/**** STEP 1-a ****/
			I2C1->DR = Address;  //  send the address
			while (!(I2C1->SR1 & (1<<1)));  // wait for ADDR bit to set

			/**** STEP 1-b ****/
			I2C1->CR1 &= ~(1<<10);  // clear the ACK bit
			temp = I2C1->SR1 | I2C1->SR2;  // read SR1 and SR2 to clear the ADDR bit.... EV6 condition
			I2C1->CR1 |= (1<<9);  // Stop I2C

			/**** STEP 1-c ****/
			while (!(I2C1->SR1 & (1<<6)));  // wait for RxNE to set

			/**** STEP 1-d ****/
			buffer[size-remaining] = I2C1->DR;  // Read the data from the DATA REGISTER

		}

	/**** STEP 2 ****/
		else
		{
			/**** STEP 2-a ****/
			I2C1->DR = Address;  //  send the address
			while (!(I2C1->SR1 & (1<<1)));  // wait for ADDR bit to set

			/**** STEP 2-b ****/
			temp = I2C1->SR1 | I2C1->SR2;  // read SR1 and SR2 to clear the ADDR bit

			while (remaining>2)
			{
				/**** STEP 2-c ****/
				while (!(I2C1->SR1 & (1<<6)));  // wait for RxNE to set

				/**** STEP 2-d ****/
				buffer[size-remaining] = I2C1->DR;  // copy the data into the buffer

				/**** STEP 2-e ****/
				I2C1->CR1 |= 1<<10;  // Set the ACK bit to Acknowledge the data received

				remaining--;
			}

			// Read the SECOND LAST BYTE
			while (!(I2C1->SR1 & (1<<6)));  // wait for RxNE to set
			buffer[size-remaining] = I2C1->DR;

			/**** STEP 2-f ****/
			I2C1->CR1 &= ~(1<<10);  // clear the ACK bit

			/**** STEP 2-g ****/
			I2C1->CR1 |= (1<<9);  // Stop I2C

			remaining--;

			// Read the Last BYTE
			while (!(I2C1->SR1 & (1<<6)));  // wait for RxNE to set
			buffer[size-remaining] = I2C1->DR;  // copy the data into the buffer
		}
}
