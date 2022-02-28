 /******************************************************************************
 *
 * Module: Sw_Delay
 *
 * File Name: Sw_Delay.h
 *
 * Description: Header file for Sw_Delay
 *
 * Author: Youssef El-Nemaky
 ******************************************************************************/

#ifndef SW_DELAY_H
#define SW_DLEAY_H

#define NUMBER_OF_ITERATIONS_PER_ONE_MILI_SECOND 762

/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/

/*******************************************************************************************************
 * [Name]: Delay_ms
 * [Parameters]: uint64 n
 * [Return]: void (none)
 * [Description]: The Function responsible for The Delay in the internal Operations of the peripherals
 * and Hardware Components that don't need accurate timing using Timers like (Systic or GPT).
 ********************************************************************************************************/
void Delay_Ms(unsigned long long timeInMs);

#endif /* SW_DELAY_H*/