/******************************************************************************
   Timer.h
 *****************************************************************************/

#ifndef TIMER_H
#define TIMER_H

#define NUMTIMERS 4

#include "Datatypes.h"


struct timers
{
	UINT16 TimerField;
	UINT8 Free;
};


// Function Prototypes
void InitT0C4(void);
void InitT0C5(void);
void InitT0C6(void);

void init_timerpool(void);
UINT8 get_timer(void);
void free_timer(UINT8 nbr);
void init_timer(UINT8 nbr, UINT16 tout);
UINT16 check_timer(UINT8 nbr);
void irqhandler_timer(void);





#endif