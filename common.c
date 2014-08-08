/**************************************************************************
 *
 * FILE NAME:			common.c
 * FILE DESCRIPTION:	Common routines
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

#include "sysdef.h"
#include "common.h"
#include "sys.h"

// for time delays
unsigned char time_delayUsLeft;

// ==========================================================================
// uint_8 sys_calculateChecksum(uint_8 *buffer, uint_8 size)
//
// Caluclates an 1's complement checksum over contents of buffer
// 
// Arguments:
//	buffer: ptr to start of buffer on which chksum is calculated over
//	size: size, in bytes, of buffer
//
// Return value:
//	checksum value
//
uint_8 sys_calculateChecksum(uint_8 *buffer, uint_8 size)
{
	uint_8 chksum = 0;

	// calculate the checksum over the buffer
	while (size--) 
	{
		chksum += *buffer;
		buffer++;
	}

	// return the checksum
	return (~chksum + 1);
}
// ==========================================================================
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

uint_16 sys_calculateChecksum16(uint_8 *buffer, uint_16 size)
{
	uint_16 chksum = 0;

	// calculate the checksum over the buffer
	while (size--) 
	{
		chksum += *buffer;
		buffer++;
	}

	// return the checksum
	return (~chksum + 1);
}

// ==========================================================================
// void time_delayMs(int_16 ms)
//
// delay one mS
//
// Arguments:
//	ms: the amount of miliseconds to delay
// 

void time_delayMs(int_16 ms)
{
	uint_8 i;

	do 
	{
		// reset i
		i = 4;
		
		do 
		{
			// delay a quarter of a ms
			time_delayUs(250);
			CLRWDT();
		
		} while(--i);
	

	} while(--ms);
}

