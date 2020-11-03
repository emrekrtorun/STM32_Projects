#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "misc.h"


void Configure_GPIO(void);
void Configure_PWM1(void);
void Configure_PWM2(void);
void Configure_USART(void);

int main(void)
{
	Configure_GPIO();
	Configure_PWM1();
	Configure_PWM2();
	Configure_USART();

	while (1)
	{
	}

}

void Configure_GPIO(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef Configure_GPIO;
	Configure_GPIO = { GPIO_Pin_2 | GPIO_Pin_9,GPIO_Speed_50MHz,GPIO_Mode_Out_PP };
	GPIO_Init(GPIOA, &Configure_GPIO);

	Configure_GPIO = { GPIO_Pin_6 | GPIO_Pin_15   ,GPIO_Speed_50MHz,GPIO_Mode_Out_PP };
	GPIO_Init(GPIOB, &Configure_GPIO);
}

void Configure_PWM1(void);
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	// Create 1kHz PWM
	// TIM2 is connected to APB1 bus that have default clock 72MHz
	// So, the frequency of TIM2 is 72MHz
	// I use prescaler 10 here
	// So, the frequency of TIM2 now is 7.2MHz
	TIM_TimeBaseInitStruct.TIM_Prescaler = 10;
	// TIM_Period determine the PWM frequency by this equation: PWM_frequency = timer_clock / (TIM_Period + 1)
	// If I want 1kHz PWM I can calculate: TIM_Period = (timer_clock / PWM_frequency) - 1	// 
	// TIM_Period = (7.2MHz / 1kHz) - 1 = 7199
	TIM_TimeBaseInitStruct.TIM_Period = 7199;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);


	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	// TIM_Pulse = (((TIM_Period + 1) * duty_cycle) / 100) - 1
	TIM_OCInitStruct.TIM_Pulse = 0;
	TIM_OC1Init(TIM2, &TIM_OCInitStruct);
	//TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC4Init(TIM2, &TIM_OCInitStruct);

	TIM_Cmd(TIM2, ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

}

void Configure_PWM2(void);
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	// Configurations are the same as previous one
	TIM_TimeBaseInitStruct.TIM_Prescaler = 10;
	TIM_TimeBaseInitStruct.TIM_Period = 7199;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);

	TIM_Cmd(TIM1, ENABLE); 	// Start TIM2

	//  Initialize PWM
	// PWM settings
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_Pulse = 0;
	TIM_OC2Init(TIM1, &TIM_OCInitStruct);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 	// Initialize GPIOA (PA9)

	// Initialize PA0 as push-pull alternate function (PWM output)
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

}


void Configure_USART(void);
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //USART 3 USES B10 AND B11


	NVIC_InitTypeDef NVIC_USART_Kesme_Yapilandir;
	NVIC_USART_Kesme_Yapilandir.NVIC_IRQChannel = USART3_IRQn;
	NVIC_USART_Kesme_Yapilandir.NVIC_IRQChannelCmd = ENABLE;
	NVIC_USART_Kesme_Yapilandir.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_USART_Kesme_Yapilandir.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_USART_Kesme_Yapilandir);

	GPIO_InitTypeDef GPIO_Configure_USART;
	GPIO_Configure_USART.GPIO_Pin = GPIO_Pin_10;  //USART Tx
	GPIO_Configure_USART.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Configure_USART.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_Configure_USART);

	GPIO_Configure_USART.GPIO_Pin = GPIO_Pin_11; //USART3 Rx
	GPIO_Configure_USART.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Configure_USART.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_Configure_USART);

	USART_InitTypeDef USART3_Yapilandir;
	USART3_Yapilandir.USART_BaudRate = 115200;
	USART3_Yapilandir.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART3_Yapilandir.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART3_Yapilandir.USART_Parity = USART_Parity_No;
	USART3_Yapilandir.USART_StopBits = USART_StopBits_1;
	USART3_Yapilandir.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART3, &USART3_Yapilandir);

	USART_Cmd(USART3, ENABLE);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
}

void USART3_IRQHandler(void)
{
	if (USART_GetITStatus(USART3, USART_IT_RXNE))
	{

		switch ((char)USART_ReceiveData(USART3))
		{
		case '0': 		TIM1->CCR2 = 0;
						break;

		case '1': 		TIM1->CCR2 = 1799;
						break;

		case '2': 		TIM1->CCR2 = 3599;
						break;

		case '3': 		TIM1->CCR2 = 5399;
						break;

		case '4': 		TIM1->CCR2 = 7199;
						break;

		case '5':		TIM2->CCR4 = 0;
						break;

		case '6': 	    TIM2->CCR4 = 1799;
						break;

		case '7': 		TIM2->CCR4 = 3599;
						break;

		case '8': 		TIM2->CCR4 = 5399;
						break;

		case '9': 		TIM2->CCR4 = 7199;
			break;

		case 'a':		TIM2->CCR1 = 0;
						GPIO_SetBits(GPIOB, GPIO_Pin_15);
						GPIO_ResetBits(GPIOA, GPIO_Pin_2);
						break;

		case 'b':		TIM2->CCR1 = 0;
						GPIO_SetBits(GPIOA, GPIO_Pin_2);
						GPIO_ResetBits(GPIOB, GPIO_Pin_15);
						break;

		case 'c':		TIM2->CCR4 = 0;
						GPIO_SetBits(GPIOB, GPIO_Pin_6);
						GPIO_ResetBits(GPIOA, GPIO_Pin_12);
						break;

		case 'd':		TIM2->CCR4 = 0;
						GPIO_SetBits(GPIOA, GPIO_Pin_12);
						GPIO_ResetBits(GPIOB, GPIO_Pin_6);
						break;
						
		}
	}
}
