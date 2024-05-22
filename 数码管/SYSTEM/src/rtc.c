#include "rtc.h"

_calendar_obj calendar;
const uint8_t mon_table[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

uint8_t Is_Leap_Year(uint16_t year) {
  if (year % 4 == 0 && year % 100 != 0 || year % 400 == 0) {
    return 1; // 是闰年
  } else {
    return 0; // 不是闰年
  }
}

void RTC_NVIC_Config(void) {
  NVIC_InitTypeDef NVIC_InitStruct;
  NVIC_InitStruct.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);
}

uint8_t RTC_Config(void) {
  uint8_t temp = 0; //查看是不是第一次配置
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP,
                         ENABLE); //使能PWR和BKP时钟
  PWR_BackupAccessCmd(ENABLE);    //使能RTC和BKP时钟的访问
  if (BKP_ReadBackupRegister(BKP_DR1) !=
      0xA8) {     //从指定位置的后备寄存器读出数据，若不是0xA5，则没有配置过
    BKP_DeInit(); //复位备份区域
    RCC_LSEConfig(RCC_LSE_ON);
    while ((RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) && temp++ < 250) {
      delay_ms(10);
    }
    if (temp >= 250)
      return 1;                             //初始化失败,晶振有问题
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE); // 选择LSE作为RTC时钟源
    RCC_RTCCLKCmd(ENABLE);                  //使能RTC时钟
    RTC_WaitForLastTask();            //等待RTC寄存器上一次写操作
    RTC_WaitForSynchro();             //等待RTC寄存器同步
    RTC_ITConfig(RTC_IT_SEC, ENABLE); //使能秒中断
    RTC_WaitForLastTask();            //等待RTC寄存器上一次写操作
    RTC_EnterConfigMode();            //进入配置模式
    RTC_SetPrescaler(32767);          //设置RTC预分频值
    RTC_WaitForLastTask();            //等待RTC寄存器上一次写操作
    RTC_Set(2024, 5, 17, 1, 57, 0);
    RTC_ExitConfigMode();                   //退出配置模式
    BKP_WriteBackupRegister(BKP_DR1, 0xA8); //将0xA5写入指定位置的后备寄存器
    USART1_SendData("RTC Init for the first time.\r\n"); //发送成功信息
  } else {
    RTC_WaitForSynchro();             //等待RTC寄存器同步
    RTC_ITConfig(RTC_IT_SEC, ENABLE); //使能秒中断
    RTC_WaitForLastTask();            //等待RTC寄存器上一次写操作
    USART1_SendData("RTC Init.\r\n");
  }
  RTC_NVIC_Config();
  RTC_Get();
  return 0;
}

uint8_t RTC_Get(void) {
  static uint16_t daycnt = 0;
  uint32_t timecount = 0, temp = 0;
  uint16_t temp1 = 0;
  timecount = RTC_GetCounter();
  temp = timecount / 86400;
  if (daycnt != temp) {
    daycnt = temp;
    temp1 = 1970; // 1970年1月1日为星期天
    while (temp > 365) {
      if (Is_Leap_Year(temp1)) { //如果是闰年
        if (temp >= 366) {
          temp -= 366;
        } else {
          temp1++;
          break;
        }
      } else { //如果是平年
        temp -= 365;
      }
      temp1++;
    }
    calendar.year = temp1;
    temp1 = 0;
    while (temp >= 28) {
      if (Is_Leap_Year(calendar.year) && temp1 == 1) { //当年是闰年还是2月份
        if (temp >= 29) {
          temp -= 29;
        } else {
          break;
        }
      } else {
        if (temp >= mon_table[temp1]) {
          temp -= mon_table[temp1];
        } else {
          break;
        }
      }
      temp1++;
    }
    calendar.month = temp1 + 1;
    calendar.day = temp + 1;
  }
  temp = timecount % 86400;
  calendar.hour = temp / 3600;
  calendar.minute = temp % 3600 / 60;
  calendar.second = temp % 60;
  // 获取星期
  return 0;
}

uint8_t RTC_Set(uint16_t year, uint8_t month, uint8_t day, uint8_t hour,
                uint8_t minute, uint8_t second) {
  uint32_t secondcnt = 0;
  if (year < 1977 || year > 2099) {
    return 1;
  }
  for (uint16_t t = 1970; t < year; t++) {
    if (Is_Leap_Year(t)) { //是闰年
      secondcnt += 31622400;
    } else {
      secondcnt += 31536000;
    }
  }
  month -= 1;
  for (uint8_t t = 0; t < month; t++) {
    secondcnt += (uint32_t)(mon_table[t]) * 86400;
    if (Is_Leap_Year(year) && t == 1) { //是闰年
      secondcnt += 86400;
    }
  }
  secondcnt += (uint32_t)(day - 1) * 86400;
  secondcnt += (uint32_t)hour * 3600;
  secondcnt += (uint32_t)minute * 60;
  secondcnt += (uint32_t)second;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP,
                         ENABLE); //使能PWR和BKP时钟
  PWR_BackupAccessCmd(ENABLE);
  RTC_SetCounter(secondcnt);
  RTC_WaitForLastTask();
  return 0;
}

void RTC_IRQHandler(void) {
  if (RTC_GetITStatus(RTC_IT_SEC) != RESET) { //秒能中断
    RTC_Get();
  }
  if (RTC_GetITStatus(RTC_IT_ALR) != RESET) { //闹钟中断

    RTC_ClearITPendingBit(RTC_IT_ALR);
    // 更新时间
    USART1_SendData("RTC Alarm.\r\n");
  }
  RTC_ClearITPendingBit(RTC_IT_SEC | RTC_IT_OW);
  RTC_WaitForLastTask();
}
