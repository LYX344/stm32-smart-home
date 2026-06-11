#include "main.h"

void key_init(void)
{

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);     
    GPIO_InitTypeDef GPIO_InitStructure = {0};    
    GPIO_InitStructure.GPIO_Mode        = GPIO_Mode_IN_FLOATING; 
    GPIO_InitStructure.GPIO_Pin         = GPIO_Pin_0 | GPIO_Pin_8;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin         = GPIO_Pin_10;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

u8 key_scanf(void)
{
    u32 key_time = 0;
    u8 key_flag = 0;
    

    if (KEY1_PRESS || KEY2_PRESS || KEY3_PRESS)
    {
        delay_nop_ms(20); 
        
        if (KEY1_PRESS)      key_flag = 1;
        else if (KEY2_PRESS) key_flag = 2;
        else if (KEY3_PRESS) key_flag = 3;
        
        if (key_flag != 0)
        {

            while ((key_flag == 1 && KEY1_PRESS) || (key_flag == 2 && KEY2_PRESS) || (key_flag == 3 && KEY3_PRESS))
            {
                delay_nop_ms(10);
                key_time += 10;
            }
        }
    }
    

    if (key_flag != 0)
    {
        if (key_time > 2000) return key_flag * 10; 
        if (key_time > 30)   return key_flag;    
    }
    
    return 0;
}
