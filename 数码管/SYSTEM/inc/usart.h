#ifndef __USART_INIT_CONFIG_H
#define __USART_INIT_CONFIG_H

#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "misc.h"
#include "stdio.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define USART_MAX_LEN 1024 //定义最大接收字节数

#define STR USART_RX_BUF      //接收字符缓存
#define STR_CLEAR BUF_Clear() //清理接收字符缓存
#define STA USART_RX_STA      //清理字符标志,当STA不是0的时候

extern volatile uint8_t USART_RX_BUF[USART_MAX_LEN];
extern volatile uint32_t USART_RX_STA;

void USART1_Config(uint32_t baud);
void BUF_Clear(void);
uint8_t USART1_SendData(const char *__format, ...);

#endif
