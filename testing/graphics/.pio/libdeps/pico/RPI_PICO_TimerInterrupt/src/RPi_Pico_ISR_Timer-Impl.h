/****************************************************************************************************************************
  RPi_Pico_ISR_Timer-Impl.h
  For RP2040-based boards such as RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040 and GENERIC_RP2040.
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/RPI_PICO_TimerInterrupt
  Licensed under MIT license

  The RPI_PICO system timer peripheral provides a global microsecond timebase for the system, and generates
  interrupts based on this timebase. It supports the following features:
    • A single 64-bit counter, incrementing once per microsecond
    • This counter can be read from a pair of latching registers, for race-free reads over a 32-bit bus.
    • Four alarms: match on the lower 32 bits of counter, IRQ on match: TIMER_IRQ_0-TIMER_IRQ_3

  Now even you use all these new 16 ISR-based timers,with their maximum interval practically unlimited (limited only by
  unsigned long miliseconds), you just consume only one RPI_PICO timer and avoid conflicting with other cores' tasks.
  The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
  Therefore, their executions are not blocked by bad-behaving functions / tasks.
  This important feature is absolutely necessary for mission-critical tasks.

  Based on SimpleTimer - A timer library for Arduino.
  Author: mromani@ottotecnica.com
  Copyright (c) 2010 OTTOTECNICA Italy

  Based on BlynkTimer.h
  Author: Volodymyr Shymanskyy

  Version: 1.3.1

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      11/05/2021 Initial coding to support RP2040-based boards such as RASPBERRY_PI_PICO. etc.
  1.0.1   K Hoang      18/05/2021 Update README and Packages' Patches to match core arduino-pico core v1.4.0
  1.1.0   K Hoang      10/00/2021 Add support to new boards using the arduino-pico core
  1.1.1   K Hoang      22/10/2021 Fix platform in library.json for PIO
  1.2.0   K.Hoang      21/01/2022 Fix `multiple-definitions` linker error
  1.3.0   K.Hoang      25/09/2022 Fix severe bug affecting time between the starts
  1.3.1   K.Hoang      29/09/2022 Using float instead of ulong for interval
*****************************************************************************************************************************/

#pragma once

#ifndef ISR_TIMER_GENERIC_IMPL_H
#define ISR_TIMER_GENERIC_IMPL_H

#include <string.h>

////////////////////////////////////////////////////////////////

RPI_PICO_ISR_Timer::RPI_PICO_ISR_Timer()
  : numTimers (-1)
{
}

////////////////////////////////////////////////////////////////

void RPI_PICO_ISR_Timer::init() 
{
  unsigned long current_millis = millis();   //elapsed();

  for (uint8_t i = 0; i < RPI_PICO_MAX_TIMERS; i++) 
  {
    memset((void*) &timer[i], 0, sizeof (timer_t));
    timer[i].prev_millis = current_millis;
  }

  numTimers = 0;
}

////////////////////////////////////////////////////////////////

void RPI_PICO_ISR_Timer::run() 
{
  uint8_t i;
  unsigned long current_millis;

  // get current time
  current_millis = millis();   //elapsed();

  // RPI_PICO is a multi core / multi processing chip. It is mandatory to disable task switches during ISR
  rp2040.idleOtherCore();
  
  for (i = 0; i < RPI_PICO_MAX_TIMERS; i++) 
  {

    timer[i].toBeCalled = RPI_PICO_DEFCALL_DONTRUN;

    // no callback == no timer, i.e. jump over empty slots
    if (timer[i].callback != NULL) 
    {

      // is it time to process this timer ?
      // see http://arduino.cc/forum/index.php/topic,124048.msg932592.html#msg932592

      if ((current_millis - timer[i].prev_millis) >= timer[i].delay) 
      {
        unsigned long skipTimes = (current_millis - timer[i].prev_millis) / timer[i].delay;
        
        // update time
        timer[i].prev_millis += timer[i].delay * skipTimes;

        // check if the timer callback has to be executed
        if (timer[i].enabled) 
        {

          // "run forever" timers must always be executed
          if (timer[i].maxNumRuns == RPI_PICO_RUN_FOREVER) 
          {
            timer[i].toBeCalled = RPI_PICO_DEFCALL_RUNONLY;
          }
          // other timers get executed the specified number of times
          else if (timer[i].numRuns < timer[i].maxNumRuns) 
          {
            timer[i].toBeCalled = RPI_PICO_DEFCALL_RUNONLY;
            timer[i].numRuns++;

            // after the last run, delete the timer
            if (timer[i].numRuns >= timer[i].maxNumRuns) 
            {
              timer[i].toBeCalled = RPI_PICO_DEFCALL_RUNANDDEL;
            }
          }
        }
      }
    }
  }

  for (i = 0; i < RPI_PICO_MAX_TIMERS; i++) 
  {
    if (timer[i].toBeCalled == RPI_PICO_DEFCALL_DONTRUN)
      continue;

    if (timer[i].hasParam)
      (*(timer_callback_p)timer[i].callback)(timer[i].param);
    else
      (*(timer_callback)timer[i].callback)();

    if (timer[i].toBeCalled == RPI_PICO_DEFCALL_RUNANDDEL)
      deleteTimer(i);
  }

  // RPI_PICO is a multi core / multi processing chip. It is mandatory to disable task switches during ISR
  rp2040.resumeOtherCore();

}

////////////////////////////////////////////////////////////////

// find the first available slot
// return -1 if none found
int RPI_PICO_ISR_Timer::findFirstFreeSlot() 
{
  // all slots are used
  if (numTimers >= RPI_PICO_MAX_TIMERS) 
  {
    return -1;
  }

  // return the first slot with no callback (i.e. free)
  for (uint8_t i = 0; i < RPI_PICO_MAX_TIMERS; i++) 
  {
    if (timer[i].callback == NULL) 
    {
      return i;
    }
  }

  // no free slots found
  return -1;
}

////////////////////////////////////////////////////////////////

int RPI_PICO_ISR_Timer::setupTimer(const float& d, void* f, void* p, bool h, const unsigned& n) 
{
  int freeTimer;

  if (numTimers < 0) 
  {
    init();
  }

  freeTimer = findFirstFreeSlot();
  if (freeTimer < 0) 
  {
    return -1;
  }

  if (f == NULL) 
  {
    return -1;
  }

  timer[freeTimer].delay = d;
  timer[freeTimer].callback = f;
  timer[freeTimer].param = p;
  timer[freeTimer].hasParam = h;
  timer[freeTimer].maxNumRuns = n;
  timer[freeTimer].enabled = true;
  timer[freeTimer].prev_millis = millis();

  numTimers++;

  return freeTimer;
}

////////////////////////////////////////////////////////////////

int RPI_PICO_ISR_Timer::setTimer(const float& d, timer_callback f, const unsigned& n) 
{
  return setupTimer(d, (void *)f, NULL, false, n);
}

////////////////////////////////////////////////////////////////

int RPI_PICO_ISR_Timer::setTimer(const float& d, timer_callback_p f, void* p, const unsigned& n) 
{
  return setupTimer(d, (void *)f, p, true, n);
}

////////////////////////////////////////////////////////////////

int RPI_PICO_ISR_Timer::setInterval(const float& d, timer_callback f) 
{
  return setupTimer(d, (void *)f, NULL, false, RPI_PICO_RUN_FOREVER);
}

////////////////////////////////////////////////////////////////

int RPI_PICO_ISR_Timer::setInterval(const float& d, timer_callback_p f, void* p) 
{
  return setupTimer(d, (void *)f, p, true, RPI_PICO_RUN_FOREVER);
}

////////////////////////////////////////////////////////////////

int RPI_PICO_ISR_Timer::setTimeout(const float& d, timer_callback f) 
{
  return setupTimer(d, (void *)f, NULL, false, RPI_PICO_RUN_ONCE);
}

////////////////////////////////////////////////////////////////

int RPI_PICO_ISR_Timer::setTimeout(const float& d, timer_callback_p f, void* p) 
{
  return setupTimer(d, (void *)f, p, true, RPI_PICO_RUN_ONCE);
}

////////////////////////////////////////////////////////////////

bool RPI_PICO_ISR_Timer::changeInterval(const unsigned& numTimer, const float& d) 
{
  if (numTimer >= RPI_PICO_MAX_TIMERS) 
  {
    return false;
  }

  // Updates interval of existing specified timer
  if (timer[numTimer].callback != NULL) 
  {
    // RPI_PICO is a multi core / multi processing chip. It is mandatory to disable task switches during modifying shared vars
    rp2040.idleOtherCore();

    timer[numTimer].delay = d;
    timer[numTimer].prev_millis = millis();

    // RPI_PICO is a multi core / multi processing chip. It is mandatory to disable task switches during modifying shared vars
    rp2040.resumeOtherCore();

    return true;
  }
  
  // false return for non-used numTimer, no callback
  return false;
}

////////////////////////////////////////////////////////////////

void RPI_PICO_ISR_Timer::deleteTimer(const unsigned& timerId) 
{
  if (timerId >= RPI_PICO_MAX_TIMERS) 
  {
    return;
  }

  // nothing to delete if no timers are in use
  if (numTimers == 0) 
  {
    return;
  }

  // don't decrease the number of timers if the specified slot is already empty
  if (timer[timerId].callback != NULL) 
  {
    // RPI_PICO is a multi core / multi processing chip. It is mandatory to disable task switches during modifying shared vars
    rp2040.idleOtherCore();

    memset((void*) &timer[timerId], 0, sizeof (timer_t));
    timer[timerId].prev_millis = millis();

    // update number of timers
    numTimers--;

    // RPI_PICO is a multi core / multi processing chip. It is mandatory to disable task switches during modifying shared vars
    rp2040.resumeOtherCore();

  }
}

////////////////////////////////////////////////////////////////

// function contributed by code@rowansimms.com
void RPI_PICO_ISR_Timer::restartTimer(const unsigned& numTimer) 
{
  if (numTimer >= RPI_PICO_MAX_TIMERS) 
  {
    return;
  }

  // RPI_PICO is a multi core / multi processing chip. It is mandatory to disable task switches during modifying shared vars
  rp2040.idleOtherCore();

  timer[numTimer].prev_millis = millis();

  // RPI_PICO is a multi core / multi processing chip. It is mandatory to disable task switches during modifying shared vars
  rp2040.resumeOtherCore();
}

////////////////////////////////////////////////////////////////

bool RPI_PICO_ISR_Timer::isEnabled(const unsigned& numTimer) 
{
  if (numTimer >= RPI_PICO_MAX_TIMERS) 
  {
    return false;
  }

  return timer[numTimer].enabled;
}

////////////////////////////////////////////////////////////////

void RPI_PICO_ISR_Timer::enable(const unsigned& numTimer) 
{
  if (numTimer >= RPI_PICO_MAX_TIMERS) 
  {
    return;
  }

  timer[numTimer].enabled = true;
}

////////////////////////////////////////////////////////////////

void RPI_PICO_ISR_Timer::disable(const unsigned& numTimer) 
{
  if (numTimer >= RPI_PICO_MAX_TIMERS) 
  {
    return;
  }

  timer[numTimer].enabled = false;
}

////////////////////////////////////////////////////////////////

void RPI_PICO_ISR_Timer::enableAll() 
{
  // Enable all timers with a callback assigned (used)

  // RPI_PICO is a multi core / multi processing chip. It is mandatory to disable task switches during modifying shared vars
  rp2040.idleOtherCore();

  for (uint8_t i = 0; i < RPI_PICO_MAX_TIMERS; i++) 
  {
    if (timer[i].callback != NULL && timer[i].numRuns == RPI_PICO_RUN_FOREVER) 
    {
      timer[i].enabled = true;
    }
  }

  // RPI_PICO is a multi core / multi processing chip. It is mandatory to disable task switches during modifying shared vars
  rp2040.resumeOtherCore();
}

////////////////////////////////////////////////////////////////

void RPI_PICO_ISR_Timer::disableAll() 
{
  // Disable all timers with a callback assigned (used)

  // RPI_PICO is a multi core / multi processing chip. It is mandatory to disable task switches during modifying shared vars
  rp2040.idleOtherCore();
  
  for (uint8_t i = 0; i < RPI_PICO_MAX_TIMERS; i++) 
  {
    if (timer[i].callback != NULL && timer[i].numRuns == RPI_PICO_RUN_FOREVER) 
    {
      timer[i].enabled = false;
    }
  }

  // RPI_PICO is a multi core / multi processing chip. It is mandatory to disable task switches during modifying shared vars
  rp2040.resumeOtherCore();

}

////////////////////////////////////////////////////////////////

void RPI_PICO_ISR_Timer::toggle(const unsigned& numTimer) 
{
  if (numTimer >= RPI_PICO_MAX_TIMERS) 
  {
    return;
  }

  timer[numTimer].enabled = !timer[numTimer].enabled;
}

////////////////////////////////////////////////////////////////

unsigned RPI_PICO_ISR_Timer::getNumTimers() 
{
  return numTimers;
}

////////////////////////////////////////////////////////////////

#endif    // ISR_TIMER_GENERIC_IMPL_H

