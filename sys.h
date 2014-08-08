/**************************************************************************
 *
 * FILE NAME:            system.h
 * FILE DESCRIPTION:    System definitions (board settings)
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

#ifndef __SYSTEM_H_
#define __SYSTEM_H_

#include "sysdef.h"

// ==========================================================================
// system constants

#define SYS_CLK_RATE                (40000000L)        // clock rate (10Mhz)

// compilation directives
#define SYS_LOADED_FROM_ICD            (0)

// ==========================================================================
// I/O pinout

// LEDs
#define IO_LED_TMR0                    (RB3)    // First timer
#define IO_LED_TMR1                    (RB4)    // Second timer
#define IO_LED_TMR2                    (RB5)    // Third timer

// rotary encoder
#define IO_ROTENC_CH0                (RB0)    // Rotary encoder channel 0
#define IO_ROTENC_CH1                (RB1)    // Rotary encoder channel 1

// push button
#define IO_BUTTON                    (RB2)    // Rotary encoder button

// piezzo buzzer
#define IO_BUZZER                    (RA5)

// seven segment 
#define IO_SSEG_SELECT                (PORTA)    // transistor control
#define IO_SSEG_DISPLAY                (PORTC) // segment control

// ==========================================================================
// I/O direction

#define IO_LED_TMR0_DIR                (TRISB3)
#define IO_LED_TMR1_DIR                (TRISB4)
#define IO_LED_TMR2_DIR                (TRISB5)
#define IO_ROTENC_CH0_DIR            (TRISB0)
#define IO_ROTENC_CH1_DIR            (TRISB1)
#define IO_BUTTON_DIR                (TRISB2)
#define IO_SSEG_SELECT_DIR            (TRISA)
#define IO_SSEG_DISPLAY_DIR            (TRISC)
#define IO_BUZZER_DIR                (TRISA6)

// ==========================================================================
// Constants and types

#define IO_PIN_OUTPUT                (0)
#define IO_PIN_INPUT                (1)

// ==========================================================================
// External interface

// set a direction register used in initialization procedure. Since the data 
// sheet says bcf/bsfs cannot be called consecutively, we'll do two NOPs between
// each set. 
#define sys_setIoDirection(pin, direction)                                    \
{                                                                            \
    pin##_DIR = direction;                                                    \
    asm("nop");                                                                \
    asm("nop");                                                                \
}

// sys_init
//
// Initializes all I/O pinouts
//

extern BOOL sys_init(void);

// sys_disableInterrupts
//
// Masks all interrupts
//

#define sys_disableInterrupts() while (GIE) GIE = 0

// sys_disableInterrupts
//
// Unmasks all interrupts
//

#define sys_enableInterrupts() (GIE = 1)

#endif /* __SYSTEM_H_ */
