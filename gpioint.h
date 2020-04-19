/*
 * gpioint.h
 *
 *  Created on: Apr 7, 2019
 *      Author: alexandru.bogdan
 */

#ifndef GPIOINT_H_
#define GPIOINT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool gpioint_init(void);
void gpioint_uninit(void);

#ifdef __cplusplus
}
#endif

#endif /* GPIOINT_H_ */
