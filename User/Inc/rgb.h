#ifndef __RGB_H
#define __RGB_H
#include "main.h"  


#define   RGB_DATA_H     GPIO_SetBits(GPIOB,GPIO_Pin_9)
#define   RGB_DATA_L    GPIO_ResetBits(GPIOB,GPIO_Pin_9)

#define   RGB_CLK_H   GPIO_SetBits(GPIOB,GPIO_Pin_8)
#define   RGB_CLK_L   GPIO_ResetBits(GPIOB,GPIO_Pin_8)

void rgb_init(void);
void rgb_send_data(u8 data);
void rgb_show(u8 red, u8 green, u8 blue);




#endif

