#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_adc.h"


uint32_t ADC1_Value = 0;
uint32_t ADC1_Voltage = 0;


void EXTI0_IRQHandler(void);
void EXTI0_Init(void);
void ADC1_Init(void);
void DMA2_Init(void);


int main(void)
{
  EXTI0_Init();
  DMA2_Init();
  ADC1_Init();
 
  ADC_SoftwareStartConv(ADC1);

    while (1)
    { 
      ADC1_Voltage = ADC1_Value *3000/0xFFF;
    }
}

void EXTI0_IRQHandler(void)
{
   if(EXTI_GetITStatus(EXTI_Line0)!= RESET)
   {  
     if(ADC1_Voltage>1500)
    {
      GPIO_SetBits(GPIOD,GPIO_Pin_14);
    }
  
    else
    {
    GPIO_ResetBits(GPIOD,GPIO_Pin_14);
    }
   }
   EXTI_ClearITPendingBit(EXTI_Line0);
}

void EXTI0_Init(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
   EXTI_InitTypeDef EXTI_InitStructure;
   NVIC_InitTypeDef NVIC_InitStructure;

   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOA ,ENABLE);			
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);		

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_Init(GPIOD, &GPIO_InitStructure);

        
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
   GPIO_Init(GPIOA,&GPIO_InitStructure);
	
   SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);

    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

	
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
  
}

void ADC1_Init(void)
{
  ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  GPIO_InitTypeDef      GPIO_InitStructure;

  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);


  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  
  ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_3Cycles);

  ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);


  ADC_DMACmd(ADC1, ENABLE);


  ADC_Cmd(ADC1, ENABLE);
}

void DMA2_Init(void)
{
   DMA_InitTypeDef  DMA_InitStructure;
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
      
  DMA_InitStructure.DMA_Channel = DMA_Channel_0;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = ((uint32_t)&ADC1->DR);
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC1_Value;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = 1;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
 // DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
 // DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream4, &DMA_InitStructure);
  DMA_Cmd(DMA2_Stream4, ENABLE);
  
}
