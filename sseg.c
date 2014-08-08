/**************************************************************************
 *
 * FILE NAME:			sseg.c
 * FILE DESCRIPTION:	SSEG driver
 * FILE CREATION DATE:	9-01-2010
 *
 *==========================================================================
 * This document contains confidential information which is protected by copyright 
 * and is proprietary. No part of this document may be used, copied, disclosed, 
 * or conveyed to another party without prior written consent of its owner
 ***************************************************************************
 *
 * Modification history:
 * --------------------
 * 01a,9jan10 erd	written
 *
 ***************************************************************************/

#include "sysdef.h"
#include "sys.h"
#include "sseg_prv.h"

// ==========================================================================
// init seven segment

void sseg_init()
{
	// 1:16 and 1:16 scalars
	T2CON = 0b01111011;
	
	// set period (every 600uS)
	PR2 = 24;
	
	// enable interrupts
	TMR2IE = 1; 

	// init decpt state
	sseg_decptState = FALSE;
}	
// ==========================================================================
// start displaying

void sseg_startDisplay(void)
{
	// start timer2
	TMR2ON = 1;
}
// ==========================================================================
// set the number on the display

void sseg_setDisplay(uint_16 numberToDisplay)
{
	// init values
	uint_8 thousands, hundreds, tens, units;
	thousands = hundreds = tens = units = 0;

	// split number
	while (numberToDisplay >= 1000){thousands++; numberToDisplay -= 1000;}
	while (numberToDisplay >= 100){hundreds++; numberToDisplay -= 100;}
	while (numberToDisplay >= 10){tens++; numberToDisplay -= 10;}
	units = numberToDisplay;

	// set to display array
	sseg_digitValues[3] = sseg_digitLedMap[units];
	sseg_digitValues[2] = sseg_digitLedMap[tens];
	sseg_digitValues[1] = sseg_digitLedMap[hundreds];
	sseg_digitValues[0] = sseg_digitLedMap[thousands];

	// set decpt
	sseg_setDecPtStates(sseg_decptState);
}	
// ==========================================================================
// seven segment ISR

void sseg_isr(void)
{
	uint_8 previousSelectReg;
	
	// get previous
	previousSelectReg = (IO_SSEG_SELECT & 0xF0);

	// update new select
	previousSelectReg |= (1 << sseg_currentDigit);

	// select current digit
	IO_SSEG_SELECT = previousSelectReg;

	// load current digit
	IO_SSEG_DISPLAY = sseg_digitValues[sseg_currentDigit];

	// next digit
	sseg_currentDigit++;

	// and wrap
	if (sseg_currentDigit >= 4) sseg_currentDigit = 0;
}
// ==========================================================================
// set dots (currently support only on or off)

void sseg_setDecPtStates(BOOL state)
{
	// save state
	sseg_decptState = state;
	
	// set decpt
	if (state)
	{
		// clear appropriate bit
		sseg_digitValues[0] &= ~SSEG_DECPT;
		sseg_digitValues[1] &= ~SSEG_DECPT;
		sseg_digitValues[2] &= ~SSEG_DECPT;
		sseg_digitValues[3] &= ~SSEG_DECPT;
	}
	// clear decpt
	else
	{
		// set appropriate bit
		sseg_digitValues[0] |= SSEG_DECPT;
		sseg_digitValues[1] |= SSEG_DECPT;
		sseg_digitValues[2] |= SSEG_DECPT;
		sseg_digitValues[3] |= SSEG_DECPT;
	}
}
// ==========================================================================
// toggle dots (currently support only on or off)

void sseg_toggleDecPtStates(void)
{
	// toggle values
    sseg_decptState = !sseg_decptState;

	// set
	sseg_setDecPtStates(sseg_decptState);
}
// ==========================================================================

