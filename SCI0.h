/******************************************************************************
   SCI0.h
 *****************************************************************************/

#ifndef SCI0_H
#define SCI0_H

#include <mc9s12e128.h>

// Defines
#define ECLK  24000000  // Bus clock

#define SCI0_BAUD_RATE  19200 // Desired baud rate
#define SCI0_BAUD_DIV (ECLK/16)/SCI0_BAUD_RATE  // Value for baud register
#define rxqsize0  64    // Number of bytes in receive buffer
#define txqsize0  32    // Number of pointers in transmit queue
#define RcvBufSize0 32  // Number of bytes in Receive Buffer
#define DC3 0x13        // XOFF control character
#define DC1 0x11        // XON control character
#define NUL 0x00        // NUL control character

typedef char *pnt;
typedef void (*tPointerFcn)(void);  // Pointer to a function

// Rx buffer and the input and output pointers
typedef struct rx_q_struct
{
  word in;        // Input pointer
  word out;       // Output pointer
  char q[rxqsize0];  // Queue array of characters
}rx_q_struct;

// Tx buffer and its input and output pointers
typedef struct tx_q_struct
{
  word in;         // Input pointer
  word out;        // Output pointer
  pnt q[txqsize0];  // Queue array of pointers
}tx_q_struct;


// Function Prototypes
void InitSCI0(void);
void SCI0_SetBaud(void);
void SCI0_RxQueueInit(void);
void SCI0_StartRx(void);
void SCI0_StopRx(void);
void SCI0_TxQueueInit(void);
void SCI0_PutQueue(char *msg);
void SCI0_StartTx(void);
byte SCI0_GetChar(char *rxbyte);
void SCI0_SendMsg(char *s);
void SCI0_CheckQueue(void);
void SCI0_Rx(void);
void SCI0_Rx1(void);
void SCI0_Rx2(void);


#endif
