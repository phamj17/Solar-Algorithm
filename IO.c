/******************************************************************************
   IO.c

******************************************************************************/

// Include Files
#include <MC9S12E128.h>		// Derivative information
#include "IO.h"




/*****************************************************************************/
void InitIO(void)
{


//-----------------------------------------------------------------------------
	/***** PORT AD *****/
//-----------------------------------------------------------------------------
	/* Port AD is associated with the analog-to-digital converter (ATD) and
	   keyboard wake-up (KWU) interrupts. Each pin is assigned to these modules
	   according to the following priority: ATD > KWU > general-purpose I/O.
	   For the pins of port AD to be used as inputs, the corresponding bits of
	   the ATDDIEN0 and ATDDIEN1 registers in the ATD module must be set to 1
	   (digital input buffer is enabled). The ATDDIEN0 and ATDDIEN1 registers
	   do not affect the port AD pins when they are configured as outputs.

	   During reset, port AD pins are configured as high-impedance analog inputs
	   (digital input buffer is disabled). */



	/* Port AD I/O Register (PTAD) */
	/* Read: Anytime. Write: Anytime. */
	/* If the data direction bit of the associated I/O pin (DDRADx) is set to 1
	   (output), a write to the corresponding I/O Register bit sets the value
	   to be driven to the Port AD pin. If the data direction bit of the
	   associated I/O pin (DDRADx) is set to 0 (input), a write to the
	   corresponding I/O Register bit takes place but has no effect on the Port
	   AD pin.

	   If the associated data direction bit (DDRADx) is set to 1 (output), a read
	   returns the value of the I/O register bit.

	   If the associated data direction bit (DDRADx) is set to 0 (input) and
	   the associated ATDDIEN0(1) bit is set to 0 (digital input buffer is
	   disabled), the associated I/O register bit (PTADx) reads '1'.

	   If the associated data direction bit (DDRADx) is set to 0 (input) and
	   the associated ATDDIEN0(1) bit is set to 1 (digital input buffer is
	   enabled), a read returns the value of the pin. */

	PTAD	=	(0
				/*| PTAD_PTAD0_MASK*/
				/*| PTAD_PTAD1_MASK*/
				/*| PTAD_PTAD2_MASK*/
				/*| PTAD_PTAD3_MASK*/
				/*| PTAD_PTAD4_MASK*/
				/*| PTAD_PTAD5_MASK*/
				/*| PTAD_PTAD6_MASK*/
				/*| PTAD_PTAD7_MASK*/
				/*| PTAD_PTAD8_MASK*/	// Switch 1 input
				/*| PTAD_PTAD9_MASK*/	// Switch 2 input
				/*| PTAD_PTAD10_MASK*/	// Switch 3 input
				/*| PTAD_PTAD11_MASK*/	// Switch 4 input
				/*| PTAD_PTAD12_MASK*/	// LED backlight on output (0 = on)
				/*| PTAD_PTAD13_MASK*/	// RS output
				/*| PTAD_PTAD14_MASK*/	// RW output
				/*| PTAD_PTAD15_MASK*/	// E output
				);



	/* Port AD Input Register (PTIAD) */
	/* Read: Anytime. Write: Never; writes to these registers have no effect. */
    /* If the ATDDIEN0(1) bit of the associated I/O pin is set to 0 (digital input
       buffer is disabled), a read returns a 1. If the ATDDIEN0(1) bit of the
	   associated I/O pin is set to 1 (digital input buffer is enabled), a read
	   returns the status of the associated pin. */

	// Variable = PTIAD;



	/* Port AD Data Direction Register (DDRAD) */
	/* Read: Anytime. Write: Anytime. */
	/* This register configures port pins PAD[15:0] as either input or output.

	   If a data direction bit is 0 (pin configured as input), then a read value
	   on PTADx depends on the associated ATDDIEN0(1) bit. If the associated
	   ATDDIEN0(1) bit is set to 1 (digital input buffer is enabled), a read
	   on PTADx returns the value on port AD pin. If the associated ATDDIEN0(1)
	   bit is set to 0 (digital input buffer is disabled), a read on PTADx returns
	   a 1.

	   0 Associated pin is configured as input.
	   1 Associated pin is configured as output. */

	DDRAD	=	(0
				/*| DDRAD_DDRAD0_MASK*/		// AC current sense micro
				/*| DDRAD_DDRAD1_MASK*/		// AC V in volatage sense micro
				/*| DDRAD_DDRAD2_MASK*/		// Input current sense micro
				/*| DDRAD_DDRAD3_MASK*/		// Input voltage sense micro
				/*| DDRAD_DDRAD4_MASK*/		// Bus voltage sense micro
				/*| DDRAD_DDRAD5_MASK*/		// Spare
				/*| DDRAD_DDRAD6_MASK*/		// Spare
				/*| DDRAD_DDRAD7_MASK*/		// Spare
				/*| DDRAD_DDRAD8_MASK*/		// Switch 1 input
				/*| DDRAD_DDRAD9_MASK*/		// Switch 2 input
				/*| DDRAD_DDRAD10_MASK*/	// Switch 3 input
				/*| DDRAD_DDRAD11_MASK*/	// Switch 4 input
				| DDRAD_DDRAD12_MASK		// LED backlight on output
				| DDRAD_DDRAD13_MASK		// RS output
				| DDRAD_DDRAD14_MASK		// RW output
				| DDRAD_DDRAD15_MASK		// E output
				);



	/* Port AD Reduced Drive Register (RDRAD) */
	/* Read: Anytime. Write: Anytime. */
	/* This register configures the drive strength of configured output pins as
	   either full or reduced. If a pin is configured as input, the corresponding
	   Reduced Drive Register bit has no effect. */

	RDRAD	=	(0							// Full drive strength for all outputs
				/*| RDRAD_RDRAD0_MASK*/
				/*| RDRAD_RDRAD1_MASK*/
				/*| RDRAD_RDRAD2_MASK*/
				/*| RDRAD_RDRAD3_MASK*/
				/*| RDRAD_RDRAD4_MASK*/
				/*| RDRAD_RDRAD5_MASK*/
				/*| RDRAD_RDRAD6_MASK*/
				/*| RDRAD_RDRAD7_MASK*/
				/*| RDRAD_RDRAD8_MASK*/
				/*| RDRAD_RDRAD9_MASK*/
				/*| RDRAD_RDRAD10_MASK*/
				/*| RDRAD_RDRAD11_MASK*/
				/*| RDRAD_RDRAD12_MASK*/	// Digital output
				/*| RDRAD_RDRAD13_MASK*/	// Digital output
				/*| RDRAD_RDRAD14_MASK*/	// Digital output
				/*| RDRAD_RDRAD15_MASK*/	// Digital output
				);



	/* Port AD Pull Device Enable Register (PERAD) */
	/* Read: Anytime. Write: Anytime. */
    /* This register configures whether a pull-up or a pull-down device is activated
       on configured input pins. If a pin is configured as output, the corresponding
       Pull Device Enable Register bit has no effect.
       0 Pull-up or pull-down device is disabled.
       1 Pull-up or pull-down device is enabled. */

	PERAD	=	(0
				/*| PERAD_PERAD0_MASK*/
				/*| PERAD_PERAD1_MASK*/
				/*| PERAD_PERAD2_MASK*/
				/*| PERAD_PERAD3_MASK*/
				/*| PERAD_PERAD4_MASK*/
				/*| PERAD_PERAD5_MASK*/
				/*| PERAD_PERAD6_MASK*/
				/*| PERAD_PERAD7_MASK*/
				| PERAD_PERAD8_MASK			// Pull up activated on digital input
				| PERAD_PERAD9_MASK			// Pull up activated on digital input
				| PERAD_PERAD10_MASK		// Pull up activated on digital input
				| PERAD_PERAD11_MASK		// Pull up activated on digital input
				/*| PERAD_PERAD12_MASK*/
				/*| PERAD_PERAD13_MASK*/
				/*| PERAD_PERAD14_MASK*/
				/*| PERAD_PERAD15_MASK*/
				);



	/* Port AD Polarity Select Register (PPSAD) */
	/* Read: Anytime. Write: Anytime. */
	/* The Port AD Polarity Select Register serves a dual purpose by selecting the
	   polarity of the active interrupt edge as well as selecting a pull-up or
	   pull-down device if enabled (PERADx = 1). The Port AD Polarity Select Register
	   is effective only when the corresponding Data Direction Register bit is set
	   to 0 (input). In pull-down mode (PPSADx = 1), a rising edge on a port AD pin
	   sets the corresponding PIFADx bit. In pull-up mode (PPSADx = 0), a falling
	   edge on a port AD pin sets the corresponding PIFADx bit.
	   0 A pull-up device is connected to the associated port AD pin, and detects
	     falling edge for interrupt generation.
	   1 A pull-down device is connected to the associated port AD pin, and detects
	     rising edge for interrupt generation. */

	PPSAD	=	(0
				/*| PPSAD_PPSAD0_MASK*/
				/*| PPSAD_PPSAD1_MASK*/
				/*| PPSAD_PPSAD2_MASK*/
				/*| PPSAD_PPSAD3_MASK*/
				/*| PPSAD_PPSAD4_MASK*/
				/*| PPSAD_PPSAD5_MASK*/
				/*| PPSAD_PPSAD6_MASK*/
				/*| PPSAD_PPSAD7_MASK*/
				/*| PPSAD_PPSAD8_MASK*/		// Pull up
				/*| PPSAD_PPSAD9_MASK*/		// Pull up
				/*| PPSAD_PPSAD10_MASK*/	// Pull up
				/*| PPSAD_PPSAD11_MASK*/	// Pull up
				/*| PPSAD_PPSAD12_MASK*/
				/*| PPSAD_PPSAD13_MASK*/
				/*| PPSAD_PPSAD14_MASK*/
				/*| PPSAD_PPSAD15_MASK*/
				);



	/* Port AD Interrupt Enable Register (PIEAD) */
	/* Read: Anytime. Write: Anytime. */
	/* This register disables or enables on a per pin basis the edge sensitive
	   external interrupt associated with port AD.
	   0 Interrupt is disabled (interrupt flag masked).
	   1 Interrupt is enabled. */

	PIEAD	=	(0
				/*| PIEAD_PIEAD0_MASK*/
				/*| PIEAD_PIEAD1_MASK*/
				/*| PIEAD_PIEAD2_MASK*/
				/*| PIEAD_PIEAD3_MASK*/
				/*| PIEAD_PIEAD4_MASK*/
				/*| PIEAD_PIEAD5_MASK*/
				/*| PIEAD_PIEAD6_MASK*/
				/*| PIEAD_PIEAD7_MASK*/
				/*| PIEAD_PIEAD8_MASK*/		// Keyboard interrupts disabled
				/*| PIEAD_PIEAD9_MASK*/		// Keyboard interrupts disabled
				/*| PIEAD_PIEAD10_MASK*/	// Keyboard interrupts disabled
				/*| PIEAD_PIEAD11_MASK*/	// Keyboard interrupts disabled
				/*| PIEAD_PIEAD12_MASK*/
				/*| PIEAD_PIEAD13_MASK*/
				/*| PIEAD_PIEAD14_MASK*/
				/*| PIEAD_PIEAD15_MASK*/
				);



	/* Port AD Interrupt Flag Register (PIFAD) */
	/* Read: Anytime. Write: Anytime. */
	/* Each flag is set by an active edge on the associated input pin. The active
	   edge could be rising or falling based on the state of the corresponding
	   PPSADx bit. To clear each flag, write '1' to the corresponding PIFADx bit.
	   Writing a '0' has no effect.
	   0 No active edge pending. Writing a '0' has no effect.
	   1 Active edge on the associated bit has occurred (an interrupt will occur
	   if the associated enable bit is set). Writing a '1' clears the associated
	   flag. */

	PIFAD	=	(0							// Clear all pending interrupts
				| PIEAD_PIEAD0_MASK
				| PIEAD_PIEAD1_MASK
				| PIEAD_PIEAD2_MASK
				| PIEAD_PIEAD3_MASK
				| PIEAD_PIEAD4_MASK
				| PIEAD_PIEAD5_MASK
				| PIEAD_PIEAD6_MASK
				| PIEAD_PIEAD7_MASK
				| PIEAD_PIEAD8_MASK
				| PIEAD_PIEAD9_MASK
				| PIEAD_PIEAD10_MASK
				| PIEAD_PIEAD11_MASK
				| PIEAD_PIEAD12_MASK
				| PIEAD_PIEAD13_MASK
				| PIEAD_PIEAD14_MASK
				| PIEAD_PIEAD15_MASK
				);


//-----------------------------------------------------------------------------
	/***** Port M *****/
//-----------------------------------------------------------------------------
	/* Port M is associated with the serial communication interface (SCI2),
	   Inter-IC bus (IIC) and the digital to analog converter (DAC0 and DAC1) modules.
	   Each pin is assigned to these modules according to the following priority:
	   IIC/SCI2/DAC1/DAC0 > general-purpose I/O.

	   When the IIC bus is enabled, the PM[7:6] pins become SCL and SDA respectively.

	   When the SCI2 receiver and transmitter are enabled, the PM[5:4] become RXD2
	   and TXD2 respectively.

	   When the DAC1 and DAC0 outputs are enabled, the PM[1:0] become DAO1 and DAO0
	   respectively.

	   During reset, PM[3] and PM[1:0] pins are configured as high-impedance inputs
	   and PM[7:4] pins are configured as pull-up inputs. */



	/* Port M I/O Register (PTM) */
	/* Read: Anytime. Write: Anytime. */
	/* If the associated data direction bit (DDRMx) is set to 1 (output), a read
	   returns the value of the I/O register bit. If the associated data direction
	   bit (DDRMx) is set to 0 (input), a read returns the value of the pin. */

	PTM		=	(0
				/*| PTM_PTM0_MASK*/		// Controlled by DAC0
				/*| PTM_PTM1_MASK*/		// Controlled by DAC1

				/*| PTM_PTM3_MASK*/		// AC Relay On
				/*| PTM_PTM4_MASK*/		// Switch1 LED
				/*| PTM_PTM5_MASK*/		// Switch2 LED
				/*| PTM_PTM6_MASK*/		// Switch3 LED
				/*| PTM_PTM7_MASK*/		// Switch4 LED
				);



	/* Port M Input Register (PTIM) */
	/* Read: Anytime. Write: Never, writes to this register have no effect. */
	/* This register always reads back the status of the associated pins. */

	// Variable = PTIM;



	/* Port M Data Direction Register (DDRM) */
	/* Read: Anytime. Write: Anytime. */
	/* This register configures port pins PM[7:3] and PM[1:0] as either input or output.

	   If the IIC is enabled, the IIC controls the SCL and SDA I/O direction, and the
	   corresponding DDRM[7:6] bits have no effect on their I/O direction.

	   If the SCI2 transmitter is enabled, the I/O direction of the transmit pin TXD2
	   is controlled by SCI2, and the DDRM5 bit has no effect. If the SCI2 receiver is
	   enabled, the I/O direction of the receive pin RXD2 is controlled by SCI2, and the
	   DDRM4 bit has no effect.

	   If the DAC1 or DAC0 channel is enabled, the associated pin DAO1 or DAO0 is forced
	   to be output, and the associated DDRM1 or DDRM0 bit has no effect.

	   The DDRM bits do not change to reflect the pin I/O direction when not being used
	   as GPIO. The DDRM[7:3]; DDRM[1:0] bits revert to controlling the I/O direction of
	   the pins when the associated IIC, SCI, or DAC1/0 function are disabled.
	   0 Associated pin is configured as input.
	   1 Associated pin is configured as output. */

	DDRM	=	(0
				/*| DDRM_DDRM0_MASK*/	// Controlled by DAC0
				/*| DDRM_DDRM1_MASK*/	// Controlled by DAC1
										// Unbonded pin
				| DDRM_DDRM3_MASK		// AC Relay On (output)
				| DDRM_DDRM4_MASK		// Switch1 LED (output)
				| DDRM_DDRM5_MASK		// Switch2 LED (output)
				| DDRM_DDRM6_MASK		// Switch3 LED (output)
				| DDRM_DDRM7_MASK		// Switch4 LED (output)
				);



	/* Port M Reduced Drive Register (RDRM) */
	/* Read: Anytime. Write: Anytime. */
	/* This register configures the drive strength of configured output pins as either
	   full or reduced. If a pin is configured as input, the corresponding Reduced Drive
	   Register bit has no effect.
	   0 Full drive strength at output
	   1 Associated pin drives at about 1/3 of the full drive strength. */

	RDRM	=	(0
				/*| RDRM_RDRM0_MASK*/
				/*| RDRM_RDRM1_MASK*/

				/*| RDRM_RDRM3_MASK*/
				/*| RDRM_RDRM4_MASK*/
				/*| RDRM_RDRM5_MASK*/
				/*| RDRM_RDRM6_MASK*/
				/*| RDRM_RDRM7_MASK*/
				);



	/* Port M Pull Device Enable Register (PERM) */
	/* Read: Anytime. Write: Anytime. */
	/* This register configures whether a pull-up or a pull-down device is activated on
	   configured input or wired-or output pins. If a pin is configured as push-pull
	   output, the corresponding Pull Device Enable Register bit has no effect.
	   0 Pull-up or pull-down device is disabled.
	   1 Pull-up or pull-down device is enabled. */

	PERM	=	(0
				/*| PERM_PERM0_MASK*/
				/*| PERM_PERM1_MASK*/

				/*| PERM_PERM3_MASK*/
				/*| PERM_PERM4_MASK*/
				/*| PERM_PERM5_MASK*/
				/*| PERM_PERM6_MASK*/
				/*| PERM_PERM7_MASK*/
				);



	/* Port M Polarity Select Register (PPSM) */
	/* Read: Anytime. Write: Anytime. */
	/* The Port M Polarity Select Register selects whether a pull-down or a pull-up
	   device is connected to the pin. The PortMPolarity Select Register is effective
	   only when the corresponding Data Direction Register bit is set to 0 (input)
	   and the corresponding Pull Device Enable Register bit is set to 1.
	   0 A pull-up device is connected to the associated port M pin.
	   1 A pull-down device is connected to the associated port M pin. */

	PPSM	=	(0
				/*| PPSM_PPSM0_MASK*/
				/*| PPSM_PPSM1_MASK*/

				/*| PPSM_PPSM3_MASK*/
				/*| PPSM_PPSM4_MASK*/
				/*| PPSM_PPSM5_MASK*/
				/*| PPSM_PPSM6_MASK*/
				/*| PPSM_PPSM7_MASK*/
				);



	/* Port M Wired-OR Mode Register (WOMM) */
	/* Read: Anytime. Write: Anytime. */
	/* This register selects whether a port M output is configured as push-pull or
	   wired-or. When a Wired-OR Mode Register bit is set to 1, the corresponding
	   output pin is driven active low only (open drain) and a high level is not
	   driven. A Wired-OR Mode Register bit has no effect if the corresponding pin
	   is configured as an input.

	   These bits apply also to the SCI2 outputs and allow a multipoint connection
	   of several serial modules.

	   If the IIC is enabled, the associated pins are always set to wired-or mode,
	   and the state of the WOMM[7:6] bits have no effect. The WOMM[7:6] bits will
	   not change to reflect their wired-or mode configuration when the IIC is enabled.
	   0 Output buffers operate as push-pull outputs.
	   1 Output buffers operate as open-drain outputs. */

	WOMM	=	(0
				/*| WOMM_WOMM4_MASK*/
				/*| WOMM_WOMM5_MASK*/
				/*| WOMM_WOMM6_MASK*/
				/*| WOMM_WOMM7_MASK*/
				);


//-----------------------------------------------------------------------------
	/***** Port P *****/
//-----------------------------------------------------------------------------
	/* Port P is associated with the Pulse Width Modulator (PMF) modules. Each
	   pin is assigned according to the following priority: PMF > general-purpose I/O.

	   When a PMF channel is enabled, the corresponding pin becomes a PWM output.

	   During reset, port P pins are configured as high-impedance inputs. */



	/* Port P I/O Register (PTP) */
	/* Read: Anytime. Write: Anytime. */
	/* If the associated data direction bit (DDRPx) is set to 1 (output), a read
	   returns the value of the I/O register bit. If the associated data direction
	   bit (DDRPx) is set to 0 (input), a read returns the value of the pin.

	   The PMF function takes precedence over the general-purpose I/O function if
	   the associated PWM channel is enabled. The PWM channels 5-0 are outputs if
	   the respective channels are enabled. */

	PTP		=	(0
				/*| PTP_PTP0_MASK*/		// Controlled by PWM-A
				/*| PTP_PTP1_MASK*/		// Controlled by PWM-B
				| PTP_PTP2_MASK			// Shutdown Drive (output) (hi for shutdown)
				/*| PTP_PTP3_MASK*/		// Enable Drive (output) (hi for enable)
				| PTP_PTP4_MASK			// Enable COM0 (output) (hi for enable)
				/*| PTP_PTP5_MASK*/		// Spare
				);

	/* Port P Input Register (PTIP) */
	/* This register always reads back the status of the associated pins. */

	/* Port P Data Direction Register (DDRP) */
	/* This register configures port pins PP[5:0] as either input or output.

	   If a PMF channel is enabled, the corresponding pin is forced to be an output
	   and the associated Data Direction Register bit has no effect. If a PMF channel
	   is disabled, the corresponding Data Direction Register bit reverts to control
	   the I/O direction of the associated pin.
	   0 Associated pin is configured as input.
	   1 Associated pin is configured as output. */

	DDRP	=	(0
				| DDRP_DDRP0_MASK		// Controlled by PWM-A
				| DDRP_DDRP1_MASK		// Controlled by PWM-B
				| DDRP_DDRP2_MASK		// Shutdown Drive (output)
				| DDRP_DDRP3_MASK		// Enable Drive (output)
				| DDRP_DDRP4_MASK		// Enable COM0 (output)
				| DDRP_DDRP5_MASK		// Spare (make it an output)
				);

	/* Port P Reduced Drive Register (RDRP) */
	/* This register configures the drive strength of configured output pins as
	   either full or reduced. If a pin is configured as input, the corresponding
	   Reduced Drive Register bit has no effect.
	   0 Full drive strength at output.
	   1 Associated pin drives at about 1/3 of the full drive strength. */

	RDRP	=	(0
				/*| RDRP_RDRP0_MASK*/
				/*| RDRP_RDRP1_MASK*/
				/*| RDRP_RDRP2_MASK*/
				/*| RDRP_RDRP3_MASK*/
				/*| RDRP_RDRP4_MASK*/
				/*| RDRP_RDRP5_MASK*/
				);

	/* Port P Pull Device Enable Register (PERP) */
	/* This register configures whether a pull-up or a pull-down device is activated
	   on configured input pins. If a pin is configured as output, the corresponding
	   Pull Device Enable Register bit has no effect.
	   0 Pull-up or pull-down device is disabled.
	   1 Pull-up or pull-down device is enabled. */

	PERP	=	(0
				/*| PERP_PERP0_MASK*/
				/*| PERP_PERP1_MASK*/
				/*| PERP_PERP2_MASK*/
				/*| PERP_PERP3_MASK*/
				/*| PERP_PERP4_MASK*/
				/*| PERP_PERP5_MASK*/
				);

	/* Port P Polarity Select Register (PPSP) */
	/* The Port P Polarity Select Register selects whether a pull-down or a pull-up
	   device is connected to the pin. The Port P Polarity Select Register is
	   effective only when the corresponding Data Direction Register bit is set
	   to 0 (input) and the corresponding Pull Device Enable Register bit is set to 1.
	   0 A pull-up device is connected to the associated port P pin.
	   1 A pull-down device is connected to the associated port P pin. */

	PPSP	=	(0
				/*| PPSP_PPSP0_MASK*/
				/*| PPSP_PPSP1_MASK*/
				/*| PPSP_PPSP2_MASK*/
				/*| PPSP_PPSP3_MASK*/
				/*| PPSP_PPSP4_MASK*/
				/*| PPSP_PPSP5_MASK*/
				);


//-----------------------------------------------------------------------------
	/***** Port Q *****/
//-----------------------------------------------------------------------------
	/* Port Q is associated with the Pulse Width Modulator (PMF) modules. Each pin
	   is assigned according to the following priority: PMF > general-purpose I/O.

	   When a current status or fault function is enabled, the corresponding pin
	   becomes an input. PQ[3:0] are connected to FAULT[3:0] inputs and PQ[6:4] are
	   connected to IS[2:0] inputs of the PMF module.

	   During reset, port Q pins are configured as high-impedance inputs. */



	/* Port Q I/O Register (PTQ) */
	/* Read: Anytime. Write: Anytime. */
	/* If the associated data direction bit (DDRQx) is set to 1 (output), a read
	   returns the value of the I/O register bit. If the associated data direction
	   bit (DDRQx) is set to 0 (input), a read returns the value of the pin. */

	PTQ		=	(0
				/*| PTQ_PTQ0_MASK*/
				/*| PTQ_PTQ1_MASK*/
				/*| PTQ_PTQ2_MASK*/
				/*| PTQ_PTQ3_MASK*/
				/*| PTQ_PTQ4_MASK*/
				/*| PTQ_PTQ5_MASK*/
				/*| PTQ_PTQ6_MASK*/
				);



	/* Port Q Input Register (PTIQ) */
	/* Read: Anytime. Write: Never, writes to this register have no effect. */
	/* This register always reads back the status of the associated pins. */

	// Variable = PTIQ;



	/* Port Q Data Direction Register (DDRQ) */
	/* Read: Anytime. Write: Anytime. */
	/* This register configures port pins PQ[6:0] as either input or output.

	   If a PMF function is enabled, the corresponding pin is forced to be an input
	   and the associated Data Direction Register bit has no effect. If a PMF channel
	   is disabled, the corresponding Data Direction Register bit reverts to control
	   the I/O direction of the associated pin.
	   0 Associated pin is configured as input.
	   1 Associated pin is configured as output. */

	DDRQ	=	(0
				| DDRQ_DDRQ0_MASK		// Enable Bluetooth (output)
				| DDRQ_DDRQ1_MASK		// Bluetooth Reset (output)
				| DDRQ_DDRQ2_MASK		// Auto Discovery (output)
				| DDRQ_DDRQ3_MASK		// Set Factory Defaults (output)
				| DDRQ_DDRQ4_MASK		// Set BT Master (output)
				| DDRQ_DDRQ5_MASK		// Set BAUD Rate (output)
				/*| DDRQ_DDRQ6_MASK*/	// Connected (input)
				);



	/* Port Q Reduced Drive Register (RDRQ) */
	/* Read:Anytime. Write:Anytime. */
	/* This register configures the drive strength of configured output pins as
	   either full or reduced. If a pin is configured as input, the corresponding
	   Reduced Drive Register bit has no effect.
	   0 Full drive strength at output.
	   1 Associated pin drives at about 1/3 of the full drive strength. */

	RDRQ	=	(0
				/*| RDRQ_RDRQ0_MASK*/
				/*| RDRQ_RDRQ1_MASK*/
				/*| RDRQ_RDRQ2_MASK*/
				/*| RDRQ_RDRQ3_MASK*/
				/*| RDRQ_RDRQ4_MASK*/
				/*| RDRQ_RDRQ5_MASK*/
				/*| RDRQ_RDRQ6_MASK*/
				);



	/* Port Q Pull Device Enable Register (PERQ) */
	/* Read: Anytime. Write: Anytime. */
	/* This register configures whether a pull-up or a pull-down device is
	   activated on configured input pins. If a pin is configured as output,
	   the corresponding Pull Device Enable Register bit has no effect.
	   0 Pull-up or pull-down device is disabled.
	   1 Pull-up or pull-down device is enabled. */

	PERQ	=	(0
				/*| PERQ_PERQ0_MASK*/
				/*| PERQ_PERQ1_MASK*/
				/*| PERQ_PERQ2_MASK*/
				/*| PERQ_PERQ3_MASK*/
				/*| PERQ_PERQ4_MASK*/
				/*| PERQ_PERQ5_MASK*/
				/*| PERQ_PERQ6_MASK*/
				);



	/* Port Q Polarity Select Register (PPSQ) */
	/* Read: Anytime. Write: Anytime. */
	/* The Port Q Polarity Select Register selects whether a pull-down or a pull-up
	   device is connected to the pin. The Port Q Polarity Select Register is
	   effective only when the corresponding Data Direction Register bit is set to 0
	   (input) and the corresponding Pull Device Enable Register bit is set to 1.
	   0 A pull-up device is connected to the associated port Q pin.
	   1 A pull-down device is connected to the associated port Q pin. */

	PPSQ	=	(0
				/*| PPSQ_PPSQ0_MASK*/
				/*| PPSQ_PPSQ1_MASK*/
				/*| PPSQ_PPSQ2_MASK*/
				/*| PPSQ_PPSQ3_MASK*/
				/*| PPSQ_PPSQ4_MASK*/
				/*| PPSQ_PPSQ5_MASK*/
				/*| PPSQ_PPSQ6_MASK*/
				);


//-----------------------------------------------------------------------------
	/***** Port S *****/
//-----------------------------------------------------------------------------
	/* Port S is associated with the serial peripheral interface (SPI) and serial
	   communication interfaces (SCI0 and SCI1). Each pin is assigned to these modules
	   according to the following priority: SPI/SCI1/SCI0 > general-purpose I/O.

	   When the SPI is enabled, the PS[7:4] pins become SS, SCK, MOSI, and MISO
	   respectively.

	   When the SCI1 receiver and transmitter are enabled, the PS[3:2] pins become
	   TXD1 and RXD1 respectively. When the SCI0 receiver and transmitter are enabled,
	   the PS[1:0] pins become TXD0 and RXD0 respectively.

	   During reset, port S pins are configured as high-impedance inputs. */

	/* Port S I/O Register (PTS) */
	/* If the associated data direction bit (DDRSx) is set to 1 (output), a read
	   returns the value of the I/O register bit. If the associated data direction
	   bit (DDRSx) is set to 0 (input), a read returns the value of the pin. */

	PTS		=	(0
				/*| PTS_PTS0_MASK*/		// Controlled by COM0 RXD
				/*| PTS_PTS1_MASK*/		// Controlled by COM0 TXD
				/*| PTS_PTS2_MASK*/		// Controlled by COM1 RXD
				| PTS_PTS3_MASK			// Controlled by COM1 TXD
				/*| PTS_PTS4_MASK*/	 	// COM0 RTS (input)
				| PTS_PTS5_MASK			// COM0 CTS (output)
				/*| PTS_PTS6_MASK*/		// COM1 RTS (input)
				/*| PTS_PTS7_MASK*/		// COM1 CTS (output) (HIGH disables transmitter
				);

	//PTS_PTS5 = ~(PTS_PTS5);
	//PTS_PTS5 = 1;						// Set CTS0



	/* Port S Input Register (PTIS) */
	/* Read: Anytime. Write: Never, writes to this register have no effect. */
	/* This register always reads back the status of the associated pins. */

	// Variable = PTIS;



	/* Port S Data Direction Register (DDRS) */
	/* Read: Anytime. Write: Anytime. */
	/* This register configures port pins PS[7:4] and PS[2:0] as either input or
	   output. When the SPI is enabled, the PS[7:4] pins become the SPI bidirectional
	   pins. The associated Data Direction Register bits have no effect.

	   When the SCI1 transmitter is enabled, the PS[3] pin becomes the TXD1 output pin
	   and the associated Data Direction Register bit has no effect. When the SCI1
	   receiver is enabled, the PS[2] pin becomes the RXD1 input pin and the associated
	   Data Direction Register bit has no effect.

	   When the SCI0 transmitter is enabled, the PS[1] pin becomes the TXD0 output pin
	   and the associated Data Direction Register bit has no effect. When the SCI0
	   receiver is enabled, the PS[0] pin becomes the RXD0 input pin and the associated
	   Data Direction Register bit has no effect.

	   If the SPI, SCI1 and SCI0 functions are disabled, the corresponding Data
	   Direction Register bit reverts to control the I/O direction of the associated
	   pin.

	   0 Associated pin is configured as input.
	   1 Associated pin is configured as output. */

	DDRS	=	(0
				/*| DDRS_DDRS0_MASK*/	// Controlled by COM0 RXD
				/*| DDRS_DDRS1_MASK*/	// Controlled by COM0 TXD
				/*| DDRS_DDRS2_MASK*/	// Controlled by COM1 RXD
				| DDRS_DDRS3_MASK		// Controlled by COM1 TXD
				/*| DDRS_DDRS4_MASK*/	// COM0 RTS (input)
				| DDRS_DDRS5_MASK		// COM0 CTS (output)
				/*| DDRS_DDRS6_MASK*/	// COM1 RTS (input)
				/*| DDRS_DDRS7_MASK*/	// COM1 CTS (output)
				);

	/* Port S Reduced Drive Register (RDRS) */
	/* Read: Anytime. Write: Anytime. */
	/* This register configures the drive strength of configured output pins as
	   either full or reduced. If a pin is configured as input, the corresponding
	   Reduced Drive Register bit has no effect.
	   0 Full drive strength at output.
	   1 Associated pin drives at about 1/3 of the full drive strength. */

	RDRS	=	(0
				/*| RDRS_RDRS0_MASK*/
				/*| RDRS_RDRS1_MASK*/
				/*| RDRS_RDRS2_MASK*/
				/*| RDRS_RDRS3_MASK*/
				/*| RDRS_RDRS4_MASK*/
				/*| RDRS_RDRS5_MASK*/
				/*| RDRS_RDRS6_MASK*/
				/*| RDRS_RDRS7_MASK*/
				);



	/* Port S Pull Device Enable Register (PERS) */
	/* Read: Anytime. Write: Anytime. */
	/* This register configures whether a pull-up or a pull-down device is
	   activated on configured input or wired-or (open drain) output pins. If a
	   pin is configured as push-pull output, the corresponding Pull Device
	   Enable Register bit has no effect.
	   0 Pull-up or pull-down device is disabled.
	   1 Pull-up or pull-down device is enabled. */

	PERS	=	(0
				/*| PERS_PERS0_MASK*/
				/*| PERS_PERS1_MASK*/
				/*| PERS_PERS2_MASK*/
				/*| PERS_PERS3_MASK*/
				/*| PERS_PERS4_MASK*/
				/*| PERS_PERS5_MASK*/
				/*| PERS_PERS6_MASK*/
				/*| PERS_PERS7_MASK*/
				);



	/* Port S Polarity Select Register (PPSS) */
	/* Read: Anytime. Write: Anytime. */
	/* The Port S Polarity Select Register selects whether a pull-down or a
	   pull-up device is connected to the pin. The Port S Polarity Select Register
	   is effective only when the corresponding Data Direction Register bit is set
	   to 0 (input) and the corresponding Pull Device Enable Register bit is set to 1.
	   0 A pull-up device is connected to the associated port S pin.
	   1 A pull-down device is connected to the associated port S pin. */

	PPSS	=	(0
				/*| PPSS_PPSS0_MASK*/
				/*| PPSS_PPSS1_MASK*/
				/*| PPSS_PPSS2_MASK*/
				/*| PPSS_PPSS3_MASK*/
				/*| PPSS_PPSS4_MASK*/
				/*| PPSS_PPSS5_MASK*/
				/*| PPSS_PPSS6_MASK*/
				/*| PPSS_PPSS7_MASK*/
				);



	/* Port S Wired-OR Mode Register (WOMS) */
	/* Read: Anytime. Write: Anytime. */
	/* This register selects whether a port S output is configured as push-pull or
	   wired-or. When a Wired-OR Mode Register bit is set to 1, the corresponding
	   output pin is driven active low only (open drain) and a high level is not
	   driven.AWired-OR Mode Register bit has no effect if the corresponding pin is
	   configured as an input.
	   0 Output buffers operate as push-pull outputs.
	   1 Output buffers operate as open-drain outputs. */

	WOMS	=	(0
				/*| WOMS_WOMS0_MASK*/
				/*| WOMS_WOMS1_MASK*/
				/*| WOMS_WOMS2_MASK*/
				/*| WOMS_WOMS3_MASK*/
				/*| WOMS_WOMS4_MASK*/
				/*| WOMS_WOMS5_MASK*/
				/*| WOMS_WOMS6_MASK*/
				/*| WOMS_WOMS7_MASK*/
				);


//-----------------------------------------------------------------------------
	/***** Port T *****/
//-----------------------------------------------------------------------------
	/* Port T is associated with two 4-channel timers (TIM0 and TIM1). Each pin
	   is assigned to these modules according to the following priority:
	   TIM1/TIM0 > general-purpose I/O.

	   If the timer TIM0 is enabled, the channels configured for output compare
	   are available on port T pins PT[3:0]. If the timer TIM1 is enabled, the
	   channels configured for output compare are available on port T pins PT[7:4].

	   During reset, port T pins are configured as high-impedance inputs. */



	/* Port T I/O Register (PTT) */
	/* Read: Anytime. Write: Anytime. */
	/* If the associated data direction bit (DDRTx) is set to 1 (output), a read
	   returns the value of the I/O register bit. If the associated data direction
	   bit (DDRTx) is set to 0 (input), a read returns the value of the pin. */

	PTT		=	(0
				/*| PTT_PTT0_MASK*/
				/*| PTT_PTT1_MASK*/
				/*| PTT_PTT2_MASK*/
				/*| PTT_PTT3_MASK*/
				/*| PTT_PTT4_MASK*/
				/*| PTT_PTT5_MASK*/
				/*| PTT_PTT6_MASK*/
				/*| PTT_PTT7_MASK*/
				);

	/* Port T Input Register (PTIT) */
	/* Read: Anytime. Write: Never, writes to this register have no effect. */
	/* This register always reads back the status of the associated pins. */

	// Variable = PTIT;



	/* Port T Data Direction Register (DDRT) */
	/* Read: Anytime. Write: Anytime. */
	/* This register configures port pins PT[7:0] as either input or output.

	   If the TIM0(1) module is enabled, each port pin configured for output
	   compare is forced to be an output and the associated Data Direction Register
	   bit has no effect. If the associated timer output compare is disabled,
	   the corresponding DDRTx bit reverts to control the I/O direction of the
	   associated pin.

	   If the TIM0(1) module is enabled, each port pin configured as an input
	   capture has the corresponding DDRTx bit controlling the I/O direction
	   of the associated pin.
	   0 Associated pin is configured as input.
	   1 Associated pin is configured as output. */

	DDRT	=	(0
				| DDRT_DDRT0_MASK		// DB0
				| DDRT_DDRT1_MASK		// DB1
				| DDRT_DDRT2_MASK		// DB2
				| DDRT_DDRT3_MASK		// DB3
				| DDRT_DDRT4_MASK		// DB4
				| DDRT_DDRT5_MASK		// DB5
				| DDRT_DDRT6_MASK		// DB6
				| DDRT_DDRT7_MASK		// DB7
				);



	/* Port T Reduced Drive Register (RDRT) */
	/* Read: Anytime. Write: Anytime. */
	/* This register configures the drive strength of configured output pins as
	   either full or reduced. If a pin is configured as input, the corresponding
	   Reduced Drive Register bit has no effect.
	   0 Full drive strength at output.
	   1 Associated pin drives at about 1/3 of the full drive strength. */

	RDRT	=	(0
				/*| RDRT_RDRT0_MASK*/
				/*| RDRT_RDRT1_MASK*/
				/*| RDRT_RDRT2_MASK*/
				/*| RDRT_RDRT3_MASK*/
				/*| RDRT_RDRT4_MASK*/
				/*| RDRT_RDRT5_MASK*/
				/*| RDRT_RDRT6_MASK*/
				/*| RDRT_RDRT7_MASK*/
				);



	/* Port T Pull Device Enable Register (PERT) */
	/* Read: Anytime. Write: Anytime. */
	/* This register configures whether a pull-up or a pull-down device is
	   activated on configured input pins. If a pin is configured as output,
	   the corresponding Pull Device Enable Register bit has no effect.
	   0 Pull-up or pull-down device is disabled.
	   1 Pull-up or pull-down device is enabled. */

	PERT	=	(0						// Enable pull-up on all bits
				| PERT_PERT0_MASK
				| PERT_PERT1_MASK
				| PERT_PERT2_MASK
				| PERT_PERT3_MASK
				| PERT_PERT4_MASK
				| PERT_PERT5_MASK
				| PERT_PERT6_MASK
				| PERT_PERT7_MASK
				);



	/* Port T Polarity Select Register (PPST) */
	/* Read: Anytime. Write: Anytime. */
	/* The Port T Polarity Select Register selects whether a pull-down or a
	   pull-up device is connected to the pin. The Port T Polarity Select Register
	   is effective only when the corresponding Data Direction Register bit is
	   set to 0 (input) and the corresponding Pull Device Enable Register bit
	   is set to 1.
	   0 A pull-up device is connected to the associated port T pin.
	   1 A pull-down device is connected to the associated port T pin. */

	PPST	=	(0						// Pull-up
				/*| PPST_PPST0_MASK*/
				/*| PPST_PPST1_MASK*/
				/*| PPST_PPST2_MASK*/
				/*| PPST_PPST3_MASK*/
				/*| PPST_PPST4_MASK*/
				/*| PPST_PPST5_MASK*/
				/*| PPST_PPST6_MASK*/
				/*| PPST_PPST7_MASK*/
				);


//-----------------------------------------------------------------------------
	/***** Port U *****/
//-----------------------------------------------------------------------------
	/* Port U is associated with one 4-channel timer (TIM2) and the pulse width
	   modulator (PWM) module. Each pin is assigned to these modules according
	   to the following priority: TIM2/PWM > general-purpose I/O.

	   If the timer TIM2 is enabled, the channels configured for output compare
	   are available on port U pins PU[3:0].

	   When a PWMchannel is enabled, the corresponding pin becomes a PWMoutput.

	   If both PWM and TIM2 are enabled simultaneously, the pin functionality is
	   determined by the configuration of the MODRR bits During reset, port U
	   pins are configured as high-impedance inputs. */



	/* Port U I/O Register (PTU) */
	/* Read: Anytime. Write: Anytime. */
	/* If the associated data direction bit (DDRUx) is set to 1 (output), a read
	   returns the value of the I/O register bit. If the associated data
	   direction bit (DDRUx) is set to 0 (input), a read returns the value of the pin. */

	PTU		=	(0
				/*| PTU_PTU0_MASK*/		// Controlled by PWM-A
				| PTU_PTU1_MASK			// Clear flip-flop A reset (output)
				/*| PTU_PTU2_MASK*/		// Controlled by PWM-B
				| PTU_PTU3_MASK			// Clear flip-flop B reset (output)
				/*| PTU_PTU4_MASK*/		// Not available on 80-pin package
				/*| PTU_PTU5_MASK*/		// Not available on 80-pin package
				/*| PTU_PTU6_MASK*/		// Not available on 80-pin package
				/*| PTU_PTU7_MASK*/		// Not available on 80-pin package
				);



	/* Port U Input Register (PTIU) */
	/* Read: Anytime. Write: Never, writes to this register have no effect. */
	/* This register always reads back the status of the associated pins. */

	// Variable = PTIU;



	/* Port U Data Direction Register (DDRU) */
	/* Read: Anytime. Write: Anytime. */
	/* This register configures port pins PU[7:0] as either input or output.

	   If a pulse width modulator channel is enabled, the associated pin is
	   forced to be an output and the associated Data Direction Register bit
	   has no effect. If the associated pulse width modulator channel is disabled,
	   the corresponding DDRUx bit reverts to control the I/O direction of the
	   associated pin.

	   If the TIM2 module is enabled, each port pin configured for output
	   compare is forced to be an output and the associated Data Direction
	   Register bit has no effect. If the associated timer output compare
	   is disabled, the corresponding DDRUx bit reverts to control the I/O
	   direction of the associated pin.

	   If the TIM2 module is enabled, each port pin configured as an input
	   capture has the corresponding DDRUx bit controlling the I/O direction
	   of the associated pin.

	   When both a timer function and a PWM function are enabled on the same pin,
	   the MODRR register determines which function has control of the pin.
	   0 Associated pin is configured as input.
	   1 Associated pin is configured as output. */

	DDRU	=	(0
				/*| DDRU_DDRU0_MASK*/	// Controlled by PWM-A
				| DDRU_DDRU1_MASK		// Flip-flop A reset (output)
				/*| DDRU_DDRU2_MASK*/	// Controlled by PWM-B
				| DDRU_DDRU3_MASK		// Flip-flop B reset (output)
				/*| DDRU_DDRU4_MASK*/	// Not available on 80-pin package
				/*| DDRU_DDRU5_MASK*/	// Not available on 80-pin package
				/*| DDRU_DDRU6_MASK*/	// Not available on 80-pin package
				/*| DDRU_DDRU7_MASK*/	// Not available on 80-pin package
				);



	/* Port U Reduced Drive Register (RDRU) */
	/* Read: Anytime. Write: Anytime. */
	/* This register configures the drive strength of configured output pins as
	   either full or reduced. If a pin is configured as input, the corresponding
	   Reduced Drive Register bit has no effect.
	   0 Full drive strength at output.
	   1 Associated pin drives at about 1/3 of the full drive strength. */

	RDRU	=	(0
				/*| RDRU_RDRU0_MASK*/
				/*| RDRU_RDRU1_MASK*/
				/*| RDRU_RDRU2_MASK*/
				/*| RDRU_RDRU3_MASK*/
				/*| RDRU_RDRU4_MASK*/
				/*| RDRU_RDRU5_MASK*/
				/*| RDRU_RDRU6_MASK*/
				/*| RDRU_RDRU7_MASK*/
				);



	/* Port U Pull Device Enable Register (PERU) */
	/* Read: Anytime. Write: Anytime. */
	/* This register configures whether a pull-up or a pull-down device is
	   activated on configured input pins. If a pin is configured as output,
	   the corresponding Pull Device Enable Register bit has no effect.
	   0 Pull-up or pull-down device is disabled.
	   1 Pull-up or pull-down device is enabled. */

	PERU	=	(0
				/*| PERU_PERU0_MASK*/
				/*| PERU_PERU1_MASK*/
				/*| PERU_PERU2_MASK*/
				/*| PERU_PERU3_MASK*/
				/*| PERU_PERU4_MASK*/
				/*| PERU_PERU5_MASK*/
				/*| PERU_PERU6_MASK*/
				/*| PERU_PERU7_MASK*/
				);



	/* Port U Polarity Select Register (PPSU) */
	/* Read: Anytime. Write: Anytime. */
	/* The Port U Polarity Select Register selects whether a pull-down or a
	   pull-up device is connected to the pin. The Port U Polarity Select
	   Register is effective only when the corresponding Data Direction Register
	   bit is set to 0 (input) and the corresponding Pull Device Enable Register
	   bit is set to 1.
	   0 A pull-up device is connected to the associated port T pin.
	   1 A pull-down device is connected to the associated port T pin. */

	PPSU	=	(0
				/*| PPSU_PPSU0_MASK*/
				/*| PPSU_PPSU1_MASK*/
				/*| PPSU_PPSU2_MASK*/
				/*| PPSU_PPSU3_MASK*/
				/*| PPSU_PPSU4_MASK*/
				/*| PPSU_PPSU5_MASK*/
				/*| PPSU_PPSU6_MASK*/
				/*| PPSU_PPSU7_MASK*/
				);



	/* Port U Module Routing Register (MODRR) */
	/* Read: Anytime. Write: Anytime. */
	/* This register selects the module connected to port U.
	   0 If enabled, TIM2 channel is connected to the associated port U pin.
	   1 If enabled, PWM channel is connected to the associated port U pin. */

	MODRR	=	(0
				/*| MODRR_MODRR0_MASK*/
				/*| MODRR_MODRR1_MASK*/
				/*| MODRR_MODRR2_MASK*/
				/*| MODRR_MODRR3_MASK*/
				);
//-----------------------------------------------------------------------------
}

/******************************************************************************
  Initialize green LED on the controller board */
void InitLED(void)		// Initialize IO for the LED
{
	DDRE_BIT7 = 1;		// Port E bit 7 is an output
	PORTE_BIT7 = 1;		// LED is off
}

/******************************************************************************
  Set LED on the controller board to ON */
void Set_LED(void)
{
	PORTE_BIT7 = 0;     // Set LED
}

/******************************************************************************
  Set LED on the controller board to OFF */
void Clear_LED(void)
{
	PORTE_BIT7 = 1;     // Clear LED
}

/******************************************************************************
  Toggle LED on the controller board */
void Toggle_LED(void)
{
	PORTE_BIT7 = ~(PORTE_BIT7);  // Toggle LED
}

/******************************************************************************
  Turn on LED backlight on LCD */
void SetLEDBacklight(void)
{
	PTAD_PTAD12 = 0;	// Clear Port AD bit 12
}

/******************************************************************************
  Turn off LED backlight on LCD */
void ClearLEDBacklight(void)
{
	PTAD_PTAD12 = 1;
}

/******************************************************************************
  Turn on AC relay */
void AC_RelayOn(void)
{
	PTM_PTM3 = 1;
}

/******************************************************************************
  Turn off AC relay */
void AC_RelayOff(void)
{
	PTM_PTM3 = 0;
}

/******************************************************************************
  Initialize BlueTooth Module */
void InitBT(void)
{
	PTQ_PTQ0 = 0;		// 0 = Apply power to bluetooth module

	PTQ_PTQ1 = 0;		// 0 = Reset is on
	PTQ_PTQ1 = 1;		// 1 = Reset is off

	PTQ_PTQ3 = 1;		// Start with HIGH and toggle 2 times
	PTQ_PTQ3 = 0;
	PTQ_PTQ3 = 1;
	PTQ_PTQ3 = 0;
	PTQ_PTQ3 = 1;

	PTQ_PTQ2 = 1;		// 0 = Autodiscovery off, 1 = Autodiscovery on
	PTQ_PTQ4 = 0;		// 0 = BT Master off, 1 = BT Master on
	PTQ_PTQ5 = 1;		// 0 = BAUD rate 115.2K, 1 = BAUD rate 9600
}

/******************************************************************************
  Disable BlueTooth Module */
void DisableBT(void)
{
	PTQ_PTQ0 = 1;		// Power off the bluetooth module
}

/******************************************************************************
  Enable BlueTooth Module */
void EnableBT(void)
{
	InitBT();
}


/*****************************************************************************/


