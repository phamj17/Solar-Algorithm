/******************************************************************************
  LCD.h
  Driver for the LCD module.
******************************************************************************/

#ifndef LCD_H
#define LCD_H


/* Defines */
#define ClearDisplay 0x01
#define ReturnHome 0x02
#define EntryModeSet 0x06		// Cursor shifts right
#define ShiftCursorRight 0x14
#define FunctionSet	0x38		// 8-bit data, 2-line display, 5x8 dots
//#define FunctionSet 0x3C		// 8-bit data, 2-line display, 5x10 dots
#define ControlCommand 0x0F		// Display on, Cursor on, Blinks on
#define SetLine2 0xC0

/* Function Prototypes */
void InitLCD(void);
char ReadLCD(void);
char ReadBusy(void);
void SendCommand(char Command);
void SendCommandBusy(char Command);
void SendData(char Data);
void SendMessage(char *Address);
void SendMsgAtCursor(char *Address);


#endif