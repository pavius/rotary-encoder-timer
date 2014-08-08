/**************************************************************************
 *
 * FILE NAME:			types.h
 * FILE DESCRIPTION:	Extended types header
 *
 * FILE CREATION DATE:	24-05-2004
 *
 *==========================================================================
 *           Copyright (c) 2004 NIR Diagnostics Inc., Ontario, Canada
 *
 * This document contains confidential information which is protected by
 * copyright and is proprietary to NIR Diagnostics Inc., Ontario, Canada. No part
 * of this document may be used, copied, disclosed, or conveyed to another
 * party without prior written consent of NIR Diagnostics Inc., Ontario, Canada.
 ***************************************************************************
 *
 * Modification history:
 * --------------------
 * 01a,24may03 erd	written
 *
 ***************************************************************************/

#ifndef __TYPES_H_
#define __TYPES_H_

// basic types
typedef signed char 		int_8;
typedef signed int 			int_16;
typedef unsigned char 		uint_8;
typedef unsigned int 		uint_16;
typedef unsigned long 		uint_32;

// boolean type
#define FALSE				(0)
#define TRUE				(1)
typedef uint_8				BOOL;
#define NULL				(0)

#endif /* __TYPES_H_ */
