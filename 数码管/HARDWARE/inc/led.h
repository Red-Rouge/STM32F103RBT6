#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#define LED0_ON GPIO_ResetBits(GPIOC, GPIO_Pin_0);
#define LED0_OFF GPIO_SetBits(GPIOC, GPIO_Pin_0);
#define LED0_REV                                                               \
  GPIO_WriteBit(GPIOC, GPIO_Pin_0,                                             \
                (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_0)));

#define LED1_ON GPIO_ResetBits(GPIOC, GPIO_Pin_1);
#define LED1_OFF GPIO_SetBits(GPIOC, GPIO_Pin_1);
#define LED1_REV                                                               \
  GPIO_WriteBit(GPIOC, GPIO_Pin_1,                                             \
                (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_1)));

#define LED2_ON GPIO_ResetBits(GPIOC, GPIO_Pin_2);
#define LED2_OFF GPIO_SetBits(GPIOC, GPIO_Pin_2);
#define LED2_REV                                                               \
  GPIO_WriteBit(GPIOC, GPIO_Pin_2,                                             \
                (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_2)));

#define LED3_ON GPIO_ResetBits(GPIOC, GPIO_Pin_3);
#define LED3_OFF GPIO_SetBits(GPIOC, GPIO_Pin_3);
#define LED3_REV                                                               \
  GPIO_WriteBit(GPIOC, GPIO_Pin_3,                                             \
                (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_3)));

#define LED4_ON GPIO_ResetBits(GPIOC, GPIO_Pin_4);
#define LED4_OFF GPIO_SetBits(GPIOC, GPIO_Pin_4);
#define LED4_REV                                                               \
  GPIO_WriteBit(GPIOC, GPIO_Pin_4,                                             \
                (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_4)));

#define LED5_ON GPIO_ResetBits(GPIOC, GPIO_Pin_5);
#define LED5_OFF GPIO_SetBits(GPIOC, GPIO_Pin_5);
#define LED5_REV                                                               \
  GPIO_WriteBit(GPIOC, GPIO_Pin_5,                                             \
                (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_5)));

#define LED6_ON GPIO_ResetBits(GPIOC, GPIO_Pin_6);
#define LED6_OFF GPIO_SetBits(GPIOC, GPIO_Pin_6);
#define LED6_REV                                                               \
  GPIO_WriteBit(GPIOC, GPIO_Pin_6,                                             \
                (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_1)));

#define LED7_ON GPIO_ResetBits(GPIOC, GPIO_Pin_7);
#define LED7_OFF GPIO_SetBits(GPIOC, GPIO_Pin_7);
#define LED7_REV                                                               \
  GPIO_WriteBit(GPIOC, GPIO_Pin_7,                                             \
                (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_7)));

#define LED_ON                                                                 \
  LED0_ON;                                                                     \
  LED1_ON;                                                                     \
  LED2_ON;                                                                     \
  LED3_ON;                                                                     \
  LED4_ON;                                                                     \
  LED5_ON;                                                                     \
  LED6_ON;                                                                     \
  LED7_ON;
#define LED_OFF                                                                \
  LED0_OFF;                                                                    \
  LED1_OFF;                                                                    \
  LED2_OFF;                                                                    \
  LED3_OFF;                                                                    \
  LED4_OFF;                                                                    \
  LED5_OFF;                                                                    \
  LED6_OFF;                                                                    \
  LED7_OFF;
#define LED_REV                                                                \
  LED0_REV;                                                                    \
  LED1_REV;                                                                    \
  LED2_REV;                                                                    \
  LED3_REV;                                                                    \
  LED4_REV;                                                                    \
  LED5_REV;                                                                    \
  LED6_REV;                                                                    \
  LED7_REV;

void LED_Init(void);

#endif
