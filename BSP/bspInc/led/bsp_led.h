#ifndef __BSP_LED_H__
#define __BSP_LED_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* 类型定义 ------------------------------------------------------------------*/
typedef enum
{
  LED_OFF = 0,
  LED_ON  = 1,
  LED_TOGGLE = 2,
}LEDState_TypeDef;
#define IS_LED_STATE(STATE)           (((STATE) == LED_OFF) || ((STATE) == LED_ON) || ((STATE) == LED_TOGGLE))

/* 宏定义 --------------------------------------------------------------------*/
#define LED_RED                          (uint8_t)0x01
#define LED_GREEN                        (uint8_t)0x02
#define LED_BLUE                         (uint8_t)0x04
#define IS_LED_TYPEDEF(LED)           (((LED) == LED_RED) || ((LED) == LED_GREEN) || ((LED) == LED_BLUE))

/*
 * 以下宏定义内容跟开发板硬件息息相关，需要查看开发板电路原理图才能正确编写。
 * 例如，查原理图可在LED_RED灯接在stm32f103芯片的PC15引脚上，所以有关LED_RED的宏定义
 * 都是与GPIOC，GPIO_Pin_15相关的，专门把这些与开发板硬件相关的内容定义为宏，
 * 这对于修改或者移植程序非常方便。
 */
#define LED_RED_RCC_CLK_ENABLE()            __HAL_RCC_GPIOC_CLK_ENABLE()
#define LED_RED_GPIO_PIN                    GPIO_PIN_15
#define LED_RED_GPIO                        GPIOC


#define LED_GREEN_RCC_CLK_ENABLE()          __HAL_RCC_GPIOC_CLK_ENABLE()
#define LED_GREEN_GPIO_PIN                  GPIO_PIN_14
#define LED_GREEN_GPIO                      GPIOC


#define LED_BLUE_RCC_CLK_ENABLE()           __HAL_RCC_GPIOC_CLK_ENABLE()
#define LED_BLUE_GPIO_PIN                   GPIO_PIN_13
#define LED_BLUE_GPIO                       GPIOC

#define LED_RED_ON                          HAL_GPIO_WritePin(LED_RED_GPIO,LED_RED_GPIO_PIN,GPIO_PIN_SET)    // 输出高电平
#define LED_RED_OFF                         HAL_GPIO_WritePin(LED_RED_GPIO,LED_RED_GPIO_PIN,GPIO_PIN_RESET)  // 输出低电平
#define LED_RED_TOGGLE                      HAL_GPIO_TogglePin(LED_RED_GPIO,LED_RED_GPIO_PIN)                // 输出反转

#define LED_GREEN_ON                        HAL_GPIO_WritePin(LED_GREEN_GPIO,LED_GREEN_GPIO_PIN,GPIO_PIN_SET)    // 输出高电平
#define LED_GREEN_OFF                       HAL_GPIO_WritePin(LED_GREEN_GPIO,LED_GREEN_GPIO_PIN,GPIO_PIN_RESET)  // 输出低电平
#define LED_GREEN_TOGGLE                    HAL_GPIO_TogglePin(LED_GREEN_GPIO,LED_GREEN_GPIO_PIN)                // 输出反转

#define LED_BLUE_ON                         HAL_GPIO_WritePin(LED_BLUE_GPIO,LED_BLUE_GPIO_PIN,GPIO_PIN_SET)    // 输出高电平
#define LED_BLUE_OFF                        HAL_GPIO_WritePin(LED_BLUE_GPIO,LED_BLUE_GPIO_PIN,GPIO_PIN_RESET)  // 输出低电平
#define LED_BLUE_TOGGLE                     HAL_GPIO_TogglePin(LED_BLUE_GPIO,LED_BLUE_GPIO_PIN)                // 输出反转


/* 扩展变量 ------------------------------------------------------------------*/
/* 函数声明 ------------------------------------------------------------------*/
void LED_GPIO_Init(void);
void LEDx_StateSet(uint8_t LEDx,LEDState_TypeDef state);

#endif  // __BSP_LED_H__

/******************* (C) COPYRIGHT 2015-2020 kian *****END OF FILE****/
