/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */     
#include "led/bsp_led.h"
#include "usart/bsp_debug_usart.h"

/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId defaultTaskHandle;

/* USER CODE BEGIN Variables */
extern TIM_HandleTypeDef htim3;
extern uint8_t uart2RcvOneData;
extern UART_HandleTypeDef huart2;
extern cmd_analyze_struct cmd_analyze;  
extern const cmd_list_struct cmd_list[CMD_NUM];

 TaskHandle_t xCmdAnalyzeHandle = NULL;
 TaskHandle_t redLEDTaskHandle = NULL;
 TaskHandle_t greenLEDTaskHandle = NULL;
 TaskHandle_t blueLEDTaskHandle = NULL;

void vTaskCmdAnalyze(void *pvParameters);
void redLEDTaskFunc(void *pvParameters);
void greenLEDTaskFunc(void *pvParameters);
void blueLEDTaskFunc(void *pvParameters);

/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void StartDefaultTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* Hook prototypes */
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);

/* USER CODE BEGIN 1 */
/* Functions needed when configGENERATE_RUN_TIME_STATS is on */
__weak void configureTimerForRunTimeStats(void)
{

}

__weak unsigned long getRunTimeCounterValue(void)
{
return 0;
}
/* USER CODE END 1 */

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  HAL_TIM_Base_Start_IT(&htim3);//启动TIM3定时器
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  xTaskCreate(vTaskCmdAnalyze,  "CmdAnalyzeTask",   512, NULL, 1, &xCmdAnalyzeHandle);
  xTaskCreate(redLEDTaskFunc,   "redLEDTask",       256, NULL, 2, &redLEDTaskHandle);
  xTaskCreate(greenLEDTaskFunc, "greenLEDTask",     256, NULL, 3, &greenLEDTaskHandle);
  xTaskCreate(blueLEDTaskFunc,  "blueLEDTask",      256, NULL, 4, &blueLEDTaskHandle);
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{

  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Application */
void vTaskCmdAnalyze( void *pvParameters )  
{  
    uint32_t i;  
    int32_t rec_arg_num;  
    char cmd_buf[CMD_LEN];        

    //开启串口中断
    HAL_UART_Receive_IT(&huart2, &uart2RcvOneData, 1);
    
    while(1)  
    {  
        uint32_t rec_num;  
        ulTaskNotifyTake(pdTRUE,portMAX_DELAY);  
        rec_num=get_true_char_stream(cmd_analyze.processed_buf,cmd_analyze.rec_buf);  
         
       /*从接收数据中提取命令*/  
       for(i=0;i<CMD_LEN;i++)  
       {  
           if((i>0)&&((cmd_analyze.processed_buf[i]==' ')||(cmd_analyze.processed_buf[i]==0x0D)))  
           {  
                cmd_buf[i]='\0';        //字符串结束符  
                break;  
           }  
           else  
           {  
                cmd_buf[i]=cmd_analyze.processed_buf[i];  
           }  
       }  
         
       rec_arg_num=cmd_arg_analyze(&cmd_analyze.processed_buf[i],rec_num);  
         
       for(i=0; i<(sizeof(cmd_list)/sizeof(cmd_list[0])); i++)  
       {  
           if(!strcmp(cmd_buf,cmd_list[i].cmd_name))       //字符串相等  
           {  
                if(rec_arg_num<0 || rec_arg_num>cmd_list[i].max_args)  
                {  
                    MY_DEBUGF(CMD_LINE_DEBUG,("参数数目过多!\n"));  
                }  
                else  
                {  
                    cmd_list[i].handle(rec_arg_num,(void *)cmd_analyze.cmd_arg);  
                }  
                break;  
           }  
             
       }  
       if( i >= ( sizeof(cmd_list) / sizeof(cmd_list[0]) ) )  
       {   
           MY_DEBUGF(CMD_LINE_DEBUG,("不支持的指令!\n"));  
       }  
    }  
}  


void redLEDTaskFunc(void *pvParameters)
{
    while(1)
    {
        LED_RED_TOGGLE;
        vTaskDelay(500);
    }
}
void greenLEDTaskFunc(void *pvParameters)
{
    while(1)
    {
        LED_GREEN_TOGGLE;
        vTaskDelay(750);
    }
}
void blueLEDTaskFunc(void *pvParameters)
{
    while(1)
    {
        LED_BLUE_TOGGLE;
        vTaskDelay(1000);
        //MY_DEBUGF(1,("debug 1 \r\n"));
        //MY_DEBUGF(0,("debug 0 \r\n"));
       // printf("hello TASK is running\r\n");
    }
}

     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
