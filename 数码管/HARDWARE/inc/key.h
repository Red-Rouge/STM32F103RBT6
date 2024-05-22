#ifndef _KEY_H
#define _KEY_H

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_tim.h"
#include "delay.h"
#include "led.h"

// PA0 PC8,PC9,PD2

#define Pin(gpiox, pinx) GPIO_ReadInputDataBit(gpiox, pinx)

extern uint8_t key_state;

#define KEY_STATE key_state
#define KEY_CLEAR key_state = 0

#define START TIM_Cmd(TIM2, ENABLE);
#define STOP TIM_Cmd(TIM2, DISABLE)

void KEY_Init(void);
uint8_t KEY_Scan(uint8_t mode);
void EXTI_Config(void);

#endif
