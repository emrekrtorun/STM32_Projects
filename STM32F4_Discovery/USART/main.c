// Kütüphaneler
#include "stm32f4xx.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

void GPIO_Yapilandir(void);
void USART_Yapilandir(void);
void USART_Mesaj_Gonder(volatile char *mesaj);


int main(void)
{
  GPIO_Yapilandir();
  USART_Yapilandir();
  
  
  int state=1;
  while(1)
  {

    int a=GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15);
    if (a==1 && state==0)
    {
      state=1;
      GPIO_SetBits(GPIOD, GPIO_Pin_14);
      USART_Mesaj_Gonder("1\n");
    } 
    else if(a==0 && state==1)
    {
      state=0;
      GPIO_ResetBits(GPIOD, GPIO_Pin_14);
      USART_Mesaj_Gonder("0\n");
    }
  }
   
}

void GPIO_Yapilandir(void)
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC |RCC_AHB1Periph_GPIOD,ENABLE);
  
  GPIO_InitTypeDef Led;
  GPIO_InitTypeDef Sensor;
  
  Led.GPIO_Pin = GPIO_Pin_14; 
  Led.GPIO_Mode = GPIO_Mode_OUT;
  Led.GPIO_OType = GPIO_OType_PP;
  Led.GPIO_PuPd = GPIO_PuPd_NOPULL;
  Led.GPIO_Speed = GPIO_Speed_50MHz;
   
  GPIO_Init(GPIOD,&Led);
  
  Sensor.GPIO_Pin = GPIO_Pin_15;
  Sensor.GPIO_Mode = GPIO_Mode_IN;
  Sensor.GPIO_OType = GPIO_OType_PP;
  Sensor.GPIO_PuPd = GPIO_PuPd_DOWN;
  Sensor.GPIO_Speed = GPIO_Speed_50MHz;
  
  GPIO_Init(GPIOC,&Sensor);  
}


void USART_Yapilandir(void)
{
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);  //USART 3 B10 VE B11 PINLERI KULLANILACAK
 
 	GPIO_InitTypeDef GPIO_USART_Yapilandir;
	GPIO_USART_Yapilandir.GPIO_Pin = GPIO_Pin_10;  //USART Tx
	GPIO_USART_Yapilandir.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_USART_Yapilandir.GPIO_Mode = GPIO_Mode_AF;
        GPIO_USART_Yapilandir.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB,&GPIO_USART_Yapilandir);
        
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3); //PB10 dan gönderecegiz

	USART_InitTypeDef USART3_Yapilandir;
	USART3_Yapilandir.USART_BaudRate = 9600;
	USART3_Yapilandir.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART3_Yapilandir.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART3_Yapilandir.USART_Parity = USART_Parity_No;
	USART3_Yapilandir.USART_StopBits = USART_StopBits_1;
	USART3_Yapilandir.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART3,&USART3_Yapilandir);
	
	USART_Cmd(USART3,ENABLE); 
}


void USART_Mesaj_Gonder(volatile char *mesaj)
{
  while(*mesaj)
  {
    USART_SendData(USART3,*mesaj++);
    while(USART_GetFlagStatus(USART3,USART_FLAG_TC)== RESET)
   {}
  }	
}

