/**************************************************************************
 *
 * FILE NAME:			common.h
 * FILE DESCRIPTION:	Common routines and macros
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

#ifndef __COMMON_H_
#define __COMMON_H_

#include <stdio.h>

//
// time routines 
//

//
// time routines 
//

// delay one microsecond
#define time_delayOneUs() {asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");}

// declare delay counter
extern unsigned char time_delayUsLeft;

// delay one uS
#define time_delayUs(x) 							\
{													\
	time_delayUsLeft = (unsigned char)(x);			\
	asm("movlb (_time_delayUsLeft) >> 8");			\
	time_delayOneUs();								\
	asm("decfsz (_time_delayUsLeft)&0ffh, f");		\
	asm("goto $ - 16");								\
}

// delay one mS
//
// Arguments:
//	ms: the amount of miliseconds to delay
// 
extern void time_delayMs(int_16 ms);

//
// bit definitions
//

#define SYS_BIT0			(1 << 0)
#define SYS_BIT1			(1 << 1)
#define SYS_BIT2			(1 << 2)
#define SYS_BIT3			(1 << 3)
#define SYS_BIT4			(1 << 4)
#define SYS_BIT5			(1 << 5)
#define SYS_BIT6			(1 << 6)
#define SYS_BIT7			(1 << 7)

//
// bit routines
//

// set / clear bits
#define sys_bitSet(bit)						(bit = 1)
#define sys_bitClear(bit)					(bit = 0)

// clear a bit and wait for it to set
#define sys_bitClearWaitSet(bit) 			{sys_bitClear(bit); while(!bit);}
#define sys_bitSetWaitClear(bit) 			{sys_bitSet(bit); while(bit);}

// pulse a bit
#define sys_bitSetNoWaitClear(bit)			{sys_bitSet(bit); sys_bitClear(bit);}
#define sys_bitSetWaitUsClear(bit, delay)	{sys_bitSet(bit); time_delayUs(delay); sys_bitClear(bit);}
#define sys_bitSetWaitMsClear(bit, delay)	{sys_bitSet(bit); time_delayMs(delay); sys_bitClear(bit);}
#define sys_bitClearNoWaitSet(bit)			{sys_bitClear(bit); sys_bitSet(bit);}
#define sys_bitClearWaitUsSet(bit, delay)	{sys_bitClear(bit); time_delayUs(delay); sys_bitSet(bit);}
#define sys_bitClearWaitMsSet(bit, delay)	{sys_bitClear(bit); time_delayMs(delay); sys_bitSet(bit);}

// masking routines
#define sys_getIntMsb(myInt)				((myInt >> 8) & 0xFF)	// get high byte from an int
#define sys_getIntLsb(myInt)				((myInt) & 0xFF)		// get low byte from an int


//
// Generic routines
// 

// uint_8 calculateChecksum(uint_8 *buffer, uint_8 size)
//
// Caluclates an 8bit checksum over contents of buffer
// 
// Arguments:
//	buffer: ptr to start of buffer on which chksum is calculated over
//	size: size, in bytes, of buffer
//
// Return value:
//	checksum value
//
uint_8 sys_calculateChecksum(uint_8 *buffer, uint_8 size);


// uint_16 calculateChecksum16(uint_8 *buffer, uint_16 size)
//
// Caluclates an 16bit checksum over contents of buffer
// 
// Arguments:
//	buffer: ptr to start of buffer on which chksum is calculated over
//	size: size, in bytes, of buffer
//
// Return value:
//	checksum value
//

uint_16 sys_calculateChecksum16(uint_8 *buffer, uint_16 size);

#endif /* __COMMON_H_ */

