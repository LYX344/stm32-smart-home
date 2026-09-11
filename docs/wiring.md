# 接线说明

本表根据源码中的 GPIO 定义整理，用于快速复现实物接线和功能说明。

## LED

| 功能 | STM32 引脚 | 源码位置 |
| --- | --- | --- |
| LED1 | PB11 | `User/Inc/led.h` |
| LED2 | PB14 | `User/Inc/led.h` |
| LED3 | PA11 | `User/Inc/led.h` |

## 按键

| 功能 | STM32 引脚 | 触发方式 | 源码位置 |
| --- | --- | --- | --- |
| KEY1 | PA0 | 高电平读取 | `User/Inc/key.h` |
| KEY2 | PA8 | 低电平读取 | `User/Inc/key.h` |
| KEY3 | PB10 | 低电平读取 | `User/Inc/key.h` |

## RGB

| 功能 | STM32 引脚 | 源码位置 |
| --- | --- | --- |
| RGB_CLK | PB8 | `User/Inc/rgb.h` |
| RGB_DATA | PB9 | `User/Inc/rgb.h` |

## OLED

| 功能 | STM32 引脚 | 源码位置 |
| --- | --- | --- |
| SCL | PB13 | `User/Inc/oled.h` |
| MOSI | PB15 | `User/Inc/oled.h` |
| CS | PB7 | `User/Inc/oled.h` |
| RESET | PB6 | `User/Inc/oled.h` |
| DC | PB12 | `User/Inc/oled.h` |

## DHT11

| 功能 | STM32 引脚 | 源码位置 |
| --- | --- | --- |
| DATA | PB3 | `User/Inc/dht11.h` |

说明：PB3 默认复用为 JTAG 相关引脚，源码中通过 `GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE)` 关闭 JTAG 后作为普通 GPIO 使用。

## 直流电机

| 功能 | STM32 引脚 | 定时器通道 | 源码位置 |
| --- | --- | --- | --- |
| Motor IN1 | PB4 | TIM3_CH1 重映射 | `User/Src/dc_motor.c` |
| Motor IN2 | PB5 | TIM3_CH2 重映射 | `User/Src/dc_motor.c` |

PWM 配置：

- 定时器：TIM3
- 计数周期 ARR：1000
- 预分频 PSC：72 - 1
- 占空比范围：0 到 1000

## 继电器

| 功能 | STM32 引脚 | 源码位置 |
| --- | --- | --- |
| RELAY | PB1 | `User/Inc/relay.h` |

