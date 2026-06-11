#include "main.h"
#include "string.h"
#include "stdio.h"

#define PIC_NUM         7
#define BIG_ICON_X      40
#define BIG_ICON_PAGE   2
#define BIG_ICON_W      48
#define BIG_ICON_H      48

#define LEFT_ICON_X     8
#define RIGHT_ICON_X    96
#define SMALL_ICON_PAGE 3
#define SMALL_ICON_W    24
#define SMALL_ICON_H    24

#define FAN_ICON_X      52
#define FAN_ICON_PAGE   5
#define FAN_ICON_W      24


#define PAGE_MENU       0
#define PAGE_FUNCTION   1

#define FUNC_LED        0
#define FUNC_RGB        1
#define FUNC_DC_MOTOR   2
#define FUNC_DHT11      3
#define FUNC_HOME       4
#define FUNC_CHARGE     5
#define FUNC_TIME       6

#define LED_EXIT_SELECT 3
#define RGB_EXIT_SELECT 3

#define TIME_INIT_YEAR  2026
#define TIME_INIT_MON   5
#define TIME_INIT_DAY   17
#define TIME_INIT_HOUR  16
#define TIME_INIT_MIN   0
#define TIME_INIT_SEC   0


u8 old_pic = 0;
u8 page_state = PAGE_MENU;
u8 led1_state = 0;
u8 led2_state = 0;
u8 led3_state = 0;
u8 led_select = 0;
u8 led_adjust = 0;
u8 rgb_red = 0;
u8 rgb_green = 0;
u8 rgb_blue = 0;
u8 rgb_select = 0;
u8 rgb_adjust = 0;
u16 motor_speed = 0;
u16 dht11_refresh_ms = 0;
u8 relay_state = 0;
u8 fan_frame = 0;
u16 fan_anim_ms = 0;
u16 time_year = TIME_INIT_YEAR;
u8 time_mon = TIME_INIT_MON;
u8 time_day = TIME_INIT_DAY;
u8 time_hour = TIME_INIT_HOUR;
u8 time_min = TIME_INIT_MIN;
u8 time_sec = TIME_INIT_SEC;
u8 time_running = 1;
u16 time_refresh_ms = 0;




const unsigned char *pic_big[PIC_NUM] = {
      gImage_led_48,
      gImage_rgb_48,
      gImage_dc_motor_48,
      gImage_dht11_48,
      gImage_home_48,
      gImage_charge_48,
      gImage_time_48
  };

const unsigned char *pic_small[PIC_NUM] = {
      gImage_led_24,
      gImage_rgb_24,
      gImage_dc_motor_24,
      gImage_dht11_24,
      gImage_home_24,
      gImage_charge_24,
      gImage_time_24
};

const char hz_led[] = "\xB5\xC6";
const char hz_rgb[] = "\xB2\xCA\xB5\xC6";
const char hz_fan[] = "\xB7\xE7\xC9\xC8";
const char hz_dht11[] = "\xCE\xC2\xCA\xAA\xB6\xC8";
const char hz_home[] = "\xD4\xC6\xBC\xD2\xBE\xD3";
const char hz_charge[] = "\xB3\xE4\xB5\xE7";
const char hz_time[] = "\xCA\xB1\xBC\xE4";
const char hz_select[] = "\xD1\xA1\xD4\xF1";
const char hz_adjust[] = "\xB5\xF7\xD5\xFB";
const char hz_exit[] = "\xCD\xCB\xB3\xF6";
const char hz_on[] = "\xBF\xAA";
const char hz_off[] = "\xB9\xD8";
const char hz_speed[] = "\xCB\xD9\xB6\xC8";
const char hz_temp[] = "\xCE\xC2\xB6\xC8";
const char hz_humi[] = "\xCA\xAA\xB6\xC8";
const char hz_error[] = "\xB4\xED\xCE\xF3";
const char hz_red[] = "\xBA\xEC";
const char hz_green[] = "\xC2\xCC";
const char hz_blue[] = "\xC0\xB6";
const char hz_run[] = "\xD4\xCB\xD0\xD0";
const char hz_stop[] = "\xCD\xA3\xD6\xB9";
const char hz_home_page[] = "\xD4\xC6\xBC\xD2\xBE\xD3\xD2\xB3\xC3\xE6";

const char *pic_name[PIC_NUM] = {
      hz_led,
      hz_rgb,
      hz_fan,
      hz_dht11,
      hz_home,
      hz_charge,
      hz_time
};


 u8 pic_next(u8 pic_cnt)
{
      if (pic_cnt < PIC_NUM - 1)
          pic_cnt++;
      else
          pic_cnt = 0;

      return pic_cnt;
}

  u8 pic_last(u8 pic_cnt)
{
      if (pic_cnt > 0)
          pic_cnt--;
      else
          pic_cnt = PIC_NUM - 1;

      return pic_cnt;
}

void show_pic_page(u8 pic_cnt)
{
      u8 left_pic;
      u8 right_pic;

      left_pic = pic_last(pic_cnt);
      right_pic = pic_next(pic_cnt);

      oled_clear();

      oled_display_str((128 - strlen(pic_name[pic_cnt]) * 8) / 2, 0, pic_name[pic_cnt]);

      oled_show_pic_xy(pic_small[left_pic], LEFT_ICON_X, SMALL_ICON_PAGE, SMALL_ICON_W, SMALL_ICON_H);
      oled_show_pic_xy(pic_big[pic_cnt], BIG_ICON_X, BIG_ICON_PAGE, BIG_ICON_W, BIG_ICON_H);
      oled_show_pic_xy(pic_small[right_pic], RIGHT_ICON_X, SMALL_ICON_PAGE, SMALL_ICON_W, SMALL_ICON_H);
}



void show_pic_slide(u8 old_pic, u8 new_pic, u8 dir)
{
      u8 step;
      int old_x;
      int new_x;

      for (step = 0; step <= 88; step += 4)
  {
      oled_clear_area(0, BIG_ICON_PAGE, 128, BIG_ICON_H);

      if (dir == 1)
      {
          old_x = BIG_ICON_X - step;
          new_x = 128 - step;
      }
      else
      {
          old_x = BIG_ICON_X + step;
          new_x = -BIG_ICON_W + step;
      }

      oled_show_pic_xy(pic_big[old_pic], old_x, BIG_ICON_PAGE, BIG_ICON_W, BIG_ICON_H);
      oled_show_pic_xy(pic_big[new_pic], new_x, BIG_ICON_PAGE, BIG_ICON_W, BIG_ICON_H);

      delay_nop_ms(8);
  }
}

void show_led_page(void)
{
      char str[16];

      oled_clear();

      oled_display_str(0, 0, hz_led);
      oled_display_str(32, 0, led_adjust ? hz_adjust : hz_select);
      oled_display_str(80, 0, led_select == LED_EXIT_SELECT ? ">" : " ");
      oled_display_str(88, 0, hz_exit);

      sprintf(str, "%c%s1 %s", led_select == 0 ? '>' : ' ', hz_led, led1_state ? hz_on : hz_off);
      oled_display_str(16, 2, str);

      sprintf(str, "%c%s2 %s", led_select == 1 ? '>' : ' ', hz_led, led2_state ? hz_on : hz_off);
      oled_display_str(16, 4, str);

      sprintf(str, "%c%s3 %s", led_select == 2 ? '>' : ' ', hz_led, led3_state ? hz_on : hz_off);
      oled_display_str(16, 6, str);
}

void show_rgb_page(void)
{
      char str[16];

      oled_clear();

      oled_display_str(0, 0, hz_rgb);
      oled_display_str(40, 0, rgb_adjust ? hz_adjust : hz_select);
      oled_display_str(80, 0, rgb_select == RGB_EXIT_SELECT ? ">" : " ");
      oled_display_str(88, 0, hz_exit);

      sprintf(str, "%c%s:%3d", rgb_select == 0 ? '>' : ' ', hz_red, rgb_red);
      oled_display_str(24, 2, str);

      sprintf(str, "%c%s:%3d", rgb_select == 1 ? '>' : ' ', hz_green, rgb_green);
      oled_display_str(24, 4, str);

      sprintf(str, "%c%s:%3d", rgb_select == 2 ? '>' : ' ', hz_blue, rgb_blue);
      oled_display_str(24, 6, str);
}

u8 fan_pic_get_pixel(u8 x, u8 y)
{
      const unsigned char *p = gImage_dc_motor_24 + 6;

      if (p[(y / 8) * FAN_ICON_W + x] & (0x01 << (y % 8)))
          return 1;

      return 0;
}

void oled_show_fan_rotate(u8 frame)
{
      u8 i;
      u8 j;
      u8 bit;
      u8 data;
      u8 src_x;
      u8 src_y;
      u8 dst_y;

      frame %= 4;

      for (i = 0; i < 3; i++)
      {
          oled_set_pos(FAN_ICON_X, FAN_ICON_PAGE + i);

          for (j = 0; j < FAN_ICON_W; j++)
          {
              data = 0;

              for (bit = 0; bit < 8; bit++)
              {
                  dst_y = i * 8 + bit;

                  if (frame == 0)
                  {
                      src_x = j;
                      src_y = dst_y;
                  }
                  else if (frame == 1)
                  {
                      src_x = dst_y;
                      src_y = FAN_ICON_W - 1 - j;
                  }
                  else if (frame == 2)
                  {
                      src_x = FAN_ICON_W - 1 - j;
                      src_y = FAN_ICON_W - 1 - dst_y;
                  }
                  else
                  {
                      src_x = FAN_ICON_W - 1 - dst_y;
                      src_y = j;
                  }

                  if (fan_pic_get_pixel(src_x, src_y))
                      data |= 0x01 << bit;
              }

              oled_send_data(data);
          }
      }
}

void show_dc_motor_page(void)
{
      char str[16];

      oled_clear();

      oled_display_str(48, 0, hz_fan);

      sprintf(str, "%s:%4d", hz_speed, motor_speed);
      oled_display_str(16, 2, str);

      oled_show_fan_rotate(fan_frame);
}

void show_fan_anim(void)
{
      oled_show_fan_rotate(fan_frame);
}

u16 get_fan_anim_time(void)
{
      if (motor_speed == 0)
          return 0;

      return 260 - motor_speed / 5;
}

void show_dht11_page(void)
{
      char str[16];
      u8 ret;

      oled_clear();

      oled_display_str(40, 0, hz_dht11);

      ret = dht11_read();

      if (ret == 0)
      {
          sprintf(str, "%s:%3d", hz_temp, temperature);
          oled_display_str(16, 3, str);

          sprintf(str, "%s:%3d", hz_humi, humidity);
          oled_display_str(16, 5, str);
      }
      else
      {
          sprintf(str, "%s:%d", hz_error, ret);
          oled_display_str(32, 3, str);
      }
}

void show_charge_page(void)
{
      char str[16];

      oled_clear();

      oled_display_str(48, 0, hz_charge);

      sprintf(str, "%s %s", hz_charge, relay_state ? hz_on : hz_off);
      oled_display_str(32, 3, str);
}

void time_reset(void)
{
      time_year = TIME_INIT_YEAR;
      time_mon = TIME_INIT_MON;
      time_day = TIME_INIT_DAY;
      time_hour = TIME_INIT_HOUR;
      time_min = TIME_INIT_MIN;
      time_sec = TIME_INIT_SEC;
      time_refresh_ms = 0;
}

void time_add_second(void)
{
      time_sec++;

      if (time_sec >= 60)
      {
          time_sec = 0;
          time_min++;
      }

      if (time_min >= 60)
      {
          time_min = 0;
          time_hour++;
      }

      if (time_hour >= 24)
          time_hour = 0;
}

void show_time_page(void)
{
      char str[16];

      oled_clear();

      oled_display_str(48, 0, hz_time);

      sprintf(str, "%04d/%02d/%02d", time_year, time_mon, time_day);
      oled_display_str(16, 2, str);

      sprintf(str, "%02d:%02d:%02d", time_hour, time_min, time_sec);
      oled_display_str(28, 4, str);

      oled_display_str(48, 6, time_running ? hz_run : hz_stop);
}

void show_function_page(u8 func)
{
      oled_clear();

      switch (func)
      {
          case FUNC_LED:
              led_adjust = 0;
              show_led_page();
              break;

          case FUNC_RGB:
              rgb_adjust = 0;
              show_rgb_page();
              break;

          case FUNC_DC_MOTOR:
              show_dc_motor_page();
              break;

          case FUNC_DHT11:
              show_dht11_page();
              break;

          case FUNC_HOME:
              oled_display_str(40, 0, hz_home);
              oled_display_str(24, 3, hz_home_page);
              break;

          case FUNC_CHARGE:
              show_charge_page();
              break;

          case FUNC_TIME:
              show_time_page();
              break;

          default:
              break;
      }
}

void led_set_state(void)
{
      if (led1_state) LED1_ON();
      else            LED1_OFF();

      if (led2_state) LED2_ON();
      else            LED2_OFF();

      if (led3_state) LED3_ON();
      else            LED3_OFF();
}

void relay_set_state(void)
{
      if (relay_state) RELAY_ON();
      else             RELAY_OFF();
}

void led_set_select_state(u8 state)
{
      if (led_select == 0)
          led1_state = state;
      else if (led_select == 1)
          led2_state = state;
      else
          led3_state = state;
}

u8 rgb_add_limit(u8 value, u8 step)
{
      if (value >= 255 - step)
          return 255;

      return value + step;
}

u8 rgb_sub_limit(u8 value, u8 step)
{
      if (value <= step)
          return 0;

      return value - step;
}

void rgb_change_select(u8 dir, u8 step)
{
      if (rgb_select == 0)
      {
          if (dir)
              rgb_red = rgb_add_limit(rgb_red, step);
          else
              rgb_red = rgb_sub_limit(rgb_red, step);
      }
      else if (rgb_select == 1)
      {
          if (dir)
              rgb_green = rgb_add_limit(rgb_green, step);
          else
              rgb_green = rgb_sub_limit(rgb_green, step);
      }
      else
      {
          if (dir)
              rgb_blue = rgb_add_limit(rgb_blue, step);
          else
              rgb_blue = rgb_sub_limit(rgb_blue, step);
      }
}

u8 handle_function_key(u8 func, u8 key)
{
      if (key == 20)
          return 1;

      switch (func)
      {
          case FUNC_LED:
              if (led_adjust == 0)
              {
                  if (key == 1 || key == 10)
                      led_select = led_select == 0 ? LED_EXIT_SELECT : led_select - 1;
                  else if (key == 3 || key == 30)
                      led_select = led_select == LED_EXIT_SELECT ? 0 : led_select + 1;
                  else if (key == 2)
                  {
                      if (led_select == LED_EXIT_SELECT)
                          return 1;

                      led_adjust = 1;
                  }
                  else
                      break;
              }
              else
              {
                  if (key == 1 || key == 10)
                      led_set_select_state(0);
                  else if (key == 3 || key == 30)
                      led_set_select_state(1);
                  else if (key == 2)
                      led_adjust = 0;
                  else
                      break;

                  led_set_state();
              }

              show_led_page();
              break;

          case FUNC_RGB:
              if (rgb_adjust == 0)
              {
                  if (key == 1 || key == 10)
                      rgb_select = rgb_select == 0 ? RGB_EXIT_SELECT : rgb_select - 1;
                  else if (key == 3 || key == 30)
                      rgb_select = rgb_select == RGB_EXIT_SELECT ? 0 : rgb_select + 1;
                  else if (key == 2)
                  {
                      if (rgb_select == RGB_EXIT_SELECT)
                          return 1;

                      rgb_adjust = 1;
                  }
                  else
                      break;
              }
              else
              {
                  if (key == 1 || key == 3 || key == 10 || key == 30)
                  {
                      if (key == 1)
                          rgb_change_select(0, 1);
                      else if (key == 3)
                          rgb_change_select(1, 1);
                      else if (key == 10)
                          rgb_change_select(0, 20);
                      else
                          rgb_change_select(1, 20);

                      rgb_show(rgb_red, rgb_green, rgb_blue);
                  }
                  else if (key == 2)
                  {
                      rgb_adjust = 0;
                  }
                  else
                  {
                      break;
                  }
              }

              show_rgb_page();
              break;

          case FUNC_DC_MOTOR:
              if (key == 3)
              {
                  if (motor_speed <= 900)
                      motor_speed += 100;
                  else
                      motor_speed = 1000;
              }
              else if (key == 1)
              {
                  if (motor_speed >= 100)
                      motor_speed -= 100;
                  else
                      motor_speed = 0;
              }
              else if (key == 2)
              {
                  motor_speed = 0;
              }
              else
              {
                  break;
              }

              dc_motor_left(motor_speed);
              show_dc_motor_page();
              show_fan_anim();
              break;

          case FUNC_DHT11:
              if (key == 1 || key == 2 || key == 3)
                  show_dht11_page();
              break;

          case FUNC_CHARGE:
              if (key == 1)
                  relay_state = 0;
              else if (key == 3)
                  relay_state = 1;
              else
                  break;

              relay_set_state();
              show_charge_page();
              break;

          case FUNC_HOME:
              if (key == 2)
                  return 1;
              break;

          case FUNC_TIME:
              if (key == 1)
              {
                  time_reset();
                  show_time_page();
              }
              else if (key == 2)
              {
                  show_time_page();
              }
              else if (key == 3)
              {
                  time_running = !time_running;
                  show_time_page();
              }
              break;

          default:
              break;
      }

      return 0;
}


int main(void)
{
	u8 key = 0;
  u8 pic_cnt = 0;
	led_init();
	key_init();
	rgb_init();
	relay_init();
	oled_init();
	dht11_init();
	dc_motor_init();

      show_pic_page(pic_cnt);

while (1)
{
      key = key_scanf();

      if (page_state == PAGE_MENU)
      {
          switch (key)
          {
              case 1:
                  old_pic = pic_cnt;
                  pic_cnt = pic_last(pic_cnt);

                  show_pic_slide(old_pic, pic_cnt, 3);
                  show_pic_page(pic_cnt);
                  break;

              case 3:
                  old_pic = pic_cnt;
                  pic_cnt = pic_next(pic_cnt);

                  show_pic_slide(old_pic, pic_cnt, 1);
                  show_pic_page(pic_cnt);
                  break;

              case 2:
                  page_state = PAGE_FUNCTION;
                  show_function_page(pic_cnt);
                  break;

              default:
                  break;
          }
      }
      else
      {
          if (handle_function_key(pic_cnt, key))
          {
              page_state = PAGE_MENU;
              show_pic_page(pic_cnt);
          }

          if (pic_cnt == FUNC_DHT11 && key == 0)
          {
              delay_nop_ms(100);
              dht11_refresh_ms += 100;

              if (dht11_refresh_ms >= 2000)
              {
                  dht11_refresh_ms = 0;
                  show_dht11_page();
              }
          }

          if (pic_cnt == FUNC_DC_MOTOR && key == 0)
          {
              u16 fan_anim_time;

              delay_nop_ms(50);
              fan_anim_time = get_fan_anim_time();

              if (fan_anim_time != 0)
              {
                  fan_anim_ms += 50;

                  if (fan_anim_ms >= fan_anim_time)
                  {
                      fan_anim_ms = 0;
                      fan_frame++;

                      if (fan_frame >= 4)
                          fan_frame = 0;

                      show_fan_anim();
                  }
              }
          }

          if (pic_cnt == FUNC_TIME && key == 0)
          {
              delay_nop_ms(100);

              if (time_running)
              {
                  time_refresh_ms += 100;

                  if (time_refresh_ms >= 1000)
                  {
                      time_refresh_ms = 0;
                      time_add_second();
                      show_time_page();
                  }
              }
          }
      }
}
}
