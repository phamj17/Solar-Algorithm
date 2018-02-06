/******************************************************************************
   CPU.h
 *****************************************************************************/

#ifndef CPU_H
#define CPU_H


// Macro Definitions
#define stop_enable() {asm ANDCC #0x7F;}		// Enable stop mode
#define wait()        {asm WAI;}				// Enter wait mode
#define stop()        {asm STOP;}				// Enter stop mode
#define nop()         {asm NOP;}				// Enter NOP instruction

// Function Prototypes
void InitPLL(void);


#endif