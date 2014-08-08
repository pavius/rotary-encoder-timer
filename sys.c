/**************************************************************************
 *
 * FILE NAME:            system.h
 * FILE DESCRIPTION:    DLS specific system definitions
 *
 * FILE CREATION DATE:    24-05-2004
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
 * 01a,24may03 erd    written
 *
 ***************************************************************************/

#include "sys_prv.h"

// ==========================================================================
// sys_init
//
// Initializes all I/O pinouts
//

BOOL sys_init()
{
    // disable interrupts
    sys_bitClear(GIE);
    
    // disable A2D and comparators
    ADCON1 = 0b0001111;
    CMCON = 0;

    // 
    // Set GPIO port directions
    //
    
    sys_setIoDirection(IO_LED_TMR0,         IO_PIN_OUTPUT);
    sys_setIoDirection(IO_LED_TMR1,         IO_PIN_OUTPUT);
    sys_setIoDirection(IO_LED_TMR2,         IO_PIN_OUTPUT);
    sys_setIoDirection(IO_ROTENC_CH0,         IO_PIN_INPUT);
    sys_setIoDirection(IO_ROTENC_CH1,         IO_PIN_INPUT);
    sys_setIoDirection(IO_BUTTON,             IO_PIN_INPUT);
    sys_setIoDirection(IO_BUZZER,             IO_PIN_OUTPUT);
    
    // 
    // Initialize port values
    //

    sys_bitClear(IO_LED_TMR0);
    sys_bitClear(IO_LED_TMR1);
    sys_bitClear(IO_LED_TMR2);
    sys_bitClear(IO_BUZZER);

    // zero out seven segment ports
    IO_SSEG_SELECT = 0;
    IO_SSEG_DISPLAY = 0xFF;

    // set port values
    IO_SSEG_SELECT_DIR = 0x0;    // output                
    IO_SSEG_DISPLAY_DIR = 0x0;    // output

    // enable interrupts
    sys_bitSet(GIE);
    sys_bitSet(PEIE);

    // success
    return TRUE;
}



