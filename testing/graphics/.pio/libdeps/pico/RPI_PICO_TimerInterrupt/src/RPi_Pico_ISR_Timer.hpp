/****************************************************************************************************************************
  RPi_Pico_ISR_Timer.hpp
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

#ifndef ISR_TIMER_GENERIC_HPP
#define ISR_TIMER_GENERIC_HPP

////////////////////////////////////////////////////////////////

#if ( defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_RASPBERRY_PI_PICO) || defined(ARDUINO_ADAFRUIT_FEATHER_RP2040) || defined(ARDUINO_GENERIC_RP2040) ) && !defined(ARDUINO_ARCH_MBED) 
  #if defined(USING_RPI_PICO_TIMER_INTERRUPT)
    #undef USING_RPI_PICO_TIMER_INTERRUPT
  #endif  
  #define USING_RPI_PICO_TIMER_INTERRUPT        true  
#else
  #error This code is intended to run on the non-mbed RP2040 arduino-pico platform! Please check your Tools->Board setting.
#endif

////////////////////////////////////////////////////////////////

#ifndef RPI_PICO_TIMER_INTERRUPT_VERSION
  #define RPI_PICO_TIMER_INTERRUPT_VERSION       "RPi_Pico_TimerInterrupt v1.3.1"
  
  #define RPI_PICO_TIMER_INTERRUPT_VERSION_MAJOR      1
  #define RPI_PICO_TIMER_INTERRUPT_VERSION_MINOR      3
  #define RPI_PICO_TIMER_INTERRUPT_VERSION_PATCH      1

  #define RPI_PICO_TIMER_INTERRUPT_VERSION_INT        1003001  
#endif

////////////////////////////////////////////////////////////////

#include "TimerInterrupt_Generic_Debug.h"

#include <stddef.h>

#include <inttypes.h>

#include "pico/multicore.h"

#if defined(ARDUINO)
  #if ARDUINO >= 100
    #include <Arduino.h>
  #else
    #include <WProgram.h>
  #endif
#endif

#define FLAG_VALUE    0xDEADBEEF

#define RPI_PICO_ISR_Timer RPI_PICO_ISRTimer

typedef void (*timer_callback)();
typedef void (*timer_callback_p)(void *);

////////////////////////////////////////////////////////////////

class RPI_PICO_ISR_Timer 
{
  public:
    // maximum number of timers
#define RPI_PICO_MAX_TIMERS        16
#define RPI_PICO_RUN_FOREVER       0
#define RPI_PICO_RUN_ONCE          1

    // constructor
    RPI_PICO_ISR_Timer();

    void init();

    // this function must be called inside loop()
    void run();

    // Timer will call function 'f' every 'd' milliseconds forever
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int setInterval(const float& d, timer_callback f);

    // Timer will call function 'f' with parameter 'p' every 'd' milliseconds forever
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int setInterval(const float& d, timer_callback_p f, void* p);

    // Timer will call function 'f' after 'd' milliseconds one time
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int setTimeout(const float& d, timer_callback f);

    // Timer will call function 'f' with parameter 'p' after 'd' milliseconds one time
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int setTimeout(const float& d, timer_callback_p f, void* p);

    // Timer will call function 'f' every 'd' milliseconds 'n' times
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int setTimer(const float& d, timer_callback f, const unsigned& n);

    // Timer will call function 'f' with parameter 'p' every 'd' milliseconds 'n' times
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int setTimer(const float& d, timer_callback_p f, void* p, const unsigned& n);

    // updates interval of the specified timer
    bool changeInterval(const unsigned& numTimer, const float& d);

    // destroy the specified timer
    void deleteTimer(const unsigned& numTimer);

    // restart the specified timer
    void restartTimer(const unsigned& numTimer);

    // returns true if the specified timer is enabled
    bool isEnabled(const unsigned& numTimer);

    // enables the specified timer
    void enable(const unsigned& numTimer);

    // disables the specified timer
    void disable(const unsigned& numTimer);

    // enables all timers
    void enableAll();

    // disables all timers
    void disableAll();

    // enables the specified timer if it's currently disabled, and vice-versa
    void toggle(const unsigned& numTimer);

    // returns the number of used timers
    unsigned getNumTimers();

    // returns the number of available timers
    unsigned getNumAvailableTimers() 
    {
      return RPI_PICO_MAX_TIMERS - numTimers;
    };

  private:
    // deferred call constants
#define RPI_PICO_DEFCALL_DONTRUN   0       // don't call the callback function
#define RPI_PICO_DEFCALL_RUNONLY   1       // call the callback function but don't delete the timer
#define RPI_PICO_DEFCALL_RUNANDDEL 2       // call the callback function and delete the timer

    // low level function to initialize and enable a new timer
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int setupTimer(const float& d, void* f, void* p, bool h, const unsigned& n);

    // find the first available slot
    int findFirstFreeSlot();

    typedef struct 
    {
      unsigned long prev_millis;        // value returned by the millis() function in the previous run() call
      void*         callback;           // pointer to the callback function
      void*         param;              // function parameter
      bool          hasParam;           // true if callback takes a parameter
      float         delay;              // delay value
      unsigned      maxNumRuns;         // number of runs to be executed
      unsigned      numRuns;            // number of executed runs
      bool          enabled;            // true if enabled
      unsigned      toBeCalled;         // deferred function call (sort of) - N.B.: only used in run()
    } timer_t;

    volatile timer_t timer[RPI_PICO_MAX_TIMERS];

    // actual number of timers in use (-1 means uninitialized)
    volatile int numTimers;
};

////////////////////////////////////////////////////////////////

#endif    // ISR_TIMER_GENERIC_HPP


