 /******************************************************************************
 *
 * Module: GPT
 *
 * File Name: GPT.c
 *
 * Description: Source File for GPT 
 *
 * Created on: June 11, 2022
 *
 * Authors: Hazem Yousery
 *
 *******************************************************************************/
#include "GPT.h"

static volatile void (*Timer1_CallBackPtr)(void) = NULL_PTR;
static volatile unsigned char timer1Counter = 0;


void Timer1_Init()
{

    volatile unsigned long delay = 0;
    RCGCTIMER |= (1<<1);
    delay = RCGCTIMER;
   
    GPTMCTL = 0;

    GPTMCFG=0x00; /* 32 bit timer configuration */

	GPTMTAMR = 0x02; /* Periodic mode - count down */

    GPTMTAILR = 480000000 - 1; /* A timer match value of 1 minute */

    GPTMIMR|=(1<<0); /* GPTM Timer A Time Out Event Interrupt Mask (Interrupt is enabled in this mode) */

    GPTMICR |= (1<<0); /* Clear interrupt flag */

    GPTMCTL |= (1<<0); /* GPTM Timer A Enable to start the timer*/

}

void Timer1_Start()
{
	GPTMCTL|=(1<<0); /* GPTM Timer A Enable to start the timer*/

}


void Timer1_Stop()
{
	GPTMCTL&=(~(1<<0)); /* GPTM Timer A Disable to stop the timer */
}

void Timer1_Handler()
{       
    timer1Counter++; /* Increment the counter value */
    if(timer1Counter == 1){ /* Check if 5 minutes have passed */
        if (Timer1_CallBackPtr == NULL_PTR)
        {
            /*do nothing*/
        }
        else
        {
            (*Timer1_CallBackPtr)(); /* Call back the function */
        }
        timer1Counter = 0;
    }
    GPTMICR |= (1<<0); /* Clear the interrupt flag bit */
        
}

void Timer1_setCallBack(void (*ptr)())
{
   Timer1_CallBackPtr = ptr; /* Set the call back function ptr */
}
