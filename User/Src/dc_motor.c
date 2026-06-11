//TIM1  wifi有关
//TIM2  红外
//TIM3  电机
//TIM4  NVIC
#include "main.h"

void dc_motor_init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);
	
	GPIO_InitTypeDef GPIO_Struct = {0};  //定义GPIO结构体变量
	
	GPIO_Struct.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_Struct.GPIO_Pin   = GPIO_Pin_4|GPIO_Pin_5;
	GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Struct);
	
	TIM_TimeBaseInitTypeDef TIM_Struct = {0};
	
	TIM_Struct.TIM_ClockDivision     = TIM_CKD_DIV1;        //时钟分频  一般不使用
	TIM_Struct.TIM_CounterMode       = TIM_CounterMode_Up;
	TIM_Struct.TIM_Period            = 1000;                //ARR
	TIM_Struct.TIM_Prescaler         = 72-1;                //PSC
	TIM_Struct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3,&TIM_Struct);
	
	TIM_OCInitTypeDef TIM_OC_Struct = {0};
	TIM_OC_Struct.TIM_OCIdleState  = 0;
	TIM_OC_Struct.TIM_OCMode       = TIM_OCMode_PWM1;
	TIM_OC_Struct.TIM_OCNIdleState = 0;
	TIM_OC_Struct.TIM_OCNPolarity  = 0;
	TIM_OC_Struct.TIM_OCPolarity   = TIM_OCPolarity_High;
	TIM_OC_Struct.TIM_OutputNState = 0;
	TIM_OC_Struct.TIM_OutputState  = TIM_OutputState_Enable;
	TIM_OC_Struct.TIM_Pulse        = 0;
	TIM_OC1Init(TIM3,&TIM_OC_Struct);
	TIM_OC2Init(TIM3,&TIM_OC_Struct);
	
	TIM_ARRPreloadConfig(TIM3,ENABLE);
	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);
	
	TIM_Cmd(TIM3,ENABLE);
}

void dc_motor_left(u16 arr)
{
	TIM_SetCompare1(TIM3,arr);
	TIM_SetCompare2(TIM3,0);
}

void dc_motor_right(u16 arr)
{
	TIM_SetCompare2(TIM3,arr);
	TIM_SetCompare1(TIM3,0);
}
