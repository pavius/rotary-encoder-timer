/**************************************************************************
 *
 * FILE NAME:            sseg_prv.h
 * FILE DESCRIPTION:    SSEG private header
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

#ifndef __SSEG_PRV_H_
#define __SSEG_PRV_H_

#include "sseg.h"

// ==========================================================================
// constants

// port bit assignments for segment
#define SSEG_SEGMENT_A     (1 << 3)
#define SSEG_SEGMENT_B     (1 << 2)
#define SSEG_SEGMENT_C     (1 << 1)
#define SSEG_SEGMENT_D     (1 << 4)
#define SSEG_SEGMENT_E     (1 << 7)
#define SSEG_SEGMENT_F     (1 << 0)
#define SSEG_SEGMENT_G     (1 << 5)
#define SSEG_DECPT        (1 << 6)

// digit values
const uint_8 sseg_digitLedMap[] = 
{
    /* 0 */ ~(SSEG_SEGMENT_A | SSEG_SEGMENT_B | SSEG_SEGMENT_C | SSEG_SEGMENT_D | SSEG_SEGMENT_E | SSEG_SEGMENT_F),
    /* 1 */ ~(SSEG_SEGMENT_B | SSEG_SEGMENT_C),
    /* 2 */ ~(SSEG_SEGMENT_A | SSEG_SEGMENT_B | SSEG_SEGMENT_D | SSEG_SEGMENT_E | SSEG_SEGMENT_G),
    /* 3 */ ~(SSEG_SEGMENT_A | SSEG_SEGMENT_B | SSEG_SEGMENT_C | SSEG_SEGMENT_D | SSEG_SEGMENT_G),
    /* 4 */ ~(SSEG_SEGMENT_B | SSEG_SEGMENT_C | SSEG_SEGMENT_F | SSEG_SEGMENT_G),
    /* 5 */ ~(SSEG_SEGMENT_A | SSEG_SEGMENT_C | SSEG_SEGMENT_D | SSEG_SEGMENT_F | SSEG_SEGMENT_G),
    /* 6 */ ~(SSEG_SEGMENT_A | SSEG_SEGMENT_C | SSEG_SEGMENT_D | SSEG_SEGMENT_E | SSEG_SEGMENT_F | SSEG_SEGMENT_G),
    /* 7 */ ~(SSEG_SEGMENT_A | SSEG_SEGMENT_B | SSEG_SEGMENT_C),
    /* 8 */ ~(SSEG_SEGMENT_A | SSEG_SEGMENT_B | SSEG_SEGMENT_C | SSEG_SEGMENT_D | SSEG_SEGMENT_E | SSEG_SEGMENT_F | SSEG_SEGMENT_G),
    /* 9 */ ~(SSEG_SEGMENT_A | SSEG_SEGMENT_B | SSEG_SEGMENT_C | SSEG_SEGMENT_D | SSEG_SEGMENT_F | SSEG_SEGMENT_G),
};
// ==========================================================================
// locals

// digit value array
/*static*/ uint_8 sseg_digitValues[4];

// current decpt state
static uint_8 sseg_decptState;

// current digit
static uint_8 sseg_currentDigit = 0;

#endif /* __SSEG_PRV_H_ */
