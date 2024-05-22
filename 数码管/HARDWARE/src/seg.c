#include "seg.h"

void HC138_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_InitStruct.GPIO_Pin = HC138_A0 | HC138_A1 | HC138_A2;
  GPIO_Init(HC138_PORT, &GPIO_InitStruct);

  Pout(HC138_PORT, HC138_A0, Bit_RESET);
  Pout(HC138_PORT, HC138_A1, Bit_RESET);
  Pout(HC138_PORT, HC138_A2, Bit_RESET);
}

void HC138_SelectOutput(uint8_t output) {
  if (output <= 7) {
    uint8_t var1 = (output >> 2) & 0x1;
    uint8_t var2 = (output >> 1) & 0x1;
    uint8_t var3 = output & 0x1;
    Pout(HC138_PORT, HC138_A0, (BitAction)var3);
    Pout(HC138_PORT, HC138_A1, (BitAction)var2);
    Pout(HC138_PORT, HC138_A2, (BitAction)var1);
  }
}

void HC595_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); // PB4部分重映射

  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_InitStruct.GPIO_Pin = HC595_SCLK | HC595_LCLK | HC595_DATA;
  GPIO_Init(HC595_PORT, &GPIO_InitStruct);

  Pout(HC595_PORT, HC595_DATA, Bit_RESET);
  Pout(HC595_PORT, HC595_LCLK, Bit_SET);
  Pout(HC595_PORT, HC595_SCLK, Bit_RESET);
}

void HC595_Send_Byte(uint8_t byte) {
  // 发送数据位
  for (int i = 0; i < 8; i++) {
    // 设置数据引脚（DIO）的状态
    if ((byte << i) & 0x80) {
      Pout(HC595_PORT, HC595_DATA, Bit_SET);
    } else {
      Pout(HC595_PORT, HC595_DATA, Bit_RESET);
    }

    // 上升沿时钟信号（SCLK）
    Pout(HC595_PORT, HC595_SCLK, Bit_RESET);
    delay_us(1);
    Pout(HC595_PORT, HC595_SCLK, Bit_SET);
    delay_us(1);
  }
  // 上升沿锁存信号（LCLK）
  Pout(HC595_PORT, HC595_LCLK, Bit_RESET);
  delay_us(1);
  Pout(HC595_PORT, HC595_LCLK, Bit_SET);
  delay_us(1);
}

void SEG_Init(void) {
  HC138_Init();
  HC595_Init();
  SEG_Display(0, 0x00);
}

void SEG_Display(uint8_t pos, uint8_t date) {
  HC138_SelectOutput(pos);
  HC595_Send_Byte(date);
}

void SEG_Display_Num(uint8_t pos, uint8_t num, uint8_t dot) {
  uint8_t segduma[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66,
                       0x6d, 0x7d, 0x07, 0x7f, 0x6f};
  if (num > 9) {
    return;
  }
  SEG_Display(pos, segduma[num] | (dot << 7));
}

void SEG_Display_Underline(uint8_t pos) { SEG_Display(pos, 0x08); }

void SEG_Display_Hozline(uint8_t pos) { SEG_Display(pos, 0x40); }

void SEG_Display_Format(const char *__format, ...) {
  va_list args;
  va_start(args, __format);

  // 格式化输出到缓冲区
  char buffer[256]; // 假设缓冲区大小为256
  vsnprintf(buffer, sizeof(buffer), __format, args);

  // 清空可变参数列表
  va_end(args);
  USART1_SendData("%s\r\n", buffer);

  // 循环buffer
  for (int i = 0; i < sizeof(buffer) && i < 8; i++) {
    //  如果是‘-’
    if (buffer[i] == '-') {
      SEG_Display_Hozline(i);
    }
    //  如果是‘_’
    else if (buffer[i] == '_') {
      SEG_Display_Underline(i);
    }
    //如果是数字
    else if (buffer[i] >= '0' && buffer[i] <= '9') {
      SEG_Display_Num(i, buffer[i] - '0', 0);
    }
    delay_ms(2);
  }
}

void SEG_Clear() {
  for (int i = 0; i < 8; i++) {
    SEG_Display(i, 0x00);
  }
}

void TIME_Set_Task(void) {
  uint16_t year = DATE.year % 2000;
  uint8_t month = DATE.month;
  uint8_t day = DATE.day;
  uint8_t hour = DATE.hour;
  uint8_t minute = DATE.minute;
  uint8_t second = DATE.second;

  int8_t pos_state = 0;

  while (1) {

    if (KEY_STATE == 1) {
      KEY_CLEAR;
      pos_state++;
      if (pos_state > 5) {
        pos_state = 0;
      }
    } else if (KEY_STATE == 2) {
      KEY_CLEAR;
      if (pos_state == 0) {
        year++;
        if (year > 99) {
          year = 0;
        }
      } else if (pos_state == 1) {
        month++;
        if (month > 12) {
          month = 1;
        }
      } else if (pos_state == 2) {
        day++;
        if (day > mon_table[month - 1]) {
          day = 1;
        }
      } else if (pos_state == 3) {
        hour++;
        if (hour > 23) {
          hour = 0;
        }
      } else if (pos_state == 4) {
        minute++;
        if (minute > 59) {
          minute = 0;
        }
      } else if (pos_state == 5) {
        second++;
        if (second > 59) {
          second = 0;
        }
      }
    } else if (KEY_STATE == 3) {
      KEY_CLEAR;
      if (pos_state == 0) {
        if (year == 0) {
          year = 99;
        } else {
          year--;
        }
      } else if (pos_state == 1) {
        if (month == 1) {
          month = 12;
        } else {
          month--;
        }
      } else if (pos_state == 2) {
        if (day == 1) {
          day = mon_table[month - 1];
        } else {
          day--;
        }
      } else if (pos_state == 3) {
        if (hour == 0) {
          hour = 23;
        } else {
          hour--;
        }
      } else if (pos_state == 4) {
        if (minute == 0) {
          minute = 59;
        } else {
          minute--;
        }
      } else if (pos_state == 5) {
        if (second == 0) {
          second = 59;
        } else {
          second--;
        }
      }
    } else if (KEY_STATE == 4) {
      KEY_CLEAR;
      RTC_Set(year + 2000, month, day, hour, minute, second);
      RTC_Get();
      return;
    }
    if (pos_state == 0) {
      LED_OFF;
      LED0_ON;
      LED7_ON;
    } else if (pos_state == 1) {
      LED_OFF;
      LED0_ON;
      LED6_ON;
    } else if (pos_state == 2) {
      LED_OFF;
      LED0_ON;
      LED5_ON;
    } else if (pos_state == 3) {
      LED_OFF;
      LED0_ON;
      LED4_ON;
    } else if (pos_state == 4) {
      LED_OFF;
      LED0_ON;
      LED3_ON;
    } else if (pos_state == 5) {
      LED_OFF;
      LED0_ON;
      LED2_ON;
    }
    if (pos_state < 3) {
      if (day >= mon_table[month - 1]) {
        day = mon_table[month - 1];
      }
      SEG_Display_Format("%d-%d-%d", year, month, day);
    } else {
      SEG_Display_Format("%d-%d-%d", hour, minute, second);
    }
  }
}
