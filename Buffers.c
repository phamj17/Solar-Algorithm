/******************************************************************************
  Buffers.c
  Defines a pool of buffers for printing messages.
******************************************************************************/

/* Include Files */
#include <mc9s12e128.h>
#include "Buffers.h"


/* Global Variables */
int NextBuf = 0;	// Points to the next buffer
char Buffers[NUMBUF][48];	// Lots of buffers of 48 bytes each


/*****************************************************************************/
/* Return the address of the next buffer */
char* GetBuffer(void)
{
	char* BufNum;

	BufNum = &Buffers[NextBuf++][0];
	if (NextBuf >= NUMBUF)
		NextBuf = 0;
	return(BufNum);
}