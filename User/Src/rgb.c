#include "main.h"  

void rgb_init(void)
{
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);  //开启GPIO端口时钟
GPIO_InitTypeDef GPIO_InitStruct = {0};                                      //定义结构体变量
GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;                               //设置为推挽输出
GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_8|GPIO_Pin_9;
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

GPIO_Init(GPIOB,&GPIO_InitStruct);

RGB_CLK_L;
RGB_DATA_H;
rgb_show(0,0,0);

}

void rgb_send_data(u8 data)
{
	for(u8 i=0; i < 8 ;i++)
	{
		RGB_CLK_L;
		delay_nop_us(20);
		if(data&0x80>>i)
			RGB_DATA_H;
		else
			RGB_DATA_L;
		delay_nop_us(20);
		RGB_CLK_H;
		delay_nop_us(20);
	}
}
void rgb_show(u8 red, u8 green, u8 blue)
{
      u8 prefix = 0xC0;

      if ((blue & 0x80) == 0)  prefix |= 0x20;
      if ((blue & 0x40) == 0)  prefix |= 0x10;
      if ((green & 0x80) == 0) prefix |= 0x08;
      if ((green & 0x40) == 0) prefix |= 0x04;
      if ((red & 0x80) == 0)   prefix |= 0x02;
      if ((red & 0x40) == 0)   prefix |= 0x01;

      rgb_send_data(0x00);
      rgb_send_data(0x00);
      rgb_send_data(0x00);
      rgb_send_data(0x00);

      rgb_send_data(prefix);
      rgb_send_data(blue);
      rgb_send_data(green);
      rgb_send_data(red);

      rgb_send_data(0x00);
      rgb_send_data(0x00);
      rgb_send_data(0x00);
      rgb_send_data(0x00);
}
