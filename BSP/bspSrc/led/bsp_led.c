/**
  ******************************************************************************
  * 文件名程: bsp_led.c 
  * 作    者: kian
  * 版    本: V1.0
  * 编写日期: 2017-07-19
  * 功    能: 基于HAL库的板载LED灯底层驱动实现
  ******************************************************************************
  * 说明：
  * 
  * 
  ******************************************************************************
  */

/* 包含头文件 ----------------------------------------------------------------*/
#include "led/bsp_led.h"

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/

/**
  * 函数功能: 板载LED灯IO引脚初始化.
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：使用宏定义方法代替具体引脚号，方便程序移植，只要简单修改bsp_led.h
  *           文件相关宏定义就可以方便修改引脚。
  */
void LED_GPIO_Init(void)
{
    /* 定义IO硬件初始化结构体变量 */
  GPIO_InitTypeDef GPIO_InitStruct;
	
    /* 使能(开启)LED引脚对应IO端口时钟 */  
  LED_RED_RCC_CLK_ENABLE();
  LED_GREEN_RCC_CLK_ENABLE();
  LED_BLUE_RCC_CLK_ENABLE();
  
  /* 配置LED_RED引脚输出电压 */
  HAL_GPIO_WritePin(LED_RED_GPIO, LED_RED_GPIO_PIN, GPIO_PIN_RESET);

  /* 配置LED_GREEN引脚输出电压 */
  HAL_GPIO_WritePin(LED_GREEN_GPIO, LED_GREEN_GPIO_PIN, GPIO_PIN_RESET);

  /* 配置LED_BLUE引脚输出电压 */
  HAL_GPIO_WritePin(LED_BLUE_GPIO, LED_BLUE_GPIO_PIN, GPIO_PIN_RESET);
  
  /* 设定LED_RED对应引脚IO编号 */
  GPIO_InitStruct.Pin = LED_RED_GPIO_PIN;
  /* 设定LED_RED对应引脚IO为输出模式 */
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  /* 设定LED_RED对应引脚IO操作速度 */
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  /* 初始化LED_RED对应引脚IO */
  HAL_GPIO_Init(LED_RED_GPIO, &GPIO_InitStruct);
  
  /* 设定LED_GREEN对应引脚IO编号 */
  GPIO_InitStruct.Pin = LED_GREEN_GPIO_PIN;
  /* 设定LED_GREEN对应引脚IO为输出模式 */
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  /* 设定LED_GREEN对应引脚IO操作速度 */
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  /* 初始化LED2对应引脚IO */
  HAL_GPIO_Init(LED_GREEN_GPIO, &GPIO_InitStruct);
  
  /* 设定LED_BLUE对应引脚IO编号 */
  GPIO_InitStruct.Pin = LED_BLUE_GPIO_PIN;
  /* 设定LED_BLUE对应引脚IO为输出模式 */
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  /* 设定LED_BLUE对应引脚IO操作速度 */
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  /* 初始化LED_BLUE对应引脚IO */
  HAL_GPIO_Init(LED_BLUE_GPIO, &GPIO_InitStruct);
  
}

/**
  * 函数功能: 设置板载LED灯的状态
  * 输入参数: LEDx:其中x可甚至为(1,2,3)用来选择对应的LED灯
  * 输入参数：state:设置LED灯的输出状态。
  *             可选值：LED_OFF：LED灯灭；
  *                     LED_ON： LED灯亮。
  *                     LED_TOGGLE：反转LED
  * 返 回 值: 无
  * 说    明：该函数使用类似标准库函数的编程方法，方便理解标准库函数编程思想。
  */
void LEDx_StateSet(uint8_t LEDx,LEDState_TypeDef state)
{
  /* 检查输入参数是否合法 */
  assert_param(IS_LED_TYPEDEF(LEDx));
  assert_param(IS_LED_STATE(state));
  
  /* 判断设置的LED灯状态，如果设置为LED灯灭 */
  if(state==LED_OFF)
  {
    if(LEDx & LED_RED)            
      LED_RED_OFF;/* LED_RED灭 */
    
    if(LEDx & LED_GREEN)
      LED_GREEN_OFF;/* LED_GREEN灭 */
    
    if(LEDx & LED_BLUE)
      LED_BLUE_OFF;/* LED_BLUE灭 */    
  }
  else if(state==LED_ON) /* 设置LED灯为亮 */
  {
    if(LEDx & LED_RED)
      LED_RED_ON;/* LED_RED亮 */
    
    if(LEDx & LED_GREEN)
      LED_GREEN_ON;/* LED_GREEN亮 */
    
    if(LEDx & LED_BLUE)
      LED_BLUE_ON;/* LED_BLUE亮 */ 
  }
  else
  {
    if(LEDx & LED_RED)
      LED_RED_TOGGLE;/* 设置引脚输出反转 */
    
    if(LEDx & LED_GREEN)
      LED_GREEN_TOGGLE;/* 设置引脚输出反转 */
    
    if(LEDx & LED_BLUE)
      LED_BLUE_TOGGLE;/* 设置引脚输出反转 */ 
  }
}

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
