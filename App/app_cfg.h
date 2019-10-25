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


extern RCC_ClocksTypeDef get_rcc_clock;    //��ȡϵͳʱ��״̬

#define TimeMeasurement                     //�����ص�����ʱ���������
/*
*********************************************************************************************************
*                                       MODULE ENABLE / DISABLE
*********************************************************************************************************
*/

#define  APP_CFG_SERIAL_EN                          DEF_DISABLED          // Modified by fire ��ԭ�� DEF_ENABLED��
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


#define  APP_TASK_START_PRIO                        10            //�����������ȼ�
#define  LED_TASK_PRIO                              15            //LED�������ȼ�   
#define  Key_Pend_TASK_PRIO                         3            //���������������ȼ�
#define  Key_Post_TASK_PRIO                         3            //���������������ȼ�
#define  Task_Tmr_TASK_PRIO                         4            //Systick�����ʱ���������ȼ�
#define  TASK_PRIO_2ms                              1            //2ms�������ȼ�
#define  TASK_PRIO_5ms                              4            //5ms�������ȼ�
#define  TASK_PRIO_10ms                             3            //10ms�������ȼ�
#define  TASK_PRIO_20ms                             2            //20ms�������ȼ�
#define  TASK_PRIO_50ms                             15            //50ms�������ȼ�
/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*                             Size of the task stacks (# of OS_STK entries)
*********************************************************************************************************
*/

#define  APP_TASK_START_STK_SIZE                    128          //���������ջ�ռ䣨��λ��sizeof(CPU_STK)��
#define  LED_TASK_STK_SIZE                          128          //LED�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
#define  Key_Post_TASK_STK_SIZE                     128          //Key���������ջ�ռ䣨��λ��sizeof(CPU_STK)��
#define  Key_Pend_TASK_STK_SIZE                     128          //Key���������ջ�ռ䣨��λ��sizeof(CPU_STK)��
#define  Task_Tmr_STK_SIZE                          128          //Systick�����ʱ�������ջ�ռ䣨��λ��sizeof(CPU_STK)��
#define  TASK_STK_SIZE_2ms                          128          //2ms�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
#define  TASK_STK_SIZE_5ms                          128          //5ms�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
#define  TASK_STK_SIZE_10ms                         128          //10ms�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
#define  TASK_STK_SIZE_20ms                         128          //20ms�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
#define  TASK_STK_SIZE_50ms                         128          //50ms�����ջ�ռ䣨��λ��sizeof(CPU_STK)��

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




