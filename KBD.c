/******************************************************************************
  KBD.c
  Driver for the keyboard wake-up circuits
******************************************************************************/

/* Include Files */
#include <mc9s12e128.h>
#include "KBD.h"
#include "Datatypes.h"
#include "SCI0.h"
#include "Timer.h"

/* Defines */


/* Global Variables */
UINT8 KeyScanState = 0;		// Scan state
UINT8 KeyTimerHandle;		// The handle to the timer
sw_fifo_struct SW_FIFO;		// Fifo for storing push button events



/******************************************************************************
  This is the KBD Interrupt Handler. */
#pragma CODE_SEG NON_BANKED
interrupt void KBD_Int(void)
{
	PIEAD &= ~(0					// Disable all key interrupts
		| PIEAD_PIEAD8_MASK			// Stop key interrupt disabled
		| PIEAD_PIEAD9_MASK			// Start key interrupt disabled
		| PIEAD_PIEAD10_MASK		// Reset key interrupt disabled
		);
}
#pragma CODE_SEG DEFAULT

/*****************************************************************************/
/* Initialize the KBD */
void InitKBD(void)
{
	PIFAD = (0							// Clear all pending interrupts
				| PIEAD_PIEAD0_MASK
				| PIEAD_PIEAD1_MASK
				| PIEAD_PIEAD2_MASK
				| PIEAD_PIEAD3_MASK
				| PIEAD_PIEAD4_MASK
				| PIEAD_PIEAD5_MASK
				| PIEAD_PIEAD6_MASK
				| PIEAD_PIEAD7_MASK
				| PIEAD_PIEAD8_MASK
				| PIEAD_PIEAD9_MASK
				| PIEAD_PIEAD10_MASK
				| PIEAD_PIEAD11_MASK
				| PIEAD_PIEAD12_MASK
				| PIEAD_PIEAD13_MASK
				| PIEAD_PIEAD14_MASK
				| PIEAD_PIEAD15_MASK
				);

	PIEAD = (0
				/*| PIEAD_PIEAD0_MASK*/
				/*| PIEAD_PIEAD1_MASK*/
				/*| PIEAD_PIEAD2_MASK*/
				/*| PIEAD_PIEAD3_MASK*/
				/*| PIEAD_PIEAD4_MASK*/
				/*| PIEAD_PIEAD5_MASK*/
				/*| PIEAD_PIEAD6_MASK*/
				/*| PIEAD_PIEAD7_MASK*/
				| PIEAD_PIEAD8_MASK			// Stop key interrupt enabled
				| PIEAD_PIEAD9_MASK			// Start key interrupt enabled
				| PIEAD_PIEAD10_MASK		// Reset key interrupt enabled
				/*| PIEAD_PIEAD11_MASK*/	// Spare key interrupt disabled
				/*| PIEAD_PIEAD12_MASK*/
				/*| PIEAD_PIEAD13_MASK*/
				/*| PIEAD_PIEAD14_MASK*/
				/*| PIEAD_PIEAD15_MASK*/
				);

	SW_FIFO_Init();		// Initialize switch FIFO pointers
}

/*****************************************************************************/
/* Read the KBD port */
UINT8 ReadKBD(void)
{
	UINT8 KBDPort;

	KBDPort = (PTIADHi) & (PTIADHi_PTIAD8_MASK | PTIADHi_PTIAD9_MASK | PTIADHi_PTIAD10_MASK);
	return (KBDPort);
}

/*****************************************************************************/
/* Set Idle LED */
void SetIdleLED(void)
{
	PTM_PTM4 = 1;
}

/*****************************************************************************/
/* Clear Idle LED */
void ClearIdleLED(void)
{
	PTM_PTM4 = 0;
}

/*****************************************************************************/
/* Set Run LED */
void SetRunLED(void)
{
	PTM_PTM5 = 1;
}

/*****************************************************************************/
/* Clear Run LED */
void ClearRunLED(void)
{
	PTM_PTM5 = 0;
}

/*****************************************************************************/
/* Set Error LED */
void SetErrorLED(void)
{
	PTM_PTM6 = 1;
}

/*****************************************************************************/
/* Clear Error LED */
void ClearErrorLED(void)
{
	PTM_PTM6 = 0;
}


/******************************************************************************
   Initialize Switch FIFO */
void SW_FIFO_Init(void)
{
	SW_FIFO.In = SW_FIFO.Out = 0;
}

/******************************************************************************
   Enter data into Switch FIFO */
void SW_FIFO_EnterData(UINT8 Data)
{
	SW_FIFO.Data[SW_FIFO.In++] = Data;		// Enter data into fifo
	if (SW_FIFO.In > (SWfifosize-1))		// Check for fifo wrap
		SW_FIFO.In = 0;
}

/******************************************************************************
   Read data from Switch FIFO */
UINT8 SW_FIFO_ReadData(void)
{
	UINT8 Data;

	if (SW_FIFO.In != SW_FIFO.Out)	// If pointers are different, there is data
	{
		Data = SW_FIFO.Data[SW_FIFO.Out++];		// Get the next byte fron fifo
		if (SW_FIFO.Out > (SWfifosize-1))		// Check for fifo wrap
			SW_FIFO.Out = 0;
		return Data;
	} else
	{
		return (0);		// No new push button data available
	}
}

/*****************************************************************************/
/* The key scanning routine
   Call this function in main loop. */
void KeyScan(void)
{
	UINT8 KeyRead;

	switch (KeyScanState)
	{
		case (0):
			if (!PIEAD_PIEAD8)	// Any key press clears all KBD interrupt bits
				KeyScanState = 1;	// Go to next state
				break;

		case (1):
			 KeyTimerHandle = get_timer();	// Get a timer handle
			 init_timer(KeyTimerHandle, DebounceTime);	// Initialize timer
			 KeyScanState = 2;	// Go to next state
			 break;

		case (2):
			if (check_timer(KeyTimerHandle))	// Check timer
				KeyScanState = 2;	// Timer has not expired, stay in this state
			else
				KeyScanState = 3;	// Timer has expired, go to next state
			break;

		case (3):
			KeyRead = ReadKBD();	// Read the switch inputs
			if (!(KeyRead & 0x01))
			{
				SCI0_PutQueue("Stop key\r\n");
				SW_FIFO_EnterData(StopKey);		// Enter Stop key into FIFO
			}
			if (!(KeyRead & 0x02))
			{
				SCI0_PutQueue("Start key\r\n");
				SW_FIFO_EnterData(StartKey);	// Enter Start key into FIFO
			}
			if (!(KeyRead & 0x04))
			{
				SCI0_PutQueue("Reset key\r\n");
				SW_FIFO_EnterData(ResetKey);	// Enter Reset key into FIFO
			}
			KeyScanState = 4;	// Go to next state
			break;

		case (4):
			InitKBD();			// Enable switch interrupts
			free_timer(KeyTimerHandle);		// Release timer to pool
			KeyScanState = 0;	// Go back to the top
			break;
	}
}


