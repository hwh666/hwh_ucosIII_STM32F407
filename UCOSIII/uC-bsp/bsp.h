/*
*********************************************************************************************************
*                                     MICIRUM BOARD SUPPORT PACKAGE
*
*                             (c) Copyright 2014; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                     BOARD SUPPORT PACKAGE (BSP)
*
*                                       IAR Development Kits
*                                              on the
*
*                                    STM32F429II-SK KICKSTART KIT
*
* Filename      : bsp.h
* Version       : V1.00
* Programmer(s) : FF
*                 DC
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                 MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               BSP present pre-processor macro definition.
*
*           (2) This file and its dependencies requires IAR v6.20 or later to be compiled.
*
*********************************************************************************************************
*/

#ifndef  BSP_PRESENT
#define  BSP_PRESENT


/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/

#ifdef   BSP_MODULE
#define  BSP_EXT
#else
#define  BSP_EXT  extern
#endif


/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/

#include  <stdio.h>
#include  <stdarg.h>

#include  <cpu.h>
#include  <cpu_core.h>

#include  <lib_def.h>
#include  <lib_ascii.h>

#include "ST_Include.h"

CPU_TS Get_Cycle_T(CPU_INT08U item)	;

enum
{
	NOW = 0,
	OLD,
	NEW,
};
enum
{
	Old=0,
	New,
	TimeCost,
};
enum 
{
	Task_LED=0,
	Task_Key,
	_2ms_Task,
	_5ms_Task,
	_10ms_Task,
	_20ms_Task,
	_50ms_Task,	
/*****ADD*****/    
 
/*************/    
    TimeTestGroup,
};

typedef struct
{
	CPU_TS LED_Task_T[2];
	CPU_TS Key_Task_T[2];
	CPU_TS _2ms_Task_T[2];
	CPU_TS _5ms_Task_T[2];
	CPU_TS _10ms_Task_T[2];
	CPU_TS _20ms_Task_T[2];
	CPU_TS _50ms_Task_T[2];
	//CPU_TS _all[2];
}_TimeMeasure_t;//每个循环执行时间结构体
typedef struct
{
	CPU_TS Cycle_T;
	CPU_TS Cycle_T_Max;
	CPU_TS Task_T;
	CPU_TS Task_T_Max;
}_Time_Mark;//每个循环执行时间结构体
typedef struct
{
	_Time_Mark LED_Task_Time;
	_Time_Mark Key_Task_Time;
	_Time_Mark _2ms_Task_Time;
	_Time_Mark _5ms_Task_Time;
	_Time_Mark _10ms_Task_Time;
	_Time_Mark _20ms_Task_Time;
	_Time_Mark _50ms_Task_Time;
}_Time_Cost;//每个循环执行时间结构体

extern _Time_Cost     Time_Cost;
extern _TimeMeasure_t TimeMeasure_t;

/*
*********************************************************************************************************
*                                               CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             PERIPH DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                                 MACRO'S
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               INT DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             PERIPH DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void        BSP_Init                          (void);

CPU_INT32U  BSP_CPU_ClkFreq                   (void);

void        BSP_Tick_Init                     (void);

/*
*********************************************************************************************************
*                                           INTERRUPT SERVICES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                     PERIPHERAL POWER/CLOCK SERVICES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                              LED SERVICES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/


#endif                                                          /* End of module include.                               */

