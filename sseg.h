/**************************************************************************
 *
 * FILE NAME:            sseg.h
 * FILE DESCRIPTION:    SSEG header
 * FILE CREATION DATE:    9-01-2010
 *
 *==========================================================================
 * This document contains confidential information which is protected by copyright 
 * and is proprietary. No part of this document may be used, copied, disclosed, 
 * or conveyed to another party without prior written consent of its owner
 ***************************************************************************
 *
 * Modification history:
 * --------------------
 * 01a,9jan10 erd    written
 *
 ***************************************************************************/

#ifndef __SSEG_H_
#define __SSEG_H_

// ==========================================================================
// interfaces

// init
void sseg_init(void);

// start displaying
void sseg_startDisplay(void);

// set the number on the display
void sseg_setDisplay(uint_16 numberToDisplay);

// set dots (currently support only on or off)
void sseg_setDecPtStates(BOOL state);

// toggle dots (currently support only on or off)
void sseg_toggleDecPtStates(void);

// seven segment ISR
void sseg_isr(void);

#endif /* __SSEG_H_ */
