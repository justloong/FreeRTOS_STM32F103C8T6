#ifndef __BASIC_TIM_H__
#define __BASIC_TIM_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/
/**********************************************************************
 * 基本定时器只有大容量的MCU产品才拥有
 * __________________________________________________________________________________
 * |        小容量          |        中等容量          |             大容量                   |
 * | 16Kflash | 32Kflash | 64Kflash | 128kflash | 256kflash | 384Kflash | 512Kflash |
 * | 6K RAM   | 10K RAM  | 20K RAM  | 20K RAM   | 48K RAM   | 64K RAM   | 64K RAM   |
 * |__________|__________|__________|___________|___________|___________|___________|
 **********************************************************************/
/********************基本定时器TIM参数定义，只限TIM6 & TIM7************/
// 如果使用TIM7，注释掉下面宏定义即可
#define USE_TIM6

#ifdef  USE_TIM6  // 使用基本定时器TIM6
#define BASIC_TIMx                     TIM6
#define BASIC_TIM_RCC_CLK_ENABLE()     __HAL_RCC_TIM6_CLK_ENABLE()
#define BASIC_TIM_RCC_CLK_DISABLE()    __HAL_RCC_TIM6_CLK_DISABLE()
#define BASIC_TIM_IRQ                  TIM6_IRQn
#define BASIC_TIM_INT_FUN              TIM6_IRQHandler

#else  // 使用基本定时器TIM7

#define BASIC_TIMx                     TIM7
#define BASIC_TIM_RCC_CLK_ENABLE()     __HAL_RCC_TIM7_CLK_ENABLE()
#define BASIC_TIM_RCC_CLK_DISABLE()    __HAL_RCC_TIM7_CLK_DISABLE()
#define BASIC_TIM_IRQ                  TIM7_IRQn
#define BASIC_TIM_INT_FUN              TIM7_IRQHandler

#endif

// 定义定时器预分频，定时器实际时钟频率为：72MHz/（BASIC_TIMx_PRESCALER+1）
#define BASIC_TIMx_PRESCALER           0  // 实际时钟频率为：1MHz
// 定义定时器周期，当定时器开始计数到BASIC_TIMx_PERIOD值是更新定时器并生成对应事件和中断
#define BASIC_TIMx_PERIOD              3600  // 定时器产生中断频率为：1MHz/100=10KHz，即100us定时周期

// 最终定时器频率计算为： 72MHz/（BASIC_TIMx_PRESCALER+1）/BASIC_TIMx_PERIOD
// 比如需要产生20ms周期定时，可以设置为： 72MHz/（359+1）/4000=50Hz，即20ms周期
// 这里设置 BASIC_TIMx_PRESCALER=359；BASIC_TIMx_PERIOD=4000；

/* 扩展变量 ------------------------------------------------------------------*/
extern TIM_HandleTypeDef htimx;
/* 函数声明 ------------------------------------------------------------------*/

void BASIC_TIMx_Init(void);

#endif	/* __BASIC_TIM_H__ */
/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
