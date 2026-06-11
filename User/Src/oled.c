#include "main.h"                  // Device header
#include "font.h"




void oled_pin_init(void)
{
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);  //开启GPIO端口时钟
GPIO_InitTypeDef GPIO_InitStruct = {0};                                      //定义结构体变量
GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;                               //设置为推挽输出
GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_13|GPIO_Pin_15|GPIO_Pin_7|GPIO_Pin_6|GPIO_Pin_12;
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

GPIO_Init(GPIOB,&GPIO_InitStruct);

}

void oled_transfer_data(u8 data)
{
	for(u8 i=0; i < 8 ;i++)
	{
		OLED_SCL_L;

		if(data&0x80>>i)
			OLED_MOSI_H;
		else
			OLED_MOSI_L;

		OLED_SCL_H;

	}
}

void oled_send_data(u8 data)
{
OLED_DATA;
OLED_CS_L;
oled_transfer_data(data);
OLED_CS_H;
}

void oled_send_cmd(u8 cmd)
{
	OLED_CMD;
	OLED_CS_L;
	oled_transfer_data(cmd);
	OLED_CS_H;
}

  void oled_clear(void)
  {
      u8 i;
      u8 j;

      for (i = 0; i < 8; i++)
      {
          oled_send_cmd(0xB0 + i);
          oled_send_cmd(0x00);
          oled_send_cmd(0x10);

          for (j = 0; j < 132; j++)
          {
              oled_send_data(0x00);

          }
      }
  }


void oled_init(void)
{
	//管脚初始化
	oled_pin_init();
	
	//上电时间（复位/延时）
	OLED_UNRESET;
	delay_nop_ms(100);
	OLED_RESET;
	delay_nop_ms(200);
	OLED_UNRESET;
	delay_nop_ms(100);
	
	/* start initial sequence */ 
	oled_send_cmd(0xAE); //关闭显示
	oled_send_cmd(0xD5); //设置时钟分频因子,震荡频率
	oled_send_cmd(0x80); //[3:0],分频因子;[7:4],震荡频率
	oled_send_cmd(0xA8); //设置驱动路数
	oled_send_cmd(0X3F); //默认0X3F(1/64)
	oled_send_cmd(0xD3); //设置显示偏移
	oled_send_cmd(0X00); //默认为0

	oled_send_cmd(0x40); //设置显示开始行 [5:0],行数.

	oled_send_cmd(0x8D); //电荷泵设置
	oled_send_cmd(0x14); //bit2，开启/关闭
	oled_send_cmd(0x20); //设置内存地址模式
	oled_send_cmd(0x02); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
	oled_send_cmd(0xA1); //段重定义设置,bit0:0,0->0;1,0->127;
	oled_send_cmd(0xC8); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
	oled_send_cmd(0xDA); //设置COM硬件引脚配置
	oled_send_cmd(0x12); //[5:4]配置

	oled_send_cmd(0x81); //对比度设置
	oled_send_cmd(0xEF); //1~255;默认0X7F (亮度设置,越大越亮)
	oled_send_cmd(0xD9); //设置预充电周期
	oled_send_cmd(0xf1); //[3:0],PHASE 1;[7:4],PHASE 2;
	oled_send_cmd(0xDB); //设置VCOMH 电压倍率
	oled_send_cmd(0x30); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	oled_send_cmd(0xA4); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
	oled_send_cmd(0xA6); //设置显示方式;bit0:1,反相显示;0,正常显示
	oled_send_cmd(0xAF); //开启显示
		
	//清屏函数
	oled_clear();
}


void oled_show_pic(const unsigned char *pic)
{
      u8 i;
      u8 j;
      const unsigned char *p = pic + 6;

      for (i = 0; i < 8; i++)
      {
          oled_send_cmd(0xB0 + i);
          oled_send_cmd(0x00);
          oled_send_cmd(0x10);

          for (j = 0; j < 64; j++)
          {
              oled_send_data(p[i * 64 + j]);
          }
      }
}


void oled_show_pic_xy(const unsigned char *pic, int x, u8 page, u8 width, u8 height)
{
      u8 i;
      u8 j;
      u8 page_num;
      u8 src_x;
      u8 dst_x;
      u8 len;
      const unsigned char *p = pic + 6;

      page_num = height / 8;

      if (x >= 128 || x <= -width)
          return;

      if (x < 0)
      {
          src_x = -x;
          dst_x = 0;
          len = width - src_x;
      }
      else
      {
          src_x = 0;
          dst_x = x;
          len = width;
      }

      if (dst_x + len > 128)
          len = 128 - dst_x;

      for (i = 0; i < page_num; i++)
      {
          if ((page + i) >= 8)
              break;

          oled_set_pos(dst_x, page + i);

          for (j = 0; j < len; j++)
          {
              oled_send_data(p[i * width + src_x + j]);
          }
      }
}


void oled_set_pos(u8 x, u8 page)
{
      oled_send_cmd(0xB0 + page);
      oled_send_cmd(0x00 + (x & 0x0F));
      oled_send_cmd(0x10 + ((x >> 4) & 0x0F));
}


void oled_display_chinese(u8 s_col, u8 s_page, const u8 *chinese)
{
    u8 i;
    u8 j;

    for (i = s_page; i < s_page + 2; i++)
    {
        oled_send_cmd(0xB0 + i);
        oled_send_cmd(0x00 + (s_col & 0x0F));
        oled_send_cmd(0x10 + ((s_col & 0xF0) >> 4));

        for (j = 0; j < 16; j++)
        {
            oled_send_data(*chinese++);
        }
    }
}

void oled_show_chinese16(u8 x, u8 page, const unsigned char *hz)
{
    oled_display_chinese(x, page, hz);
}

void oled_display_char(u8 s_col, u8 s_page, u8 pic)
{
    u8 i;
    u8 j;
    u16 num = (pic - ' ') * 16;

    for (i = s_page; i < s_page + 2; i++)
    {
        oled_send_cmd(0xB0 + i);
        oled_send_cmd(0x00 + (s_col & 0x0F));
        oled_send_cmd(0x10 + ((s_col & 0xF0) >> 4));

        for (j = 0; j < 8; j++)
        {
            oled_send_data(char_code[num++]);
        }
    }
}

void oled_display_str(u8 s_col, u8 s_page, const char *pic)
{
    u16 n;
    const u8 *str = (const u8 *)pic;

    while (*str != 0)
    {
        if (*str > 0xA1 && *(str + 1) != 0)
        {
            for (n = 0; n < sizeof(chinese_code) / sizeof(chinese_code[0]); n++)
            {
                if ((*str == chinese_code[n].gbcode[0]) && (*(str + 1) == chinese_code[n].gbcode[1]))
                {
                    oled_display_chinese(s_col, s_page, chinese_code[n].bitmap);
                    break;
                }
            }

            s_col += 16;
            str += 2;
        }
        else
        {
            oled_display_char(s_col, s_page, *str);
            s_col += 8;
            str += 1;
        }
    }
}


 void oled_clear_area(u8 x, u8 page, u8 width, u8 height)
{
      u8 i;
      u8 j;
      u8 page_num;

      page_num = height / 8;

      for (i = 0; i < page_num; i++)
      {
          oled_set_pos(x, page + i);

          for (j = 0; j < width; j++)
          {
              oled_send_data(0x00);
          }
      }
}
