#ifndef _RTC_H
#define _RTC_H

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_rtc.h"
#include "stm32f10x_pwr.h"
#include "misc.h"
#include "usart.h"
#include "delay.h"

typedef struct {
  uint8_t hour;
  uint8_t minute;
  uint8_t second;

  uint16_t year;
  uint8_t month;
  uint8_t day;
} _calendar_obj;

#define DATE calendar

extern _calendar_obj calendar;
extern uint8_t const mon_table[12];

uint8_t RTC_Config(void);
void RTC_NVIC_Config(void);
uint8_t Is_Leap_Year(uint16_t year);

uint8_t RTC_Get(void);
uint8_t RTC_Set(uint16_t year, uint8_t month, uint8_t day, uint8_t hour,
                uint8_t minute, uint8_t second);

#endif
