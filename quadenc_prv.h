/**************************************************************************
 *
 * FILE NAME:			quadenc_prv.h
 * FILE DESCRIPTION:	QUADENC private header
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

#ifndef __QUADENC_PRV_H_
#define __QUADENC_PRV_H_

#include "quadenc.h"

// ==========================================================================
// constants

// states
typedef enum
{
	QUADENC_STATE_IDLE,
	QUADENC_STATE_DETECTING_CW,
	QUADENC_STATE_DETECTING_CCW,

} QUADENC_STATE;

// clear transition event
#define QUADENC_NO_TRANSITION_EVENT (1)

// expected transitions
const uint_8 quadenc_cwTransitions[] 	= {8, 0, 15, 7};
const uint_8 quadenc_ccwTransitions[] 	= {0, 8, 7, 15};

// ==========================================================================
// locals

// transition event
uint_8 quadenc_transitionEvent = QUADENC_NO_TRANSITION_EVENT;

// search patterns
uint_8 quadenc_channel0SearchPattern;
uint_8 quadenc_channel1SearchPattern;

// channel shift registers
uint_8 quadenc_channel0Status;
uint_8 quadenc_channel1Status;
uint_8 quadenc_buttonStatus;

// the current state
QUADENC_STATE quadenc_state;
uint_8 quadenc_transitionEventIndex;

// change counts
int_8 quadenc_changeCounts;

#endif /* __QUADENC_PRV_H_ */
