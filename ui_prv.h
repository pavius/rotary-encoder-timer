/**************************************************************************
 *
 * FILE NAME:			ui_prv.h
 * FILE DESCRIPTION:	UI private header
 * FILE CREATION DATE:	12-01-10
 *
 *==========================================================================
 * This document contains confidential information which is protected by copyright 
 * and is proprietary. No part of this document may be used, copied, disclosed, 
 * or conveyed to another party without prior written consent of its owner
 ***************************************************************************
 *
 * Modification history:
 * --------------------
 * 01a,12jan10 erd	written
 *
 ***************************************************************************/

#ifndef __UI_PRV_H_
#define __UI_PRV_H_

#include "ui.h"

// ==========================================================================
// Constants

// how many ticks for button to be pressed down for click to be a long press
#define UI_BUTTON_TICKS_FOR_LONG_PRESS				(200)

// how many ticks to wait from button release to start of next click for click
// to be a double click
#define UI_BUTTON_MAX_TICKS_FOR_SECOND_CLICK		(10)

// beep counters
#define UI_SHORT_BEEP_TICKS							(6)
#define UI_LONG_BEEP_TICKS							(100)

// number of timers
#define UI_TIMER_COUNT								(3)

// how many seconds teh alarm will beep before turns off automatically
#define UI_TIMER_ALARM_SECONDS_AUTO_OFF				(60)

// how often visual indication (toggling) occurs
#define UI_TMR_VISUAL_INDICATION_TICKS				(5)

// 16 bit alarm cadence where each bit is UI_TMR_VISUAL_INDICATION_TICKS long
#define UI_ALARM_CADENCE							(0b0000000011010101)

// button state
typedef enum
{
	UI_BUTSTAT_WAIT_FOR_PRESS_DOWN,					// idle state, waiting for press
	UI_BUTSTAT_WAIT_FOR_PRESS_UP,					// button pressed down, waiting for release
	UI_BUTSTAT_WAIT_FOR_PRESS_UP_AFTER_MOVEMENT,	// button pressed down and movement was detected
	UI_BUTSTAT_WAIT_SECOND_PRESS_DOWN,				// wait to see if there is a double click
	UI_BUTSTAT_WAIT_SECOND_PRESS_UP					// button pressed down, wait for up

} UI_BUTTON_STATE;

// event
typedef enum
{
	// button specific
	UI_EVT_NONE,						// no event
	UI_EVT_SINGLE_LONG_PRESS_HOLDING,	// user is pressing for a long time, still holding
	UI_EVT_SINGLE_LONG_PRESS,			// long press (released)
	UI_EVT_SINGLE_SHORT_PRESS,			// short press (released)
	UI_EVT_DOUBLE_PRESS,				// double click

	// rotary specific
	UI_EVT_MOVEMENT_NO_PRESS,			// dial turning with no press
	UI_EVT_MOVEMENT_PRESS,				// dial turning with

	// 1 second timer
	UI_EVT_ONE_SECOND_TIMER				// one second timer event

} UI_EVENT;

// button state information
typedef struct
{
	UI_BUTTON_STATE 	state;
	UI_EVENT 			event;
	uint_8				statusHistory;
	uint_16				ticksAccumulator;

} UI_BUTTON_DATA;

// UI state
typedef enum
{
	UI_STATE_DISPLAYING_INACTIVE_TIMER,
	UI_STATE_DISPLAYING_ACTIVE_TIMER
		
} UI_STATE;

// timer state
typedef enum
{
	UI_TIMER_STATE_INACTIVE,
	UI_TIMER_STATE_ACTIVE,
	UI_TIMER_STATE_EXPIRED

} UI_TIMER_STATE;

// a timer
typedef struct
{
	int_8 				hoursLeft;
	int_8 				minutesLeft;
	uint_8				secondsLeft;
	UI_TIMER_STATE		state;
	uint_8				alarmSecondsLeft;

} UI_TIMER;

// ==========================================================================
// locals

// current state
UI_STATE ui_currentState;

// action button state
UI_BUTTON_DATA ui_actionButton;

// timers
UI_TIMER ui_timers[UI_TIMER_COUNT];

// the current timer
UI_TIMER *ui_currentTimer;
uint_8 ui_currentTimerIndex;

// timer interval
uint_8 ui_timerInterval;

// ticks left to beep
uint_16 ui_ticksToBeep;

// whether 1 second timer expired
BOOL ui_oneSecondExpired;

// number of expired timers
uint_8 ui_numberOfExpiredTimers;

// alarm visual indication timer
uint_16 ui_tmrAlarmVisualIndication;

// current alarm cadence left (after shifting)
uint_16 ui_currentAlarmCadence;
uint_8 ui_currentAlarmCadenceBitsLeft;

// ==========================================================================
// locals routines

// initialize button state
void ui_buttonDataInit(UI_BUTTON_DATA *buttonData);

// take the current button state, the button history and check if a press 
// event occurred
void ui_checkButtonEvent(BOOL currentStatus, 
						 UI_BUTTON_DATA *buttonData, 
						 BOOL rotaryEncoderMovementDetected,
						 UI_EVENT *event);

// get event
UI_EVENT ui_getRotaryEncoderEvent(int_8 *rotation);

// update a timer with rotations
void ui_modifyTimerValue(UI_TIMER *timer, int_8 rotation);

// display a timer
void ui_displayTimer(UI_TIMER *timer);

// beep
void ui_beep(uint_16 ticksToBeep);

// start beeping
void ui_beepingStart(void);

// stop beeping
void ui_beepingStop(void);

// tick beeper
void ui_beeperTick(void);

// select next timer
void ui_selectTimer(uint_8 timerIndex);

// display which timer is currently selected
void ui_timerLedSet(uint_8 timerIndex, BOOL state);

// toggle specific timer led
void ui_timerLedToggle(uint_8 timerIndex);

// decrement seconds left for all timers and check for expiration
void ui_decrementTimerSeconds(void);

// check if any expired timers need to automatically shut down
void ui_checkAutomaticAlarmShutdown(void);

// timer has expired, set
void ui_handleTimerExpired(UI_TIMER *timer);

// select next timer interval
void ui_nextTimerInterval(void);

// reset the timer value
void ui_resetTimerValue(UI_TIMER *timer);

// get the next timer index by rotation
uint_8 ui_getNextTimerIndexByRotation(int_8 rotation);

// a new alarm has expired
void ui_expiredAlarmsIncrement(void);

// an expired alarm has been acknowledged
void ui_expiredAlarmsDecrement(void);

// toggle expired timer LEDs
void ui_toggleExpiredTimerLeds(void);

// start alarm
void ui_alarmStart(void);

// stop alarm
void ui_alarmStop(void);

// start timer
void ui_startTimer(uint_16 *timer, const uint_16 ticks);

// stop timer
void ui_stopTimer(uint_16 *timer);

// plays out the next stage in teh alarm cadence
void ui_alarmPlayNextCadenceBit(void);

// reload cadence
void ui_alarmCadenceReload(void);

// whether alarm is on
#define ui_alarmOn() (ui_numberOfExpiredTimers != 0)

//
// DisplayingInactiveTimer
// 

// movement with no pressing
void ui_displayingInactiveTimerMovementNoPress(int_8 rotation);

// movement with pressing
void ui_displayingInactiveTimerMovementPress(int_8 rotation);

// 1 second timer
void ui_displayingInactiveTimer1SecondTimer(void);

// short press
void ui_displayingInactiveTimerShortPress(void);

// long press
void ui_displayingInactiveTimerLongPress(void);

// double press
void ui_displayingInactiveTimerDoublePress(void);

// timer expiration
void ui_displayingInactiveTimerHandleTimerExpiration(uint_16 *timer);

//
// DisplayingActiveTimer
// 

// movement with pressing
void ui_displayingActiveTimerMovementPress(int_8 rotation);

// 1 second timer
void ui_displayingActiveTimer1SecondTimer(void);

// short press
void ui_displayingActiveTimerShortPress(void);

// long press
void ui_displayingInactiveTimerLongPress(void);

// double press
void ui_displayingActiveTimerDoublePress(void);

// timer expiration
void ui_displayingActiveTimerHandleTimerExpiration(uint_16 *timer);


#endif /* __UI_PRV_H_ */
