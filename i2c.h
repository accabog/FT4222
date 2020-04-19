/*
 * i2c.h
 *
 *  Created on: Apr 4, 2019
 *      Author: alexandru.bogdan
 */

#ifndef I2C_H_
#define I2C_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

bool i2c_init(void);
void i2c_uninit(void);

bool i2c_transfer(uint8_t slaveAddr, const uint8_t *txData, uint32_t txLength, uint8_t *rxData, uint32_t rxLength);

#ifdef __cplusplus
}
#endif

#endif /* I2C_H_ */
