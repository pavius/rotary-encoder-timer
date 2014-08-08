/**************************************************************************
 *
 * FILE NAME:			quadenc.h
 * FILE DESCRIPTION:	QUADENC header
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

#ifndef __QUADENC_H_
#define __QUADENC_H_

// ==========================================================================
// interfaces

// initialize
void quadenc_init(void);

// get last number of counts
void quadenc_getLastChangeCount(int_8 *counts);

// ISR
void quadenc_isr(void);

#endif /* __QUADENC_H_ */
