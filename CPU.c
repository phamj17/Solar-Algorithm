/******************************************************************************
   CPU.c

******************************************************************************/

// Include Files
#include <MC9S12E128.h>
#include "CPU.h"

// Global Variables
char WaitFlags;		// Each thread has its own wait flag (not used)
// Bit0:
// Bit1: SCI0 transmitter
// Bit2: SCI1 transmitter 
// Bit3:
// Bit4:
// Bit5:
// Bit6:
// Bit7:



/******************************************************************************
  Clock Initialization
  Oscillator Frequency: 8MHz
  Bus Clock Frequency: 24MHz */
void InitPLL(void)			// Initialize the PLL
{
	CLKSEL = 0;				// Select clock source from XTAL
	PLLCTL_PLLON = 0;		// Disable the PLL
	SYNR = 2;				// Set the multiplier register
	REFDV = 0;				// Set the divider register
	PLLCTL_PLLON = 1;		// Enable the PLL
	while(!CRGFLG_LOCK);	// Wait
	CLKSEL_PLLSEL = 1;		// Select clock source from PLL

	CRGFLG_RTIF = 1;		// Clear Real Time Interrupt flag
	CRGFLG_PORF = 1;		// Clear Power On Reset flag
	CRGFLG_LVRF = 1;		// Clear Low Voltage Reset flag
	CRGFLG_LOCKIF = 1;		// Clear PLL Lock Interrupt flag
	CRGFLG_SCMIF = 1;		// Clear Self Clock Mode Interrupt flag

	CRGINT_RTIE = 0;		// Disable Real Time Interrupts
	CRGINT_LOCKIE = 1;		// Enable Lock Interrupts

	INTCR_IRQEN = 0;		// Disable the IRQ interrupt
}


