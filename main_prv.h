/**************************************************************************
 *
 * FILE NAME:            main_prv.h
 * FILE DESCRIPTION:    main private header
 * FILE CREATION DATE:    9-01-2010
 *
 *==========================================================================
 * This document contains confidential information which is protected by
 * copyright and is proprietary to NIR Diagnostics Inc., Ontario, Canada. No part
 * of this document may be used, copied, disclosed, or conveyed to another
 * party without prior written consent of NIR Diagnostics Inc., Ontario, Canada.
 ***************************************************************************
 *
 * Modification history:
 * --------------------
 * 01a,9jan10 erd    written
 *
 ***************************************************************************/

#ifndef __MAIN_PRV_H_
#define __MAIN_PRV_H_

#include "main.h"

// ==========================================================================
// Constants

// submodules
typedef enum
{
    TIMER_SM_UI,                // user interface

    // MUST be last: the number of submodules which require periodic 'tick's
    TIMER_SM_COUNT

} TIMER_SUBMODULES;

// ==========================================================================
// Variables

// this array holds counters for each submodule - when the counter for the sub
// module will reach zero, the submodule's tick() routine will be called
static uint_16 timer_moduleTickLeft[TIMER_SM_COUNT];

// uS passed for 1 second timer
static uint_32 timer_oneSecondCorrection;

#endif /* __MAIN_PRV_H_ */
