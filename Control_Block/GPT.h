 /******************************************************************************
 *
 * Module: GPT
 *
 * File Name: GPT.h
 *
 * Description: Header File for GPT 
 *
 * Created on: June 11, 2022
 *
 * Authors: Hazem Yousery
 *
 *******************************************************************************/
#ifndef GPT_H_
#define GPT_H_

#include "Std_Types.h"

#define GPTMTAPR   (*((volatile unsigned long *)0x40031038))
#define RCGCTIMER (*((volatile unsigned long*)0x400FE604))
#define GPTMCTL (*((volatile unsigned long*)0x4003100C))
#define GPTMCFG (*((volatile unsigned long*)0x40031000))
#define GPTMTAMR (*((volatile unsigned long*)0x40031004))
#define GPTMTAILR (*((volatile unsigned long*)0x40031028))
#define GPTMRIS (*((volatile unsigned long*)0x4003101C))
#define GPTMICR (*((volatile unsigned long*)0x40031024))
#define GPTMIMR (*((volatile unsigned long*)0x40031018))
#define GPTMMIS (*((volatile unsigned long*)0x40031020))
#define EN0 (*((volatile unsigned long*)0xE000E100))

void Timer1_Init();
void Timer1_Start();
void Timer1_Handler();
void Timer1_setCallBack(void (*ptr)());
void Timer1_Stop();

#endif /* TIMER_H_ */
