#ifndef _BEEP_H
#define _BEEP_H

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#define BEEP_PIN GPIO_Pin_8
#define BEEP_PORT GPIOB

#define BEEP_OFF GPIO_SetBits(BEEP_PORT, BEEP_PIN)
#define BEEP_ON GPIO_ResetBits(BEEP_PORT, BEEP_PIN)

void BEEP_Init(void);

#endif
