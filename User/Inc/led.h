#ifndef __LED_H
#define __LED_H
#include "main.h"                  // Device header

#define   LED1_ON()     GPIO_SetBits(GPIOB,GPIO_Pin_11)          //LED1置高电平
#define   LED1_OFF()    GPIO_ResetBits(GPIOB,GPIO_Pin_11)        //LED1置低电平
#define   LED1_TURN()   GPIOB->ODR  ^= (1<<11)                  //LED1翻转


#define   LED2_ON()     GPIO_SetBits(GPIOB,GPIO_Pin_14)
#define   LED2_OFF()    GPIO_ResetBits(GPIOB,GPIO_Pin_14)
#define   LED2_TURN()   GPIOB->ODR  ^= (1<<14)


#define   LED3_ON()     GPIO_SetBits(GPIOA,GPIO_Pin_11)
#define   LED3_OFF()    GPIO_ResetBits(GPIOA,GPIO_Pin_11)
#define   LED3_TURN()   GPIOA->ODR  ^= (1<<11)

void led_init(void);


#endif

