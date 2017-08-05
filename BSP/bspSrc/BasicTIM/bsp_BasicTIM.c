/**
  ******************************************************************************
  * 文件名程: bsp_BasicTIM.c 
  * 作    者: kian
  * 版    本: V1.0
  * 编写日期: 2017-07-19
  * 功    能: 实现TIM的回调函数
  ******************************************************************************
  * 说明：
  ******************************************************************************
  */
/* 包含头文件 ----------------------------------------------------------------*/
#include "BasicTIM/bsp_BasicTIM.h" 

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/

/*
 * 被钩子函数 configureTimerForRunTimeStats() 和 getRunTimeCounterValue() 调用，
 * 为了在调试系统时候，用来计算任务运行时间，它的计数频率要比系统时基要快
 */
volatile uint32_t ulHighFrequencyTimerTicks; 

/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
extern TIM_HandleTypeDef htim3;


/**
  * 函数功能: 非阻塞模式下定时器的回调函数
  * 输入参数: htim：定时器句柄
  * 返 回 值: 无
  * 说    明: 无
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(__HAL_TIM_GET_IT_SOURCE(&htim3,TIM_IT_UPDATE)!=RESET)
  {
    ulHighFrequencyTimerTicks++;
    __HAL_TIM_CLEAR_IT(&htim3, TIM_IT_UPDATE);
  }
}


/* Functions needed when configGENERATE_RUN_TIME_STATS is on */
void configureTimerForRunTimeStats(void)
{
    ulHighFrequencyTimerTicks = 0ul;

}

unsigned long getRunTimeCounterValue(void)
{
    return ulHighFrequencyTimerTicks;
}

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
