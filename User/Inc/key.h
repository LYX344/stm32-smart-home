#ifndef __KEY_H
#define __KEY_H
#include "main.h"                  // Device header

#define   KEY1_PRESS   GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)
#define   KEY2_PRESS   !GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)
#define   KEY3_PRESS   !GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10)


void key_init(void);
u8   key_scanf(void);

#endif

