/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                           (c) Copyright 2009-2013; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                      APPLICATION CONFIGURATION
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                     Micrium uC-Eval-STM32F107
*                                         Evaluation Board
*
* Filename      : app_cfg.h
* Version       : V1.00
* Programmer(s) : JJL
*                 EHS
*                 DC
*********************************************************************************************************
*/

#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__


#include "led.h"
#include "Key.h"
#include "beep.h"
#include "bsp.h"
#include "Task_Scheduler.h"


extern RCC_ClocksTypeDef get_rcc_clock;    //获取系统时钟状态

#define TimeMeasurement                     //开启回调函数时间测量功能
/*
*********************************************************************************************************
*                                       MODULE ENABLE / DISABLE
*********************************************************************************************************
*/

#define  APP_CFG_SERIAL_EN                          DEF_DISABLED          // Modified by fire （原是 DEF_ENABLED）
#define  Task_2ms_EN                                1    
#define  Task_5ms_EN                                1 
#define  Task_10ms_EN                               1 
#define  Task_20ms_EN                               1 
#define  Task_50ms_EN                               1 
/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************
*/


#define  APP_TASK_START_PRIO                        10            //启动任务优先级
#define  LED_TASK_PRIO                              15            //LED任务优先级   
#define  Key_Pend_TASK_PRIO                         3            //按键发送任务优先级
#define  Key_Post_TASK_PRIO                         3            //按键接收任务优先级
#define  Task_Tmr_TASK_PRIO                         4            //Systick软件定时器任务优先级
#define  TASK_PRIO_2ms                              1            //2ms任务优先级
#define  TASK_PRIO_5ms                              4            //5ms任务优先级
#define  TASK_PRIO_10ms                             3            //10ms任务优先级
#define  TASK_PRIO_20ms                             2            //20ms任务优先级
#define  TASK_PRIO_50ms                             15            //50ms任务优先级
/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*                             Size of the task stacks (# of OS_STK entries)
*********************************************************************************************************
*/

#define  APP_TASK_START_STK_SIZE                    128          //启动任务堆栈空间（单位：sizeof(CPU_STK)）
#define  LED_TASK_STK_SIZE                          128          //LED任务堆栈空间（单位：sizeof(CPU_STK)）
#define  Key_Post_TASK_STK_SIZE                     128          //Key发送任务堆栈空间（单位：sizeof(CPU_STK)）
#define  Key_Pend_TASK_STK_SIZE                     128          //Key接收任务堆栈空间（单位：sizeof(CPU_STK)）
#define  Task_Tmr_STK_SIZE                          128          //Systick软件定时器任务堆栈空间（单位：sizeof(CPU_STK)）
#define  TASK_STK_SIZE_2ms                          128          //2ms任务堆栈空间（单位：sizeof(CPU_STK)）
#define  TASK_STK_SIZE_5ms                          128          //5ms任务堆栈空间（单位：sizeof(CPU_STK)）
#define  TASK_STK_SIZE_10ms                         128          //10ms任务堆栈空间（单位：sizeof(CPU_STK)）
#define  TASK_STK_SIZE_20ms                         128          //20ms任务堆栈空间（单位：sizeof(CPU_STK)）
#define  TASK_STK_SIZE_50ms                         128          //50ms任务堆栈空间（单位：sizeof(CPU_STK)）

/*
*********************************************************************************************************
*                                    BSP CONFIGURATION: RS-232
*********************************************************************************************************
*/

#define  BSP_CFG_SER_COMM_SEL             			BSP_SER_COMM_UART_02
#define  BSP_CFG_TS_TMR_SEL                         2


/*
*********************************************************************************************************
*                                     TRACE / DEBUG CONFIGURATION
*********************************************************************************************************
*/
#if 0
#define  TRACE_LEVEL_OFF                            0
#define  TRACE_LEVEL_INFO                           1
#define  TRACE_LEVEL_DEBUG                          2
#endif

#define  APP_TRACE_LEVEL                            TRACE_LEVEL_INFO
#define  APP_TRACE                                  BSP_Ser_Printf

#define  APP_TRACE_INFO(x)            ((APP_TRACE_LEVEL >= TRACE_LEVEL_INFO)  ? (void)(APP_TRACE x) : (void)0)
#define  APP_TRACE_DEBUG(x)           ((APP_TRACE_LEVEL >= TRACE_LEVEL_DEBUG) ? (void)(APP_TRACE x) : (void)0)


#endif




