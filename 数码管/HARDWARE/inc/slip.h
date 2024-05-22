#ifndef __SLIP_H__
#define __SLIP_H__

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_adc.h"

void SLIP_Init(void);
uint16_t SLIP_Get_Value(void);
uint32_t SLIP_Get(uint16_t times);

#endif
