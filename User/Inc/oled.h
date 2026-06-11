#ifndef __OLED_H
#define __OLED_H
#include "main.h"                  // Device header

#define   OLED_SCL_H     GPIO_SetBits(GPIOB,GPIO_Pin_13)
#define   OLED_SCL_L    GPIO_ResetBits(GPIOB,GPIO_Pin_13)

#define   OLED_MOSI_H     GPIO_SetBits(GPIOB,GPIO_Pin_15)
#define   OLED_MOSI_L    GPIO_ResetBits(GPIOB,GPIO_Pin_15)

#define   OLED_CS_H     GPIO_SetBits(GPIOB,GPIO_Pin_7)
#define   OLED_CS_L     GPIO_ResetBits(GPIOB,GPIO_Pin_7)

#define   OLED_UNRESET     GPIO_SetBits(GPIOB,GPIO_Pin_6)
#define   OLED_RESET    GPIO_ResetBits(GPIOB,GPIO_Pin_6)

#define   OLED_DATA     GPIO_SetBits(GPIOB,GPIO_Pin_12)
#define   OLED_CMD    GPIO_ResetBits(GPIOB,GPIO_Pin_12)

  void oled_pin_init(void);
  void oled_transfer_data(u8 data);
  void oled_send_cmd(u8 cmd);
  void oled_send_data(u8 data);
  void oled_clear(void);
  void oled_init(void);
  void oled_show_pic(const unsigned char *pic);
  void oled_set_pos(u8 x, u8 page);
  void oled_display_chinese(u8 s_col, u8 s_page, const u8 *chinese);
  void oled_show_chinese16(u8 x, u8 page, const unsigned char *hz);
  void oled_display_char(u8 x, u8 page, u8 chr);
  void oled_display_str(u8 s_col, u8 s_page, const char *pic);
  void oled_show_pic_xy(const unsigned char *pic, int x, u8 page, u8 width, u8 height);
  void oled_clear_area(u8 x, u8 page, u8 width, u8 height);

#endif

