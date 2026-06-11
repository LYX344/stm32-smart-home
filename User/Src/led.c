#include "main.h"     

void led_init(void)
{
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA| RCC_APB2Periph_GPIOB, ENABLE);  //开启GPIO端口时钟
GPIO_InitTypeDef GPIO_InitStruct = {0};                                      //定义结构体变量
GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;                               //设置为推挽输出
GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_11;
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init (GPIOA, &GPIO_InitStruct);

GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_11|GPIO_Pin_14;
GPIO_Init(GPIOB,&GPIO_InitStruct);

LED1_OFF();
LED2_OFF();
LED3_OFF();

}
//void GPIO_SetBits(GPIO_TypeDef* GPIOx, unit16_t GPIO_Pin);
//void GPIO_ResetBits(GPIO_TypeDef* GPIOx, unit16_t GPIO_Pin);
