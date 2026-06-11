#ifndef __RELAY_H
#define __RELAY_H
#include "main.h"                  // Device header

#define   RELAY_ON()   GPIO_SetBits (GPIOB,GPIO_Pin_1)
#define   RELAY_OFF()   GPIO_ResetBits (GPIOB,GPIO_Pin_1)
#define   RELAY_TURN()   GPIOB->ODR ^= (1<<1)
		
		
void relay_init(void);
		
		
#endif
