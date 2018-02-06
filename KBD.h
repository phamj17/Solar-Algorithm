/******************************************************************************
  KBD.h
  Driver for the LCD module.
******************************************************************************/

#ifndef KBD_H
#define KBD_H

#include "Datatypes.h"


/* Defines */
#define DebounceTime (100)	// Debounce delay in milliseconds

#define StopKey  1
#define StartKey 2
#define ResetKey 3

#define SWfifosize 8

typedef struct sw_fifo_struct
{
	UINT8 In;
	UINT8 Out;
	UINT8 Data[SWfifosize];
}sw_fifo_struct;



/* Function Prototypes */
void InitKBD(void);
UINT8 ReadKBD(void);
void SetIdleLED(void);
void ClearIdleLED(void);
void SetRunLED(void);
void ClearRunLED(void);
void SetErrorLED(void);
void ClearErrorLED(void);

void SW_FIFO_Init(void);
void SW_FIFO_EnterData(UINT8 Data);
UINT8 SW_FIFO_ReadData(void);

void KeyScan(void);


#endif