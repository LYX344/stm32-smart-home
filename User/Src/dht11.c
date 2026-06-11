#include "main.h"                  // Device header

u8 buffer[5] = {0};
u8 temperature = 0;
u8 humidity = 0;

void dht11_init(void)
{
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB,ENABLE);  //开启GPIO端口时钟
GPIO_InitTypeDef GPIO_InitStruct = {0};                                      //定义结构体变量
GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_OD;                               //设置为推挽输出
GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_3;
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
GPIO_Init(GPIOB,&GPIO_InitStruct);


DHT11_DATA_H;
delay_nop_ms(1000);
}

static void dht11_io_out(void)
{
      GPIO_InitTypeDef GPIO_InitStruct = {0};

      GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
      GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_Init(GPIOB, &GPIO_InitStruct);
}

static void dht11_io_in(void)
{
      GPIO_InitTypeDef GPIO_InitStruct = {0};

      GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
      GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
      GPIO_Init(GPIOB, &GPIO_InitStruct);
}



u8 dht11_read(void)
{
      u8 i, j;
      u16 timeout;

      for (i = 0; i < 5; i++)
      {
          buffer[i] = 0;
      }

      dht11_io_out();
      DHT11_DATA_L;
      delay_nop_ms(20);
      DHT11_DATA_H;
      delay_nop_us(30);
      dht11_io_in();

      timeout = 0;
      while (DHT11_DATA_IN)
      {
          if (++timeout > 1000) return 1;
      }

      timeout = 0;
      while (!DHT11_DATA_IN)
      {
          if (++timeout > 1000) return 2;
      }

      timeout = 0;
      while (DHT11_DATA_IN)
      {
          if (++timeout > 1000) return 3;
      }

      for (i = 0; i < 5; i++)
      {
          for (j = 0; j < 8; j++)
          {
              timeout = 0;
              while (!DHT11_DATA_IN)
              {
                  if (++timeout > 1000) return 4;
              }

              delay_nop_us(40);

              buffer[i] <<= 1;
              if (DHT11_DATA_IN)
              {
                  buffer[i] |= 1;
              }

              timeout = 0;
              while (DHT11_DATA_IN)
              {
                  if (++timeout > 1000) return 5;
              }
          }
      }

      dht11_io_out();
      DHT11_DATA_H;

      if ((u8)(buffer[0] + buffer[1] + buffer[2] + buffer[3]) != buffer[4])
      {
          return 6;
      }
			humidity = buffer[0];
			temperature = buffer[2];
      return 0;
}
