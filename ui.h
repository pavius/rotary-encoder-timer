/**************************************************************************
 *
 * FILE NAME:            ui_prv.h
 * FILE DESCRIPTION:    UI private header
 * FILE CREATION DATE:    12-01-10
 *
 *==========================================================================
 * This document contains confidential information which is protected by copyright 
 * and is proprietary. No part of this document may be used, copied, disclosed, 
 * or conveyed to another party without prior written consent of its owner
 ***************************************************************************
 *
 * Modification history:
 * --------------------
 * 01a,12jan10 erd    written
 *
 ***************************************************************************/

#ifndef __UI_H_
#define __UI_H_

#include "sysdef.h"

// init ui
extern void ui_init(void);

// return number of main() cycles between calls to tick()
#define ui_getCyclesPerTick() (2)

// tick of UI module
extern void ui_tick(void);

// 1 second timer expiration, called from interrupt
void ui_oneSecondTimerExpiration(void);

#endif /* __UI_H_ */
