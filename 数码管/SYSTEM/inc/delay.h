#ifndef DELAY_H
#define DELAY_H

#include "stm32f10x.h" //必须添加

void SysTick_Init(u8 SYSCLK);
void delay_ms(uint32_t nms);
void delay_us(uint32_t nus);

void TimingDelay_Decrement(void);

#endif
