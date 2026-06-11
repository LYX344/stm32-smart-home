#ifndef __DHT11_H
#define __DHT11_H
#include "main.h"                  // Device header

#define   DHT11_DATA_H     GPIO_SetBits(GPIOB,GPIO_Pin_3)          //LED1÷√∏ﬂµÁ∆Ω
#define   DHT11_DATA_L     GPIO_ResetBits(GPIOB,GPIO_Pin_3) 
#define   DHT11_DATA_IN    GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)

void dht11_init(void);
u8 dht11_read(void);

extern u8 temperature;
extern u8 humidity;




#endif

