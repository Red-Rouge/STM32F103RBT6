#ifndef _SEG_H
#define _SEG_H

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "misc.h"
#include "delay.h"
#include "usart.h"
#include "rtc.h"
#include "key.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define Pout(gpiox, pinx, val) GPIO_WriteBit(gpiox, pinx, val)

#define HC595_PORT GPIOB
#define HC595_SCLK GPIO_Pin_5
#define HC595_LCLK GPIO_Pin_4
#define HC595_DATA GPIO_Pin_3

#define HC138_PORT GPIOC
#define HC138_A0 GPIO_Pin_10
#define HC138_A1 GPIO_Pin_11
#define HC138_A2 GPIO_Pin_12

void HC595_Init(void);
void HC595_Send_Byte(uint8_t byte);

void HC138_Init(void);
void HC138_SelectOutput(uint8_t output);

///////////////////////

void SEG_Init(void);
void SEG_Display(uint8_t pos, uint8_t date);

//////////////////////
void SEG_Display_Num(uint8_t pos, uint8_t num, uint8_t dot);
void SEG_Display_Underline(uint8_t pos);
void SEG_Display_Hozline(uint8_t pos);

void SEG_Display_Format(const char *__format, ...);
void SEG_Clear(void);

void TIME_Set_Task(void);

#endif
