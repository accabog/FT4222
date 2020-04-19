/*
 * ft4222.h
 *
 *  Created on: Apr 7, 2019
 *      Author: alexandru.bogdan
 */

#ifndef FT4222_H_
#define FT4222_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ftd2xx.h"
#include "LibFT4222.h"

#include <stdbool.h>

extern FT_HANDLE ftHandleStream;
extern FT_HANDLE ftHandleGpio;

bool ft4222_init(void);
void ft4222_uninit(void);

#ifdef __cplusplus
}
#endif

#endif /* FT4222_H_ */
