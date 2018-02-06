/******************************************************************************
   IO.h
 *****************************************************************************/

#ifndef IO_H
#define IO_H


// Function Prototypes
void InitIO(void);

void InitLED(void);
void Set_LED(void);
void Clear_LED(void);
void Toggle_LED(void);
void SetLEDBacklight(void);
void ClearLEDBacklight(void);
void AC_RelayOn(void);
void AC_RelayOff(void);
void InitBT(void);
void DisableBT(void);
void EnableBT(void);


#endif