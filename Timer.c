/******************************************************************************
   Timer.c
   The timer routines.
******************************************************************************/
#include <MC9S12E128.h>		// Derivative information
#include "Timer.h"
#include "SCI0.h"
#include "IO.h"
#include "Datatypes.h"


// Global variables
unsigned int msec;    // Increments every millisecond

struct timers TimerPool[NUMTIMERS];




/******************************************************************************
   Timer0 Cannel 4 interrupt handler
   This timer is used to increment the millisecond variable */
#pragma CODE_SEG NON_BANKED
interrupt void T0C4_Int(void)
{
	msec++;				// Increment millisecond timer
	irqhandler_timer();

	TIM0_TC4 += 24000;	// Output compare to previous compare + 1ms
	TIM0_TFLG1_C4F = 1;	// Clear timer 0 channel 4 flag
}
#pragma CODE_SEG DEFAULT

/******************************************************************************
   Timer0 Channel 4 initialization */
void InitT0C4(void)
{
	TIM0_TSCR1 = TIM0_TSCR1_TEN_MASK;	// Enable timer 0
	TIM0_TSCR2 = 0;						// Set prescaler to divide by 1
	TIM0_TIOS |= TIM0_TIOS_IOS4_MASK;	// Channel 4 is output compare
	TIM0_TCTL1 = 0;						// All timers disconnected from IO pins
	TIM0_TC4 = TIM0_TCNT + 24000;		// Output compare to current time + 1ms
	TIM0_TFLG1_C4F = 1;					// Clear timer 0 channel 4 flag
	TIM0_TIE_C4I = 1;					// Enable timer 0 channel 4 interrupts
}




/******************************************************************************
Function:   init_timerpool
Parameters: none
Return val: void
Desc:       Initialize timer pool to 0 and free
*******************************************************************************/

void init_timerpool(void)
{
	UINT8 i;

	for (i=0; i < NUMTIMERS; i++)
	{
		TimerPool[i].TimerField = 0;
		TimerPool[i].Free = TRUE;
	}
}

/******************************************************************************
Function:   get_timer
Parameters: none
Return val: Return handle of free timer
Desc:       Gets free timer from the timer pool
*******************************************************************************/

UINT8 get_timer(void)
{
	UINT8 i;
	UINT8 FirstMatch;

	for (i=0; i < NUMTIMERS; i++)
	{
		if (TimerPool[i].Free == TRUE)
		{
		/* We found a free timer. Mark it reserved */
		TimerPool[i].Free = FALSE;
		FirstMatch = i;
		return FirstMatch;  /* Return timer handle  */
		}
	}
	/* Error Check */
	SCI0_PutQueue("TIMERS: No Timers, Resetting..\n\r");
	//RESET_SYSTEM();
	return (0);
}

/******************************************************************************
Function:   free_timer
Parameters: none
Return val: UINT8 nbr - timer handle
Desc:       Puts the given timer back to the timer pool
*******************************************************************************/

void free_timer(UINT8 nbr)
{
	/* Make a simple check */
	if (nbr > (NUMTIMERS-1))
		return;

	TimerPool[nbr].Free = TRUE;
}

/******************************************************************************
Function:   init_timer
Parameters: UINT8 nbr - timer handle
            UINT16 tout -  time out value in milliseconds
Return val: none
Desc:       Initializes given timer to given time-out
*******************************************************************************/

void init_timer(UINT8 nbr, UINT16 tout)
{
	/* Make a simple check for a valid timer handle */
	if (nbr > (NUMTIMERS-1))
		return;

	if (TimerPool[nbr].Free == TRUE)
		return;

	/* Timer handle is valid */
	TimerPool[nbr].TimerField = tout;
}

/******************************************************************************
Function:   check_timer
Parameters: UINT8 nbr - timer handle
Return val: return the value of given timer
Desc:       Return the value of given timer
*******************************************************************************/

UINT16 check_timer(UINT8 nbr)
{
	return TimerPool[nbr].TimerField;
}

/******************************************************************************
Function:   irqhandler_timer
Parameters: void
Return val: void
Desc:       Call this function on timer interrupt
*******************************************************************************/

void irqhandler_timer(void)
{
	UINT16 i;

	for (i=0; i<NUMTIMERS; i++)
	{
		if (TimerPool[i].Free == FALSE && TimerPool[i].TimerField != 0)
			TimerPool[i].TimerField--;
	}
}


