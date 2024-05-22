#include "stm32f10x.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "key.h"
#include "beep.h"
#include "rtc.h"
#include "seg.h"

int main(void) {
  uint8_t beep_state = 0;
  uint8_t key = 0;
  SysTick_Init(72);
  USART1_Config(115200);
  RTC_Config();
  SEG_Init();
  KEY_Init();
  EXTI_Config();
  LED_Init();
  BEEP_Init();
  while (1) {
    if (KEY_STATE == 1) {
      KEY_CLEAR;
      key = ~key;
    } else if (KEY_STATE == 4) {
      KEY_CLEAR;
      USART1_SendData("TIME SET\r\n");
      SEG_Clear();
      LED0_ON;
      TIME_Set_Task();
      LED_OFF;
    }
    if (beep_state == 0 && DATE.minute == 0 && DATE.second == 0) {
      BEEP_ON;
      beep_state = 1;
    } else if (beep_state == 1) {
      BEEP_OFF;
      beep_state = 0;
    }
    if (key) {
      SEG_Display_Format("%d-%d-%d", DATE.year % 2000, DATE.month, DATE.day);
    } else {
      SEG_Display_Format("%d-%d-%d", DATE.hour, DATE.minute, DATE.second);
    }
  }
}
