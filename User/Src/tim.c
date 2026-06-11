#include "main.h"


void delay_tim4_it_ms(u32 ms)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);	

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision			= TIM_CKD_DIV1;	//这个也是分频    最大只能4分频     这个分频一般不用
	TIM_TimeBaseInitStruct.TIM_CounterMode				= TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period					= 10*ms-1;      //(ARR)计一个数0.1ms,延时1ms  计数最大值就是10，延时2ms   计数最大值就是20
	TIM_TimeBaseInitStruct.TIM_Prescaler				= 7200-1;			//PSC
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter	    = 0;			//This parameter is valid only for TIM1 and TIM8. 	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);
	
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel						=	TIM4_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd					=	ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority   =   10;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority			=	0;
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_Cmd(TIM4,ENABLE);	
}
u32 tim2_cnt[5] = {0};
u32 fan_time_cnt = 0;
u32 fan_pic_cnt = 0;
//LED1  100ms闪烁一次     LED2    200ms闪烁一次    LED3    300ms闪烁一次
//按键
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update)) 
	{
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
		fan_time_cnt++;		
	}
}

