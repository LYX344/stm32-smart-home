#include "main.h"        

void relay_init(void)
{
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //开启GPIO端口时钟
GPIO_InitTypeDef GPIO_InitStruct = {0};                                      //定义结构体变量
GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;                               //设置为推挽输出
GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_1;
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init (GPIOB, &GPIO_InitStruct);

RELAY_OFF();
	}
