/******************************************************************************
  LCD.c
  Driver for the LCD module.
******************************************************************************/

/* Include Files */
#include <mc9s12e128.h>
#include <stdio.h>
#include "LCD.h"
#include "Datatypes.h"
#include "Buffers.h"
#include "SCI0.h"

/* Defines */
#define RS (PTAD_PTAD13)
#define RW (PTAD_PTAD14)
#define E  (PTAD_PTAD15)



/* Global Variables */
extern unsigned int msec;


/*****************************************************************************/
/* Initialize the LCD */
void InitLCD(void)
{
	int Timer;

	msec = 0;
	SendCommand(FunctionSet);
	Timer = msec + 40;
	while (Timer > msec){}
	SendCommand(FunctionSet);
	Timer = msec + 40;
	while (Timer > msec){}
	SendCommand(FunctionSet);
	Timer = msec + 40;
	while (Timer > msec){}
	SendCommand(FunctionSet);
	Timer = msec + 40;
	while (Timer > msec){}

	SendCommand(EntryModeSet);
	while (ReadBusy()){}
	SendCommand(ControlCommand);
	while (ReadBusy()){}
	SendCommand(ClearDisplay);
	while (ReadBusy()){}
}




/*****************************************************************************/
/* Read a byte from the LCD */
char ReadLCD(void)
{
	char ReadValue;

	DDRT = 0;		// Make Port T inputs
	RS   = 0;		// Clear RS
	RW   = 1;		// Activate read
	E    = 1;		// Activete strobe
	ReadValue = PTIT;	// Read data bus
	E    = 0;		// Deactivate strobe
	RW   = 0;		// Normal state of RW is write
	RS   = 0;		// Normal state of RS is 0
	PTT  = 0;		// Clear data bus
	DDRT = 0xFF;	// Make Port T outputs
	return ReadValue;
}

/*****************************************************************************/
/* Read the busy flag from the LCD. Return TRUE if busy. */
char ReadBusy(void)
{
	if (ReadLCD() & 0x80)
		return TRUE;
	else
		return FALSE;
}

/*****************************************************************************/
/* Send command to LCD */
void SendCommand(char Command)
{
	PTT = Command;	// Put command on the bus
	RS  = 0;		// Command mode
	RW  = 0;		// Write mode
	E   = 1;		// Stobe active
	E   = 0;		// Strobe inactive
	PTT = 0;		// Clear bus
}

/*****************************************************************************/
/* Send command to LCD and wait while busy bit is on */
void SendCommandBusy(char Command)
{
	SendCommand(Command);
	while (ReadBusy()){}
}

/*****************************************************************************/
/* Send data to LCD */
void SendData(char Data)
{
	PTT = Data;		// Put data on the bus
	RS  = 1;		// Data mode
	RW  = 0;		// Write mode
	E   = 1;		// Strobe active
	E   = 0;		// Strobe inactive
	RS  = 0;		// Command mode
	PTT = 0;		// Cear bus
}

/*****************************************************************************/
/* Send message to LCD */
void SendMessage(char *Address)
{
	int  MsgPtr;
	char MsgData;

	MsgPtr = 0;
	SendCommand(ClearDisplay);
	while (ReadBusy()){}


	MsgData = Address[MsgPtr++];

	while (MsgData > 0)
	{
		SendData(MsgData);
		while (ReadBusy()){}
		MsgData = Address[MsgPtr++];
	}
}

/*****************************************************************************/
/* Send message to LCD at cursor location */
void SendMsgAtCursor(char *Address)
{
	int MsgPtr;
	char MsgData;

	MsgPtr = 0;

	MsgData = Address[MsgPtr++];

	while (MsgData > 0)
	{
		SendData(MsgData);
		while (ReadBusy()){};
		MsgData = Address[MsgPtr++];
	}
}


