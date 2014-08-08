/**************************************************************************
 *
 * FILE NAME:            main.c
 * FILE DESCRIPTION:    Main module
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

#include "main_prv.h"
#include "sseg.h"
#include "quadenc.h"
#include "ui.h"

// ==========================================================================
// timer_checkTickRequired(moduleId, moduleName)
//
// Macro which checks the amount of cycles which are left before a tick is 
// required for a submodule. If the cycle count reaches zero, the xxx_tick()
// routine for the sm is called, and the cycles is reset to the sm's cycle 
// per tick
//
// Arguments: 
//        moduleId: effectively holds the position of the sm in moduleTickLeft[]
//        moduleName: the prefix for the xxx_tick() routine
//    

#define timer_checkTickRequired(moduleId, moduleName)                            \
{                                                                                \
    if (--timer_moduleTickLeft[moduleId] == 0)                                    \
    {                                                                            \
        moduleName##_tick();                                                    \
        timer_moduleTickLeft[moduleId] = moduleName##_getCyclesPerTick();        \
    }                                                                            \
}

// ==========================================================================

void timer_init(void)
{
    // initialize pic env
    sys_init();

    // init seven seg
    sseg_init();

    // start displaying
    sseg_startDisplay();

    // init quadrature encoder
    quadenc_init();

    // init
    ui_init();

    //
    // Initialize tick dispatcher
    //

    // initialize the cycles per tick array
    timer_moduleTickLeft[TIMER_SM_UI]                 = ui_getCyclesPerTick();

    // init locals
    timer_oneSecondCorrection = 0;
}
// ==========================================================================
// check if one second has elapsed

void timer_oneSecondEventCheck()
{
    // increment by number of uS passed since last call
    timer_oneSecondCorrection += 6400;

    // check if passed 1 second
    if (timer_oneSecondCorrection >= 10000000)
    {
        // remove 1 second and retain error
        timer_oneSecondCorrection -= 10000000;

        // raise event
        ui_oneSecondTimerExpiration();
    }
}
// ==========================================================================
// interrupt

void interrupt timer_isrHigh()
{
    // check tmr2
    if (TMR2IF)
    {
        // clear the interrupt
        TMR2IF = 0;

        // check if one second has elapsed
        timer_oneSecondEventCheck();

        // call quadrature encoder
        quadenc_isr();

        // call segment display
        sseg_isr();
    }
}
// ==========================================================================

void main(void)
{
    // initialize
    timer_init();

    // tick dispatching
    // the main loop, each sub-module's tick() will be called according to how
    // many cycles need to pass between each tick
    while (1)
    {
        // check if we need to call _tick interface
        //                         module id                sub-module prefix to call xxx_tick() and xxx_getCyclesPerTick
        timer_checkTickRequired(TIMER_SM_UI,             ui);

        // wait 5 msec
        time_delayMs(5);
    }
}
// ==========================================================================

