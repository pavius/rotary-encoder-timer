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

#include "ui_prv.h"
#include "sys.h"
#include "sseg.h"
#include "quadenc.h"

// ==========================================================================
// init ui

void ui_init(void)
{
	// timer index
	uint_8 timerIdx;

	// init the action state
	ui_buttonDataInit(&ui_actionButton);
	
	// set ui state
	ui_currentState = UI_STATE_DISPLAYING_INACTIVE_TIMER;

	// iterate over all timers and init them
	for (timerIdx = 0; timerIdx < UI_TIMER_COUNT; ++timerIdx)
	{
		// get timer pointer
		ui_timers[ui_currentTimerIndex].hoursLeft = 0;
		ui_timers[ui_currentTimerIndex].minutesLeft = 0;
		ui_timers[ui_currentTimerIndex].secondsLeft = 0;
		ui_timers[ui_currentTimerIndex].state = UI_TIMER_STATE_INACTIVE;
	}

	// set last timer so that next timer call will init to first
	ui_selectTimer(0);

	// first interval
	ui_timerInterval = 15;

	// init ticks to beep
	ui_ticksToBeep = 0;

	// 1sec timer flag
	ui_oneSecondExpired = FALSE;

	// show points
	sseg_setDecPtStates(TRUE);

	// number of expired timers
	ui_numberOfExpiredTimers = 0;

	// init timers
	ui_tmrAlarmVisualIndication = 0;

	// zero out display
	sseg_setDisplay(0);
}
// ==========================================================================
// handle an event

void ui_handleEvent(UI_EVENT event, int_8 rotation)
{
	// according to event
	switch (event)
	{
		// movement with no pressing
		case UI_EVT_MOVEMENT_NO_PRESS:
		{
			// according to state
			switch (ui_currentState)
			{
				case UI_STATE_DISPLAYING_INACTIVE_TIMER:  ui_displayingInactiveTimerMovementNoPress(rotation); break;
			}
		}
		break;

		// movement with pressing
		case UI_EVT_MOVEMENT_PRESS:
		{
			// according to state
			switch (ui_currentState)
			{
				case UI_STATE_DISPLAYING_INACTIVE_TIMER:  	ui_displayingInactiveTimerMovementPress(rotation); break;
				case UI_STATE_DISPLAYING_ACTIVE_TIMER:  	ui_displayingActiveTimerMovementPress(rotation); break;
			}
		}
		break;

		// 1 second timer
		case UI_EVT_ONE_SECOND_TIMER:
		{
			// according to state
			switch (ui_currentState)
			{
				case UI_STATE_DISPLAYING_INACTIVE_TIMER:  	ui_displayingInactiveTimer1SecondTimer(); break;
				case UI_STATE_DISPLAYING_ACTIVE_TIMER:  	ui_displayingActiveTimer1SecondTimer(); break;
			}
		}
		break;

		// short press
		case UI_EVT_SINGLE_SHORT_PRESS:
		{
			// according to state
			switch (ui_currentState)
			{
				case UI_STATE_DISPLAYING_INACTIVE_TIMER:  	ui_displayingInactiveTimerShortPress(); break;
				case UI_STATE_DISPLAYING_ACTIVE_TIMER:  	ui_displayingActiveTimerShortPress(); break;
			}
		}
		break;

		// long press
		case UI_EVT_SINGLE_LONG_PRESS_HOLDING:
		{
			// according to state
			switch (ui_currentState)
			{
				case UI_STATE_DISPLAYING_INACTIVE_TIMER:  ui_displayingInactiveTimerLongPress(); break;
			}
		}
		break;

		// double click
		case UI_EVT_DOUBLE_PRESS:
		{
			// according to state
			switch (ui_currentState)
			{
				case UI_STATE_DISPLAYING_INACTIVE_TIMER:  	ui_displayingInactiveTimerDoublePress(); break;
				case UI_STATE_DISPLAYING_ACTIVE_TIMER:  	ui_displayingActiveTimerDoublePress(); break;
			}
		}
		break;
	}
}
// ==========================================================================
// tick of UI module

void ui_tick(void)
{
	// rotation
	int_8 rotation = 0;

	// event
	UI_EVENT event;
	
	// tick beeper (check if need to stop beeper)
	ui_beeperTick();

	//
	// check any timers expired
	// 

	// check if timer 1 active
	if (ui_tmrAlarmVisualIndication != 0)
	{
		// check if timer expired
		if (--ui_tmrAlarmVisualIndication == 0)
		{
			// according to state
			switch (ui_currentState)
			{
				case UI_STATE_DISPLAYING_INACTIVE_TIMER:			ui_displayingInactiveTimerHandleTimerExpiration(&ui_tmrAlarmVisualIndication); 			break;
				case UI_STATE_DISPLAYING_ACTIVE_TIMER:				ui_displayingActiveTimerHandleTimerExpiration(&ui_tmrAlarmVisualIndication); 			break;
			}
		}
	}

	//
	// Check if accurate 1 second timer expired
	//

	// check if 1second timer expired
	if (ui_oneSecondExpired)
	{
		// handle 1 second expiration
		ui_handleEvent(UI_EVT_ONE_SECOND_TIMER, 0);

		// clear it
		ui_oneSecondExpired = FALSE;
	}

	//
	// Check if any buttons pressed or rotation occured
	//

	// get event
	event = ui_getRotaryEncoderEvent(&rotation);

	// handle the event
	ui_handleEvent(event, rotation);
}
// ==========================================================================
// 1 second timer expiration, called from interrupt

void ui_oneSecondTimerExpiration(void)
{
	// set 1 second expiration
	ui_oneSecondExpired = TRUE;
}
// ==========================================================================
// get event

UI_EVENT ui_getRotaryEncoderEvent(int_8 *rotation)
{
	UI_EVENT event;

	// get counts of movement
	quadenc_getLastChangeCount(rotation);

	// check button event. movement will mask any event
	ui_checkButtonEvent(IO_BUTTON, &ui_actionButton, *rotation != 0, &event);

	// check if there is any movement
	if (*rotation)
	{
		// set event
		event = IO_BUTTON == 0 ? UI_EVT_MOVEMENT_PRESS : UI_EVT_MOVEMENT_NO_PRESS;
	}

	// return teh event
	return event;
}
// ==========================================================================
// take the current button state, the button history and check if a press 
// event occurred

void ui_checkButtonEvent(BOOL currentStatus, 
						 UI_BUTTON_DATA *buttonData, 
						 BOOL rotaryEncoderMovementDetected, 
						 UI_EVENT *event)
{
	// no event
	*event = UI_EVT_NONE;

	// read current status into history, remember 3 bits (ticks) back
	buttonData->statusHistory = (((buttonData->statusHistory << 1) | currentStatus) & 0x07);

	// 
	// Waiting for press down
	// 
	if (buttonData->state == UI_BUTSTAT_WAIT_FOR_PRESS_DOWN && buttonData->statusHistory == 0x00)
	{
		// stable click detected, transition state
		buttonData->state = UI_BUTSTAT_WAIT_FOR_PRESS_UP;
	}
	// 
	// Waiting for press up
	// 
	else if (buttonData->state == UI_BUTSTAT_WAIT_FOR_PRESS_UP)
	{
		// have we detected movement of rotary encoder?
		if (rotaryEncoderMovementDetected)
		{
			// zero out accumulator
			buttonData->ticksAccumulator = 0;

			// transition to state
			buttonData->state = UI_BUTSTAT_WAIT_FOR_PRESS_UP_AFTER_MOVEMENT;
		}
		// have we found a press up pattern?
		else if (buttonData->statusHistory == 0x07)
		{
			// check how long button was down
			if (buttonData->ticksAccumulator > UI_BUTTON_TICKS_FOR_LONG_PRESS)
			{
				// set event type
				buttonData->event = UI_EVT_SINGLE_LONG_PRESS;
			}
			else
			{
				// set event type
				buttonData->event = UI_EVT_SINGLE_SHORT_PRESS;
			}

			// zero out accumulator
			buttonData->ticksAccumulator = 0;

			// yes we have, start checking for double click
			buttonData->state = UI_BUTSTAT_WAIT_SECOND_PRESS_DOWN;
		}
		else
		{
			// check how long button was down. Once it passes a threshold
			// check event
			if (++buttonData->ticksAccumulator > UI_BUTTON_TICKS_FOR_LONG_PRESS)
			{
				// set event type
				*event = UI_EVT_SINGLE_LONG_PRESS_HOLDING;
			}
		}
	}
	// 
	// Waiting for press up (button release) after movement
	// 
	else if (buttonData->state == UI_BUTSTAT_WAIT_FOR_PRESS_UP_AFTER_MOVEMENT && buttonData->statusHistory == 0x07)
	{
		// back to idle state, ignoring click
		buttonData->state = UI_BUTSTAT_WAIT_FOR_PRESS_DOWN;
	}
	// 
	// Waiting for second press down
	// 
	else if (buttonData->state == UI_BUTSTAT_WAIT_SECOND_PRESS_DOWN)
	{
		// have we found a press down pattern?
		if (buttonData->statusHistory == 0x00)
		{
			// yes we have, start checking for double click
			buttonData->state = UI_BUTSTAT_WAIT_SECOND_PRESS_UP;
		}
		else
		{
			// not yet, count ticks and check if we've gone over number of ticks
			// waiting for a the second click
			if (++buttonData->ticksAccumulator > UI_BUTTON_MAX_TICKS_FOR_SECOND_CLICK)
			{
				// zero out accumulator
				buttonData->ticksAccumulator = 0;

				// set the event (long/short press)
				*event = buttonData->event;

				// zero out states/event
				buttonData->state = UI_BUTSTAT_WAIT_FOR_PRESS_DOWN;
				buttonData->event = UI_EVT_NONE;
			}
		}
	}
	// 
	// Waiting for second press up
	// 
	else if (buttonData->state == UI_BUTSTAT_WAIT_SECOND_PRESS_UP && buttonData->statusHistory == 0x07)
	{
		// zero out accumulator
		buttonData->ticksAccumulator = 0;

		// set the event (long/short press)
		*event = UI_EVT_DOUBLE_PRESS;

		// zero out states/event
		buttonData->state = UI_BUTSTAT_WAIT_FOR_PRESS_DOWN;
		buttonData->event = UI_EVT_NONE;
	}
}
// ==========================================================================
// initialize button state

void ui_buttonDataInit(UI_BUTTON_DATA *buttonData)
{
	// just initialzie the members
	buttonData->state = UI_BUTSTAT_WAIT_FOR_PRESS_DOWN;
	buttonData->event = UI_EVT_NONE;
	buttonData->statusHistory = 0xFF;
	buttonData->ticksAccumulator = 0;
}	
// ==========================================================================
// update a timer with rotations

void ui_modifyTimerValue(UI_TIMER *timer, int_8 rotation)
{
	// update minutes
	timer->minutesLeft += (rotation * ui_timerInterval);

	// have we gone under 0?
	if (timer->minutesLeft < 0) 
	{
		// are there any hours left?
		if (timer->hoursLeft)
		{
			// wrap
			timer->minutesLeft = (timer->minutesLeft + 60);

			// decrement hours, where 0 goes to 12
			timer->hoursLeft--;
		}
		else
		{
			// zero out minutes left
			timer->minutesLeft = 0;
		}
	}
	// have we gone over 59?
	else if (timer->minutesLeft >= 60)
	{
		// wrap 
		timer->minutesLeft = (timer->minutesLeft - 60);

		// increment hours, wrapping at 12
		if (++timer->hoursLeft > 12) timer->hoursLeft = 0;
	}
}
// ==========================================================================
// display a timer

void ui_displayTimer(UI_TIMER *timer)
{
	// check if any minutes or hours
	if (timer->minutesLeft || timer->hoursLeft)
	{
		// display hours/minutes
		sseg_setDisplay(timer->minutesLeft + (100 * timer->hoursLeft));
	}
	// if only seconds - display that
	else if (timer->secondsLeft)
	{
		// display seconds
		sseg_setDisplay(timer->secondsLeft);
	}		
	else
	{
		// display 0
		sseg_setDisplay(0);	
	}	
}
// ==========================================================================
// beep

void ui_beep(uint_16 ticksToBeep)
{
	// start beeping
	ui_beepingStart();

	// set ticks to beep
	ui_ticksToBeep = ticksToBeep;
}
// ==========================================================================
// start beeping

void ui_beepingStart(void)
{
	// set beeper
	sys_bitSet(IO_BUZZER);
}
// ==========================================================================
// stop beeping

void ui_beepingStop(void)
{
	// set beeper
	sys_bitClear(IO_BUZZER);
}
// ==========================================================================
// tick beeper

void ui_beeperTick(void)
{
	// decrement ticks to beep
	if (ui_ticksToBeep && --ui_ticksToBeep == 0)
	{
		// stop beeping
		ui_beepingStop();
	}
}
// ==========================================================================
// select next timer

void ui_selectTimer(uint_8 timerIndex)
{
	// save previous
	uint_8 previousTimerIndex = ui_currentTimerIndex;

	// set current index
	ui_currentTimerIndex = timerIndex;

	// set pointer
	ui_currentTimer = &ui_timers[ui_currentTimerIndex];

	// set previous led off
	ui_timerLedSet(previousTimerIndex, FALSE);

	// set new led on
	ui_timerLedSet(ui_currentTimerIndex, TRUE);
}
// ==========================================================================
// display which timer is currently selected

void ui_timerLedSet(uint_8 timerIndex, BOOL state)
{
	// set by index
	if (timerIndex == 0) IO_LED_TMR0 = state;
	else if (timerIndex == 1) IO_LED_TMR1 = state;
	else if (timerIndex == 2) IO_LED_TMR2 = state;
}
// ==========================================================================
// toggle specific timer led

void ui_timerLedToggle(uint_8 timerIndex)
{
	// set by index
	if (timerIndex == 0) IO_LED_TMR0 = !IO_LED_TMR0;
	else if (timerIndex == 1) IO_LED_TMR1 = !IO_LED_TMR1;
	else if (timerIndex == 2) IO_LED_TMR2 = !IO_LED_TMR2;
}
// ==========================================================================
// decrement seconds left for all timers and check for expiration

void ui_decrementTimerSeconds(void)
{
	// timer index
	uint_8 timerIdx;

	// ptr to timer
	UI_TIMER *timer;

	// iterate over all timers
	for (timerIdx = 0; timerIdx < UI_TIMER_COUNT; ++timerIdx)
	{
		// get timer pointer
		timer = &ui_timers[timerIdx];

		// is active?
		if (timer->state == UI_TIMER_STATE_ACTIVE)
		{
			// check seconds left
			if (timer->secondsLeft != 0)
			{
				// just decrement
				timer->secondsLeft--;
			}
			else
			{
				// no seconds left, check if any minutes left
				if (timer->minutesLeft == 0)
				{
					// no minutes left, check hours left
					if (timer->hoursLeft == 0)
					{
						// timer has expired
						ui_handleTimerExpired(timer);
					}
					else
					{
						// decrement hours, add minutes and seconds
						timer->hoursLeft--;
						timer->minutesLeft = 59;
						timer->secondsLeft = 59;
					}
				}
				else
				{
					// still minutes remaining, decrement 1 minute and add 59 seconds to seconds
					timer->minutesLeft--;
					timer->secondsLeft = 59;
				}
			}
		}
	}
}
// ==========================================================================
// check if any expired timers need to automatically shut down

void ui_checkAutomaticAlarmShutdown(void)
{
	// timer index
	uint_8 timerIdx;

	// ptr to timer
	UI_TIMER *timer;

	// iterate over all timers
	for (timerIdx = 0; timerIdx < UI_TIMER_COUNT; ++timerIdx)
	{
		// get timer pointer
		timer = &ui_timers[timerIdx];

		// check if timer is expired
		if (timer->state == UI_TIMER_STATE_EXPIRED)
		{
			// decrement seconds for automatic alarm turn off and check
			// if alarm should be turned off
			if (--timer->alarmSecondsLeft == 0)
			{
				// change timer state to inactive
				timer->state = UI_TIMER_STATE_INACTIVE;

				// decrement alarms
				ui_expiredAlarmsDecrement();

				// is this the currently displayed timer?
				if (timerIdx == ui_currentTimerIndex)
				{
					// show points
					sseg_setDecPtStates(TRUE);
					
					// must change state
					ui_currentState = UI_STATE_DISPLAYING_INACTIVE_TIMER;
				}
				else
				{
					// shut off its led
					ui_timerLedSet(timerIdx, FALSE);
				}
			}
		}
	}
}
// ==========================================================================
// timer has expired, set

void ui_handleTimerExpired(UI_TIMER *timer)
{
	// set state
	timer->state = UI_TIMER_STATE_EXPIRED;

	// increment number of expired alarms and turn on alarm if required
	ui_expiredAlarmsIncrement();

	// reset the number of seconds left for alarm
	timer->alarmSecondsLeft = UI_TIMER_ALARM_SECONDS_AUTO_OFF;

	// is it the displayed timer?
	if (ui_currentTimer == timer)
	{	
		// show points
		sseg_setDecPtStates(TRUE);
	}
}
// ==========================================================================
// select next timer interval

void ui_nextTimerInterval(void)
{
	if (ui_timerInterval == 1) 	ui_timerInterval = 15;
	else						ui_timerInterval = 1;
}
// ==========================================================================
// reset the timer value

void ui_resetTimerValue(UI_TIMER *timer)
{
	timer->hoursLeft = 0;
	timer->minutesLeft = 0;
	timer->secondsLeft = 0;
}
// ==========================================================================
// get the next timer index by rotation

uint_8 ui_getNextTimerIndexByRotation(int_8 rotation)
{
	uint_8 nextTimerIndex = ui_currentTimerIndex;

	// next/prev timer
	if (rotation > 0)
	{
		// increment timer index
		nextTimerIndex++;

		// check if overflow and zero
		if (nextTimerIndex >= UI_TIMER_COUNT) nextTimerIndex = 0;
	}
	else
	{
		// check if overflow and zero
		if (nextTimerIndex == 0) nextTimerIndex = UI_TIMER_COUNT - 1;
		else nextTimerIndex--;
	}

	// return the next timer index
	return nextTimerIndex;
}
// ==========================================================================
// a new alarm has expired

void ui_expiredAlarmsIncrement(void)
{
	// increment alarms and check if first
	if (++ui_numberOfExpiredTimers == 1)
	{
		// first new alarm? start the alarm
		ui_alarmStart();

		// and start the visual indication timer
		ui_startTimer(&ui_tmrAlarmVisualIndication, UI_TMR_VISUAL_INDICATION_TICKS);
	}
}
// ==========================================================================
// an expired alarm has been acknowledged

void ui_expiredAlarmsDecrement(void)
{
	// decrement larms and check if last
	if (--ui_numberOfExpiredTimers == 0)
	{
		// last alarm
		ui_alarmStop();

		// stop the timer
		ui_stopTimer(&ui_tmrAlarmVisualIndication);
	}
}
// ==========================================================================
// toggle expired timer LEDs

void ui_toggleExpiredTimerLeds(void)
{
	// timer index
	uint_8 timerIdx;

	// iterate over all timers
	for (timerIdx = 0; timerIdx < UI_TIMER_COUNT; ++timerIdx)
	{
		// check if timer is expired
		if (timerIdx != ui_currentTimerIndex && ui_timers[timerIdx].state == UI_TIMER_STATE_EXPIRED)
		{
			// toggle the appropriate led
			ui_timerLedToggle(timerIdx);
		}
	}
}
// ==========================================================================
// reload cadence

void ui_alarmCadenceReload(void)
{
	// number of bits left
	ui_currentAlarmCadenceBitsLeft = 16;
	
	// set cadence
	ui_currentAlarmCadence = UI_ALARM_CADENCE;	
}
// ==========================================================================
// plays out the next stage in teh alarm cadence

void ui_alarmPlayNextCadenceBit(void)
{
	// check if bit is set
	if (ui_currentAlarmCadence & 0x1)
	{
		// start beep
		ui_beepingStart();
	}
	else
	{
		// stop beep
		ui_beepingStop();
	}

	// and shift for next time
	ui_currentAlarmCadence >>= 1;
	
	// decrement bits left
	if (--ui_currentAlarmCadenceBitsLeft == 0)
	{
		// reload the cadence
		ui_alarmCadenceReload();
	}	
}
// ==========================================================================
// start alarm

void ui_alarmStart(void)
{
	// load cadence into shift variable
	ui_alarmCadenceReload();

	// start beeping
	ui_alarmPlayNextCadenceBit();
}
// ==========================================================================
// stop alarm

void ui_alarmStop(void)
{
	// stop beeping
	ui_beepingStop();
}
// ==========================================================================
// start timer

void ui_startTimer(uint_16 *timer, const uint_16 ticks)
{
	// set ticks
	*timer = ticks;
}
// ==========================================================================
// stop timer

void ui_stopTimer(uint_16 *timer)
{
	// zero ticks
	*timer = 0;
}
// ==========================================================================


// ==========================================================================
// State machine
// ==========================================================================

//
// DisplayingInactiveTimer
// 

// ==========================================================================
// movement with no pressing

void ui_displayingInactiveTimerMovementNoPress(int_8 rotation)
{
	// if we are inactive and there is movement, zero out seconds
	ui_currentTimer->secondsLeft = 0;

	// update timer interval
	ui_modifyTimerValue(ui_currentTimer, rotation);

	// update display
	ui_displayTimer(ui_currentTimer);
}
// ==========================================================================
// movement with pressing

void ui_displayingInactiveTimerMovementPress(int_8 rotation)
{
	// short beep
	ui_beep(UI_SHORT_BEEP_TICKS);

	// select the timer
	ui_selectTimer(ui_getNextTimerIndexByRotation(rotation));

	// update display
	ui_displayTimer(ui_currentTimer);

	// is new timer active?
	if (ui_currentTimer->state == UI_TIMER_STATE_ACTIVE)
	{
		// transition state
		ui_currentState = UI_STATE_DISPLAYING_ACTIVE_TIMER;
	}
	// check if timer is expired
	else if (ui_currentTimer->state == UI_TIMER_STATE_EXPIRED)
	{
		// decrement number of expired alarms
		ui_expiredAlarmsDecrement();

		// set timer inactive
		ui_currentTimer->state = UI_TIMER_STATE_INACTIVE;
	}
}
// ==========================================================================
// 1 second timer

void ui_displayingInactiveTimer1SecondTimer(void)
{
	// decrement timers
	ui_decrementTimerSeconds();

	// shutdown stale alarms
	ui_checkAutomaticAlarmShutdown();
}
// ==========================================================================
// short press

void ui_displayingInactiveTimerShortPress(void)
{
	// short beep
	ui_beep(UI_SHORT_BEEP_TICKS);

	// update the timer interval
	ui_nextTimerInterval();
}
// ==========================================================================
// long press

void ui_displayingInactiveTimerLongPress(void)
{
	// long beep
	ui_beep(UI_LONG_BEEP_TICKS);

	// set timer active
	ui_currentTimer->state = UI_TIMER_STATE_ACTIVE;

	// transition state
	ui_currentState = UI_STATE_DISPLAYING_ACTIVE_TIMER;
}
// ==========================================================================
// double press

void ui_displayingInactiveTimerDoublePress(void)
{
	// zero out the current timer
	ui_resetTimerValue(ui_currentTimer);

	// display the new timer
	ui_displayTimer(ui_currentTimer);
}
// ==========================================================================
// timer expiration

void ui_displayingInactiveTimerHandleTimerExpiration(uint_16 *timer)
{
	// toggle expired leds
	ui_toggleExpiredTimerLeds();

	// play next bit if alarm is on
	if (ui_alarmOn()) ui_alarmPlayNextCadenceBit(); 

	// and restart the visual indication timer
	ui_startTimer(&ui_tmrAlarmVisualIndication, UI_TMR_VISUAL_INDICATION_TICKS);
}
// ==========================================================================

//
// DisplayingActiveTimer
// 

// ==========================================================================
// movement with pressing

void ui_displayingActiveTimerMovementPress(int_8 rotation)
{
	// check if timer is expired
	if (ui_currentTimer->state == UI_TIMER_STATE_EXPIRED)
	{
		// decrement number of expired alarms
		ui_expiredAlarmsDecrement();

		// set timer inactive
		ui_currentTimer->state = UI_TIMER_STATE_INACTIVE;
	}

	// short beep
	ui_beep(UI_SHORT_BEEP_TICKS);

	// select the next timer
	ui_selectTimer(ui_getNextTimerIndexByRotation(rotation));

	// update display
	ui_displayTimer(ui_currentTimer);

	// is new timer active?
	if (ui_currentTimer->state == UI_TIMER_STATE_INACTIVE)
	{
		// show points
		sseg_setDecPtStates(TRUE);

		// transition state
		ui_currentState = UI_STATE_DISPLAYING_INACTIVE_TIMER;
	}
}
// ==========================================================================
// 1 second timer

void ui_displayingActiveTimer1SecondTimer(void)
{
	// displaying expired timer?
	if (ui_currentTimer->state != UI_TIMER_STATE_EXPIRED)
	{
		// toggle dots if current timer hasn't expired
		sseg_toggleDecPtStates();
	}

	// decrement timers
	ui_decrementTimerSeconds();

	// shutdown stale alarms
	ui_checkAutomaticAlarmShutdown();

	// update current timer
	ui_displayTimer(ui_currentTimer);
}
// ==========================================================================
// short press

void ui_displayingActiveTimerShortPress(void)
{
	// check if timer is expired
	if (ui_currentTimer->state == UI_TIMER_STATE_EXPIRED)
	{
		// decrement number of expired alarms
		ui_expiredAlarmsDecrement();
	}

	// short beep
	ui_beep(UI_SHORT_BEEP_TICKS);

	// show points
	sseg_setDecPtStates(TRUE);

	// set timer inactive
	ui_currentTimer->state = UI_TIMER_STATE_INACTIVE;

	// transition state
	ui_currentState = UI_STATE_DISPLAYING_INACTIVE_TIMER;
}
// ==========================================================================
// double press

void ui_displayingActiveTimerDoublePress(void)
{
	// displaying expired timer?
	if (ui_currentTimer->state == UI_TIMER_STATE_EXPIRED)
	{
		// decrement number of expired alarms
		ui_expiredAlarmsDecrement();
	}

	// short beep
	ui_beep(UI_SHORT_BEEP_TICKS);

	// show points
	sseg_setDecPtStates(TRUE);

	// set timer inactive
	ui_currentTimer->state = UI_TIMER_STATE_INACTIVE;

	// zero out the current timer
	ui_resetTimerValue(ui_currentTimer);

	// update current timer
	ui_displayTimer(ui_currentTimer);

	// transition state
	ui_currentState = UI_STATE_DISPLAYING_INACTIVE_TIMER;
}
// ==========================================================================
// timer expiration

void ui_displayingActiveTimerHandleTimerExpiration(uint_16 *timer)
{
	// toggle expired leds
	ui_toggleExpiredTimerLeds();

	// are we displaying an expired timer?
	if (ui_currentTimer->state == UI_TIMER_STATE_EXPIRED)
	{
		// toggle points rapidly
		sseg_toggleDecPtStates();
	}

	// play next bit if alarm is on
	if (ui_alarmOn()) ui_alarmPlayNextCadenceBit(); 

	// and restart the visual indication timer
	ui_startTimer(&ui_tmrAlarmVisualIndication, UI_TMR_VISUAL_INDICATION_TICKS);
}
// ==========================================================================

