#include "slip.h"

void SLIP_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct;
  ADC_InitTypeDef ADC_InitStruct;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_ADC1, ENABLE);

  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
  GPIO_Init(GPIOB, &GPIO_InitStruct);

  RCC_ADCCLKConfig(RCC_PCLK2_Div6);
  ADC_DeInit(ADC1);

  ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStruct.ADC_NbrOfChannel = 1;
  ADC_InitStruct.ADC_ScanConvMode = DISABLE;

  ADC_Init(ADC1, &ADC_InitStruct);
  ADC_Cmd(ADC1, ENABLE);

  ADC_ResetCalibration(ADC1);
  while (ADC_GetResetCalibrationStatus(ADC1)) {
  }
  ADC_StartCalibration(ADC1);
  while (ADC_GetCalibrationStatus(ADC1)) {
  }
}

uint16_t SLIP_Get_Value(void) {
  ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_239Cycles5);
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
  while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)) {
  }
  return ADC_GetConversionValue(ADC1);
}

uint32_t SLIP_Get(uint16_t times) {
  uint32_t sum = 0;
  for (int i = 0; i < times; i++) {
    sum += SLIP_Get_Value();
  }
  return sum / times;
}
