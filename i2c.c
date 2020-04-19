/*
 * i2c.c
 *
 *  Created on: Apr 4, 2019
 *      Author: alexandru.bogdan
 */

#include "i2c.h"

#include "ft4222.h"

#include <stdio.h>
#include <string.h>

enum I2C_TransferType
{
	I2C_TRANSFER_TX = 0,
	I2C_TRANSFER_RX,
	I2C_TRANSFER_TXRX,
};

bool i2c_init(void)
{
	FT_STATUS ftStatus;

	if(!ftHandleStream)
	{
		printf("FT4222 A not initialized!\n");
		return false;
	}

	printf("Init FT4222 as I2C master\n");
	ftStatus = FT4222_I2CMaster_Init(ftHandleStream, 400);
	if (FT_OK == ftStatus)
	{
		return true;
	}

    printf("Init FT4222 as I2C master device failed!\n");

	return false;
}

bool i2c_transfer(uint8_t slaveAddr, const uint8_t *txData, uint32_t txLength, uint8_t *rxData, uint32_t rxLength)
{
	FT_STATUS ftStatus = 0;
	uint16_t sizeTransferred;

	if(!ftHandleStream)
	{
		printf("I2C master not initialized\n");
		return false;
	}

	if(txLength && rxLength)
	{
		ftStatus = FT4222_I2CMaster_WriteEx(ftHandleStream, slaveAddr, START, (uint8_t *)txData, txLength, &sizeTransferred);
		if (FT_OK != ftStatus)
		{
			printf("I2C master write error\n");
		    return false;
		}

		ftStatus = FT4222_I2CMaster_ReadEx(ftHandleStream, slaveAddr, Repeated_START | STOP, rxData, rxLength, &sizeTransferred);
		if (FT_OK != ftStatus)
		{
			printf("I2C master read error\n");
			return false;
		}
	}
	else if(txLength)
	{
		ftStatus = FT4222_I2CMaster_Write(ftHandleStream, slaveAddr, (uint8_t *)txData, txLength, &sizeTransferred);
		if (FT_OK != ftStatus)
		{
			printf("I2C master write error\n");
		    return false;
		}
	}
	else if(rxLength)
	{
		ftStatus = FT4222_I2CMaster_Read(ftHandleStream, slaveAddr, rxData, rxLength, &sizeTransferred);
		if (FT_OK != ftStatus)
		{
			printf("I2C master write error\n");
		    return false;
		}
	}

	return true;
}

void i2c_uninit(void)
{

}

