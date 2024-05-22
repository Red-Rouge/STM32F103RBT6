#include "delay.h"
#include <misc.h>

static uint8_t fac_us = 0;  // us延时倍乘数
static uint16_t fac_ms = 0; // ms延时倍乘数
unsigned int TimingDelay = 0;

//初始化延迟函数
// SYSTICK的时钟固定为AHB时钟的1/8
// SYSCLK:系统时钟频率
void SysTick_Init(uint8_t SYSCLK) {
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
  fac_us = SYSCLK / 8;
  fac_ms = (uint16_t)fac_us * 1000;
}

/*******************************************************************************
* 函 数 名         : delay_us
* 函数功能		   : us延时，
* 输    入         : nus：要延时的us数
                    注意:nus的值,不要大于508400us(最大值即2^24/fac_us@fac_us=21)
* 输    出         : 无
*******************************************************************************/
void delay_us(uint32_t nus) {
  uint32_t total_ticks = nus * fac_us;
  while (total_ticks > 0) {
    uint32_t ticks = (total_ticks > 0xFFFFFF) ? 0xFFFFFF : total_ticks;
    SysTick->LOAD = ticks - 1;
    SysTick->VAL = 0x00;
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;
    while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)) {
    }
    SysTick->CTRL = 0x00;
    SysTick->VAL = 0x00;

    total_ticks -= ticks;
  }
}

/*******************************************************************************
* 函 数 名         : delay_ms
* 函数功能		   : ms延时，
* 输    入         : nms：要延时的ms数
                    注意:nms的值,SysTick->LOAD为24位寄存器，
                    不要大于0xffffff*8*1000/SYSCLK
                    对168M条件下,nms<=798ms
* 修    改 ：增加了循环，允许输入大数据,但是为了不占用资源，不建议输入太大
* 输    出         : 无
*******************************************************************************/
void delay_ms(uint32_t nms) {
  uint32_t total_ticks = nms * fac_ms;
  while (total_ticks > 0) {
    uint32_t ticks = (total_ticks > 0xFFFFFF) ? 0xFFFFFF : total_ticks;
    SysTick->LOAD = ticks - 1;
    SysTick->VAL = 0x00;
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;
    while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)) {
    }
    SysTick->CTRL = 0x00;
    SysTick->VAL = 0x00;
    total_ticks -= ticks;
  }
}

void TimingDelay_Decrement(void) {
  if (TimingDelay != 0x00) {
    TimingDelay--;
  }
}
