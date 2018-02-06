/******************************************************************************
   Vectors.c

******************************************************************************/
extern void near _Startup(void);		// Startup routine

//extern void near PMFA_Reload_Int(void);	// PMFA reload interrupt handler
//extern void near PMFB_Reload_Int(void);	// PMFB reload interrupt handler
//extern void near PMFC_Reload_Int(void);	// PMFC reload interrupt handler

//extern void near SCI1_Int(void);		// SCI1 interrupt handler
extern void near SCI0_Int(void);		// SCI0 interrupt handler

//extern void near SPI_Int(void);			// SPI interrupt handler
//extern void near ATD_Int(void);			// ATD interrupt handler
extern void near T0C4_Int(void);		// Timer 0 Channel 4 interrupt handler
//extern void near T0C5_Int(void);		// Timer 0 Channel 5 interrupt handler
extern void near KBD_Int(void);			// Keyboard interrupt handler
//extern void near RealTimeInterrupt(void);	// Real time interrupt handler


/******************************************************************************
   SOFTWARE TRAP FUNCTION
   DESCRIPTION:
   Function that traps  all unexpected interrupts.  Used for debugging
   software to find runaway code.
 *****************************************************************************/
/* These traps are useful in case an unused interrupt is inadvertently enabled.
   In that case, the code will loop in the trap and can be identified through
   the debugger by clicking on Stop.
*/
#pragma CODE_SEG __NEAR_SEG NON_BANKED
//#pragma CODE_SEG NON_BANKED
__interrupt void software_trap0(void){ for(;;); }
__interrupt void software_trap1(void){ for(;;); }
__interrupt void software_trap2(void){ for(;;); }
__interrupt void software_trap3(void){ for(;;); }
__interrupt void software_trap4(void){ for(;;); }
__interrupt void software_trap5(void){ for(;;); }
__interrupt void software_trap6(void){ for(;;); }
__interrupt void software_trap7(void){ for(;;); }
__interrupt void software_trap8(void){ for(;;); }
__interrupt void software_trap9(void){ for(;;); }
__interrupt void software_trap10(void){ for(;;); }
__interrupt void software_trap11(void){ for(;;); }
__interrupt void software_trap12(void){ for(;;); }
__interrupt void software_trap13(void){ for(;;); }
__interrupt void software_trap14(void){ for(;;); }
__interrupt void software_trap15(void){ for(;;); }
__interrupt void software_trap16(void){ for(;;); }
__interrupt void software_trap17(void){ for(;;); }
__interrupt void software_trap18(void){ for(;;); }
__interrupt void software_trap19(void){ for(;;); }
__interrupt void software_trap20(void){ for(;;); }
__interrupt void software_trap21(void){ for(;;); }
__interrupt void software_trap22(void){ for(;;); }
__interrupt void software_trap23(void){ for(;;); }
__interrupt void software_trap24(void){ for(;;); }
__interrupt void software_trap25(void){ for(;;); }
__interrupt void software_trap26(void){ for(;;); }
__interrupt void software_trap27(void){ for(;;); }
__interrupt void software_trap28(void){ for(;;); }
__interrupt void software_trap29(void){ for(;;); }
__interrupt void software_trap30(void){ for(;;); }
__interrupt void software_trap31(void){ for(;;); }
__interrupt void software_trap32(void){ for(;;); }
__interrupt void software_trap33(void){ for(;;); }
__interrupt void software_trap34(void){ for(;;); }
__interrupt void software_trap35(void){ for(;;); }
__interrupt void software_trap36(void){ for(;;); }
__interrupt void software_trap37(void){ for(;;); }
__interrupt void software_trap38(void){ for(;;); }
//__interrupt void software_trap39(void){ for(;;); }
__interrupt void software_trap40(void){ for(;;); }
__interrupt void software_trap41(void){ for(;;); }
__interrupt void software_trap42(void){ for(;;); }
//__interrupt void software_trap43(void){ for(;;); }
__interrupt void software_trap44(void){ for(;;); }
__interrupt void software_trap45(void){ for(;;); }
__interrupt void software_trap46(void){ for(;;); }
__interrupt void software_trap47(void){ for(;;); }
__interrupt void software_trap48(void){ for(;;); }
__interrupt void software_trap49(void){ for(;;); }
__interrupt void software_trap50(void){ for(;;); }
//__interrupt void software_trap51(void){ for(;;); }
__interrupt void software_trap52(void){ for(;;); }
__interrupt void software_trap53(void){ for(;;); }
__interrupt void software_trap54(void){ for(;;); }
__interrupt void software_trap55(void){ for(;;); }
__interrupt void software_trap56(void){ for(;;); }
__interrupt void software_trap57(void){ for(;;); }
__interrupt void software_trap58(void){ for(;;); }
__interrupt void software_trap59(void){ for(;;); }
__interrupt void software_trap60(void){ for(;;); }
__interrupt void software_trap61(void){ for(;;); }
__interrupt void software_trap62(void){ for(;;); }
//__interrupt void software_trap63(void){ for(;;); }
#pragma CODE_SEG DEFAULT	// Change code section to DEFAULT.


/*****************************************************************************/
typedef void (*near tIsrFunc)(void);
const tIsrFunc _vect[] @0xFF80 = {		// Interrupt table for stand-alone
//const tIsrFunc _vect[] @0xEF80 = {	// Interrupt table for loader
	software_trap0,		//  0 Reserved
	software_trap1,		//  1 Reserved
	software_trap2,		//  2 Reserved
	software_trap3,		//  3 Reserved
	software_trap4,		//  4 PWM Emergency Shutdown
	software_trap5,		//  5 VREG LVI
	software_trap6,		//  6 PWM Fault 3
	software_trap7,		//  7 PWM Fault 2
	software_trap8,		//  8 PWM Fault 1
	software_trap9,		//  9 PWM Fault 0
	software_trap10,	// 10 PWM Generator C Reload
	software_trap11,	// 11 PWM Generator B Reload
	software_trap12,	// 12 PWM Generator A Reload
	software_trap13,	// 13 Standard Timer 2 Pulse Accumulator Input Edge
	software_trap14,	// 14 Standard Timer 2 Pulse Accumulator Overflow
	software_trap15,	// 15 Standard Timer 2 Overflow
	software_trap16,	// 16 Standard Timer 2 Channel 7
	software_trap17,	// 17 Standard Timer 2 Channel 6
	software_trap18,	// 18 Standard Timer 2 Channel 5
	software_trap19,	// 19 Standard Timer 2 Channel 4
	software_trap20,	// 20 Reserved
	software_trap21,	// 21 Standard Timer 1 Pulse Accumulator Input Edge
	software_trap22,	// 22 Standard Timer 1 Pulse Accumulator Overflow
	software_trap23,	// 23 Standard Timer 1 Overflow
	software_trap24,	// 24 Standard Timer 1 Channel 7
	software_trap25,	// 25 Standard Timer 1 Channel 6
	software_trap26,	// 26 Standard Timer 1 Channel 5
	software_trap27,	// 27 Standard Timer 1 Channel 4
	software_trap28,	// 28 FLASH
	software_trap29,	// 29 Reserved
	software_trap30,	// 30 Reserved
	software_trap31,	// 31 Reserved
	software_trap32,	// 32 IIC Bus
	software_trap33,	// 33 Reserved
	software_trap34,	// 34 CRG Self Lock Mode
	software_trap35,	// 35 CRG PLL Lock
	software_trap36,	// 36 Reserved
	software_trap37,	// 37 Reserved
	software_trap38,	// 38 Reserved
	KBD_Int,			// 39 Port AD (KWU)
	software_trap40,	// 40 ATD
	software_trap41,	// 41 SCI2
	software_trap42,	// 42 SCI1
	SCI0_Int,			// 43 SCI0
	software_trap44,			// 44 SPI
	software_trap45,	// 45 Pulse Accumulator Input Edge
	software_trap46,	// 46 Pulse Accumulator Overflow
	software_trap47,	// 47 Standard Timer 0 Overflow
	software_trap48,	// 48 Standard Timer 0 Channel 7
	software_trap49,	// 49 Standard Timer 0 Channel 6
	software_trap50,	// 50 Standard Timer 0 Channel 5
	T0C4_Int,			// 51 Standard Timer 0 Channel 4
	software_trap52,	// 52 Reserved
	software_trap53,	// 53 Reserved
	software_trap54,	// 54 Reserved
	software_trap55,	// 55 Reserved
	software_trap56,	// 56 Real Time Interrupt
	software_trap57,	// 57 IRQ
	software_trap58,	// 58 XIRQ
	software_trap59,	// 59 SWI
	software_trap60,	// 60 Unimplemented Instruction Trap
	software_trap61,	// 61 COP Failure Reset
	software_trap62,	// 62 Clock Monitor Fail Reset
	_Startup			// 63 External Reset, POR, LVR
};
