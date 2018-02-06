/******************************************************************************
   main.c
   This is a classic main function - initialization followed by an infinite
   loop. No code is executed in the main loop. It simply acts as a task
   dispatcher. Make sure all functions are non-blocking so that they all
   have a chance to run.
******************************************************************************/
#include <hidef.h>			/* common defines and macros */
#include <mc9s12e128.h>		/* derivative information */
//#include "App.h"
//#include "ATD.h"
#include "CPU.h"
//#include "DAC.h"
#include "IO.h"
#include "KBD.h"
#include "LCD.h"
//#include "PMF.h"
//#include "PWM.h"
#include "SCI0.h"
//#include "SCI1.h"
//#include "SPI.h"
#include "Timer.h"



// Global Variables
extern char WaitFlags;		// Defined in CPU.c



void main(void)
{
	InitPLL();		// Initialize the phase-locked loop clock circuits
	InitLED();		// Initialize the green diagnostic LED on the board
	InitIO();		// Initialize the input/output ports
	InitKBD();		// Initialize the keyboard sense circuits

	InitSCI0();		// Initialize serial channel 0
	//InitSCI1();		// Initialize serial channel 1
	//InitSPI();		// Initialize the SPI
	//InitPMF();		// Initialize the PMF
	//InitPWM();		// Initialize the boost pulse width modulator
	//InitATD();		// Initialize the analog to digital converter

	InitT0C4();		// Initialize millisecond timer

	EnableInterrupts;

	InitLCD();		// Initialize the liquid crystal display
	InitBT();		// Initialize Bluetooth
	//InitDAC0();		// Initialize the digital to analog circuit for OP-Amp offset
	//InitDAC1();		// Initialize the digital to analog circuit for LCD contrast

	init_timerpool();			// Initialize the timer pool


	SCI0_PutQueue("Started Solar Tracking Program\r\n");	// Opening messages
	SCI0_PutQueue("Version 1.0\r\n");

	SendMessage("Solar Tracker");	// Put a message on the LCD


	WaitFlags = 0xFF;			// Not used in this application


	//TIM0_TIE_C4I = 0;			// Test: Disable timer 0 channel 4 interrupts



	for(;;)
	{
		SCI0_Rx();				// Check for received characters on SCI0
		SCI0_CheckQueue();		// Check for outgoing messages for SCI0

		//SCI1_Rx();				// Check for received characters on SCI1
		//SCI1_CheckQueue();		// Check for outgoing messages for SCI1

		//GoToThread1();			// Do Thread 1 State Machine
		//GoToThread2();			// Do Thread 2 State Machine
		//GoToThread3();			// Do Thread 3 State Machine

		KeyScan();				// Scan the push puttons
		//MonitorDC();			// Monitor DC power
		//MonitorAC();			// Monitor AC power
		//MonitorBoostPWM();		// Increase/decrease boost PWM duty cycle
	}
}
