/** @file 		drv_system.h
 *  @brief
 *  	This file contains all the basic functions to run the timing
 *  	of the system as well as initialize the system.
 *
 *
 *  @author 	Jeremy Wolfe
 *  @date 		03 MAR 2022
 */

#ifndef __DRV_SYSTEM_H__
#define __DRV_SYSTEM_H__

#include "main.h"

/* Function Prototypes */
void cycleCounterInit(void);

void delayMicroseconds(uint32_t us);
void delay(uint32_t ms);

uint32_t micros(void);
uint32_t millis(void);


#endif /* DRV_DRV_SYSTEM_H_ */
