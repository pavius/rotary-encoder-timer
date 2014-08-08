/**************************************************************************
 *
 * FILE NAME:            quadenc.c
 * FILE DESCRIPTION:    QUADENC driver
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

#include "sysdef.h"
#include "sys.h"
#include "quadenc_prv.h"

// ==========================================================================
// init

void quadenc_init(void)
{
    // init locals
    quadenc_channel0SearchPattern = 0;
    quadenc_channel1SearchPattern = 0;
    quadenc_channel0Status = 0xFF;
    quadenc_channel1Status = 0xFF;
    quadenc_buttonStatus = 0xFF;
    quadenc_state = QUADENC_STATE_IDLE;
    quadenc_transitionEventIndex = 0;
    quadenc_changeCounts = 0;
}
// ==========================================================================
// get last number of counts

void quadenc_getLastChangeCount(int_8 *counts)
{
    // disable interrupts
    // sys_disableInterrupts();

    // get how many counts changed since last poll
    *counts += quadenc_changeCounts;

    // zero it out
    quadenc_changeCounts = 0;

    // enable interrupts
    // sys_enableInterrupts();
}
// ==========================================================================
// ISR

void quadenc_isr(void)
{
    //
    // Debounce channel 0
    //

    // shift channel0 value into status. Save only N bits history
    quadenc_channel0Status = ((quadenc_channel0Status << 1) | RB0) & 0x7;

    // do lower 3 bits match search pattern?
    if (quadenc_channel0Status == quadenc_channel0SearchPattern)
    {
        // change search pattern
        quadenc_channel0SearchPattern = ((~quadenc_channel0SearchPattern) & 0x7);

        // declare debounced transition
        quadenc_transitionEvent = quadenc_channel0Status;
    }

    //
    // Debounce channel 1
    //

    // shift channel0 value into status. Save only N bits history
    quadenc_channel1Status = ((quadenc_channel1Status << 1) | RB1) & 0x7;

    // do lower 3 bits match search pattern?
    if (quadenc_channel1Status == quadenc_channel1SearchPattern)
    {
        // change search pattern
        quadenc_channel1SearchPattern = ((~quadenc_channel1SearchPattern) & 0x7);

        // declare debounced transition
        quadenc_transitionEvent = (quadenc_channel1Status | (1 << 3));
    }

    //
    // Check if transition forms an acceptable movement
    //

    // check if a transition occured
    if (quadenc_transitionEvent != QUADENC_NO_TRANSITION_EVENT)
    {
        // 
        // Idle state
        // 
        if (quadenc_state == QUADENC_STATE_IDLE)
        {
            // first value of CW transition?
            if (quadenc_transitionEvent == quadenc_cwTransitions[0])
            {
                // yup - enter state
                quadenc_state = QUADENC_STATE_DETECTING_CW;

                // next transition
                quadenc_transitionEventIndex++;
            }
            // first value of CCW transition?
            else if (quadenc_transitionEvent == quadenc_ccwTransitions[0])
            {
                // yup - enter state
                quadenc_state = QUADENC_STATE_DETECTING_CCW;

                // next transition
                quadenc_transitionEventIndex++;
            }
        }
        //
        // CW state
        // 
        else if (quadenc_state == QUADENC_STATE_DETECTING_CW)
        {
            // found next expected transition?
            if (quadenc_transitionEvent == quadenc_cwTransitions[quadenc_transitionEventIndex])
            {
                // increment it
                quadenc_transitionEventIndex++;

                // are we done?
                if (quadenc_transitionEventIndex == 4)
                {
                    // decrement count
                    quadenc_changeCounts++;

                    // back to idle
                    quadenc_state = QUADENC_STATE_IDLE;
                    quadenc_transitionEventIndex = 0;
                }
            }
            else
            {
                // incorrect transition, back to idle
                quadenc_state = QUADENC_STATE_IDLE;
                quadenc_transitionEventIndex = 0;
            }
        }
        //
        // CCW state
        // 
        else if (quadenc_state == QUADENC_STATE_DETECTING_CCW)
        {
            // found next expected transition?
            if (quadenc_transitionEvent == quadenc_ccwTransitions[quadenc_transitionEventIndex])
            {
                // increment it
                quadenc_transitionEventIndex++;

                // are we done?
                if (quadenc_transitionEventIndex == 4)
                {
                    // increment count
                    quadenc_changeCounts--;

                    // back to idle
                    quadenc_state = QUADENC_STATE_IDLE;
                    quadenc_transitionEventIndex = 0;
                }
            }
            else
            {
                // incorrect transition, back to idle
                quadenc_state = QUADENC_STATE_IDLE;
                quadenc_transitionEventIndex = 0;
            }
        }

        // zero out transition
        quadenc_transitionEvent = QUADENC_NO_TRANSITION_EVENT;
    }
}
