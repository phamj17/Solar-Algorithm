/******************************************************************************
   SCI0.c
   Interrupt driven SCI routines.
******************************************************************************/

// Include Files
#include <MC9S12E128.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//#include "App.h"
#include "Buffers.h"
//#include "ATD.h"
//#include "CPU.h"
//#include "DAC.h"
#include "IO.h"
//#include "KBD.h"
#include "LCD.h"
//#include "PMF.h"
//#include "PWM.h"
#include "SCI0.h"
//#include "SCI1.h"
//#include "SPI.h"
#include "Timer.h"
#include "Datatypes.h"
#include "spa_tester.h"



// Global Variables
char *TxMsg0;			// Pointer to the current message being transmitted
word MsgOutPtr0;		// Pointer to the current message byte
rx_q_struct rxbuf0;		// Receive buffer and input/output pointers
tx_q_struct txqueue0;	// Transmit buffer queue and input/output pointers

char RcvBuf0[RcvBufSize0];	// Receive message buffer
word RcvBufPtr0;			// Pointer tocurrent byte in receive buffer

// The receive message routine has 2 states
static tPointerFcn SCI0_RxFuncPtr = SCI0_Rx1;	// Start in State 1

extern char WaitFlags;	// Not used in this application

//extern BOOL  Phase;
//extern BOOL  Slope;
//extern word  Angle;
//extern int   Adj;
//extern byte  Amp;

//extern UINT8 LED_State;				// The state of LED lights in App.c
//extern BOOL InputVoltageGood;		// TRUE when input voltage is above minimum
//extern BOOL AC_VoltagePresent;		// TRUE when AC voltage is detected
//extern BOOL ErrorDetected;			// TRUE when an error condition was detected
//extern word AC_MidPoint;			// ATD value for zero crossing detect

//extern word ATD_Result2;			// Input Current Sense Micro
//extern word ATD_Result3;			// Input Voltage Sense Micro


/******************************************************************************
  This is the SCI0 Interrupt Handler. */
#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt void SCI0_Int(void)
{
	char data;	// Temporary data storage

	if(SCI0SR1_RDRF)	// If received data flag is on
	{
		data = SCI0DRL;  // transfer SCI data reg to temporary storage
		if (data == DC3)  // If data is XOFF
			SCI0CR2_SCTIE = 0;  // suppress transmitter interrupts
		else if (data == DC1)  // If data is XON
		{
			/*
			SCI0CR2_SCTIE = 1;  // allow transmitter interrupts
			SCI0CR2_TCIE = 1;  // and transmission complete interrupts
			*/
			SCI0CR2 |= SCI0CR2_SCTIE_MASK + SCI0CR2_TCIE_MASK;
		}
		else
		{
			rxbuf0.q[rxbuf0.in++] = data;  // Enter character and increment pointer
			if (rxbuf0.in > (rxqsize0-1))  // If exceeded buffer length
				rxbuf0.in = 0;  // set receive buffer in pointer to start of buffer
		}

	}
	else if (!SCI0CR2_SCTIE)  // If transmitter interrupts are already disabled
	{
		SCI0CR2_TCIE = 0;  // suppress transmission complete interrupts
		SCI0CR2_TE = 0;  // and disable SCI transmitter
		WaitFlags |= 0x02;
	}
	else if (SCI0SR1_TDRE)  // If transmitter data register is empty
	{
		data = TxMsg0[MsgOutPtr0++];  // get a byte from buffer and increment pointer
		if (data == NUL)  // If data is NUL we are done with this message so
			SCI0CR2_SCTIE = 0;  // suppress transmitter interrupts
		else
			SCI0DRL = data;  // Otherwise send the data byte
	}
}
#pragma CODE_SEG DEFAULT


/******************************************************************************
  Initialize SCI0 */
void InitSCI0(void)
{
	SCI0_SetBaud();			// Set SCI0 baud rate
	SCI0_StartRx();			// Start SCI0 receiver
	SCI0_TxQueueInit();		// Initialize transmit queue pointers
}

/******************************************************************************
  This routine sets SCI0 baud rate. */
void SCI0_SetBaud(void)
{
	SCI0BD  = SCI0_BAUD_DIV;	// Set baud rate register
	SCI0CR1 = 0;
}

/******************************************************************************
  This routine initializes the receive buffer in and out pointers. */
void SCI0_RxQueueInit(void)
{
	rxbuf0.in = 0;
	rxbuf0.out = 0;
}

/******************************************************************************
  This routine starts the SCI0 receiver. */
void SCI0_StartRx(void)
{
	SCI0_RxQueueInit();
/*
	SCI0CR2_RIE = 1;	// Enable receiver interrupts
	SCI0CR2_RE = 1;		// Enable receiver
*/
	SCI0CR2 |= SCI0CR2_RIE_MASK + SCI0CR2_RE_MASK;
}

/******************************************************************************
  This routine stops the SCI0 receiver to conserve power */
void SCI0_StopRx(void)
{
/*
	SCI0CR2_RIE = 0;	// Disable receiver interrupts
	SCI0CR2_RE = 0;		// Disable receiver
*/
	SCI0CR2 &= ~(SCI0CR2_RIE_MASK + SCI0CR2_RE_MASK);
}

/******************************************************************************
  This routine initializes the transmit buffer queue in and out pointers */
void SCI0_TxQueueInit(void)
{
	txqueue0.in = 0;
	txqueue0.out = 0;
}

/******************************************************************************
  This routine makes an entry into the transmit queue of pointers and updates
  the in pointer to the next available location. */
void SCI0_PutQueue(char *msg)
{
	txqueue0.q[txqueue0.in++] = msg;
	if (txqueue0.in > (txqsize0-1))
		txqueue0.in = 0;
}

/******************************************************************************
  This routine starts the transmitter */
void SCI0_StartTx(void)
{
/*
	SCI0CR2_SCTIE = 1;	// Enable transmitter interrupts
	SCI0CR2_TCIE = 1;	// Enable transmission complete interrupts
	SCI0CR2_TE = 1;		// Enable the SCI0 transmitter
*/
	SCI0CR2 |= SCI0CR2_SCTIE_MASK + SCI0CR2_TCIE_MASK + SCI0CR2_TE_MASK;
}

/******************************************************************************
  This routine retrieves a byte from the receive buffer if present. Returns 0
  if no data available. Returns 1 if a data byte has been transferred
  successfully. The pointer to the data byte is passed to the routine. */
byte SCI0_GetChar(char *rxbyte)
{
	if (rxbuf0.in != rxbuf0.out)
	{
		*rxbyte = rxbuf0.q[rxbuf0.out++];	// Read character and update pointer
		if (rxbuf0.out > (rxqsize0-1))		// If exceeded buffer length
			rxbuf0.out = 0;		// set receive buffer out pointer to start of buffer
		return 1;
	}
	else
	{
		return 0;
	}
}

/******************************************************************************
  This routine enters a pointer to a text message and starts the SCI0
  transmitter. */
void SCI0_SendMsg(char *s)
{
	TxMsg0 = s;			// Pointer to the message for the SCI0 interrupt handler
	MsgOutPtr0 = 0;		// Point to start of text message
	SCI0_StartTx();		// Turn on SCI0 transmitter
}

/******************************************************************************
  This routine checks the transmit buffer queue and if it is not empty,
  it extracts the pointer to the message and sets up the SCI0 transmit
  interrupt handler. */

/* Call this function in main loop */
void SCI0_CheckQueue(void)
{
	if (!SCI0CR2_TE)	// Don't try to start transmitter if already transmitting
		if (txqueue0.in != txqueue0.out)	// Look for a pointer in the queue and
		{
			SCI0_SendMsg(txqueue0.q[txqueue0.out++]);	// hand it off. Update pointer
			if (txqueue0.out > (txqsize0-1))	// If past the end of array
				txqueue0.out = 0;	// set pointer to start of array
			WaitFlags &= ~0x02;
		}
}



/******************************************************************************
   Local Subroutine */
void InsertCRLF(void)
{
	//ptr = strcat(MsgBuf,"\r\n");
}
/******************************************************************************
   Local Subroutine */
void DisplayResults(void)
{

}

/******************************************************************************
   SCI0_Rx State Machine */

// Call this function in main loop
void SCI0_Rx(void)
{
	SCI0_RxFuncPtr();	// Use the function pointer to go to a state
}

/******************************************************************************
   SCI0 State 1: Accumulate received characters in a buffer until a Line Feed
   character is received then insert a NUL character into the buffer and go to
   State 2.*/
void SCI0_Rx1(void)
{
	char rxchar;

	if (SCI0_GetChar(&rxchar))
	{
		switch (rxchar)
		{
			case '\n':
				RcvBuf0[RcvBufPtr0++] = rxchar;
				RcvBuf0[RcvBufPtr0++] = 0;
				RcvBufPtr0 = 0;
				SCI0_RxFuncPtr = SCI0_Rx2;
				break;

			default:
				if (RcvBufPtr0 < (RcvBufSize0-2))
					RcvBuf0[RcvBufPtr0++] = rxchar;
				break;
		}
	}
}

/******************************************************************************
   SCI0 State 2: Execute the received command and go to State 1*/
void SCI0_Rx2(void)
{
    //char* BufNum;

	switch (RcvBuf0[0])
	{

		case 'a':		// Change to suit application
			SCI0_PutQueue("Command a received\r\n");
			Toggle_LED();
			break;
		case 'b':
			SendCommand(ClearDisplay);
			SendMessage("Testing LCD");
			break;
		case 'c':
			SPA_Test();
			break;
		case 'd':

			break;
		case 'e':
			//PTP_PTP0 = ~(PTP_PTP0);		// Toggle LED
			Toggle_LED();
			SCI0_PutQueue("ACK\r\n");	// 'ACK'
			break;






		default:
			SCI0_PutQueue("Unknown command\r\n");
			break;



	}
	SCI0_RxFuncPtr = SCI0_Rx1;
}


