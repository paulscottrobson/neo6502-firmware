/****************************************************************************************************************************
  TimerInterruptTest.ino
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
*****************************************************************************************************************************/
/*
   Notes:
   Special design is necessary to share data between interrupt code and the rest of your program.
   Variables usually need to be "volatile" types. Volatile tells the compiler to avoid optimizations that assume
   variable can not spontaneously change. Because your function may change variables while your program is using them,
   the compiler needs this hint. But volatile alone is often not enough.
   When accessing shared variables, usually interrupts must be disabled. Even with volatile,
   if the interrupt changes a multi-byte variable between a sequence of instructions, it can be read incorrectly.
   If your data is multiple variables, such as an array and a count, usually interrupts need to be disabled
   or the entire sequence of your code which accesses the data.
*/

// These define's must be placed at the beginning before #include "TimerInterrupt_Generic.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         1
#define _TIMERINTERRUPT_LOGLEVEL_     4

// Can be included as many times as necessary, without `Multiple Definitions` Linker Error
#include "RPi_Pico_TimerInterrupt.h"

#ifndef LED_BUILTIN
  #define LED_BUILTIN       25
#endif

#define PIN_D1              1         // Pin D1 mapped to pin GPIO1 of RPI_PICO

bool TimerHandler0(struct repeating_timer *t)
{ 
  (void) t;
  
  static bool toggle0 = false;
  static bool started = false;

  if (!started)
  {
    started = true;
    pinMode(LED_BUILTIN, OUTPUT);
  }

#if (TIMER_INTERRUPT_DEBUG > 0)
  Serial.print("ITimer0 called, millis() = "); Serial.println(millis());
#endif

  //timer interrupt toggles pin LED_BUILTIN
  digitalWrite(LED_BUILTIN, toggle0);
  toggle0 = !toggle0;

  return true;
}

bool TimerHandler1(struct repeating_timer *t)
{ 
  (void) t;
  
  static bool toggle1 = false;
  static bool started = false;

  if (!started)
  {
    started = true;
    pinMode(PIN_D1, OUTPUT);
  }

#if (TIMER_INTERRUPT_DEBUG > 0)
  Serial.print("ITimer1 called, millis() = "); Serial.println(millis());
#endif

  //timer interrupt toggles outputPin
  digitalWrite(PIN_D1, toggle1);
  toggle1 = !toggle1;

  return true;
}

#define TIMER0_INTERVAL_MS        1000
#define TIMER0_DURATION_MS        5000

#define TIMER1_INTERVAL_MS        3000
#define TIMER1_DURATION_MS        15000

// Init RPI_PICO_Timer, can use any from 0-15 pseudo-hardware timers
RPI_PICO_Timer ITimer0(0);
RPI_PICO_Timer ITimer1(1);

void setup()
{
  Serial.begin(115200);
  while (!Serial);
  
  delay(100);
  
  Serial.print(F("\nStarting TimerInterruptTest on ")); Serial.println(BOARD_NAME);
  Serial.println(RPI_PICO_TIMER_INTERRUPT_VERSION);
  Serial.print(F("CPU Frequency = ")); Serial.print(F_CPU / 1000000); Serial.println(F(" MHz"));

  // Interval in microsecs
  if (ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS * 1000, TimerHandler0))
  {
    Serial.print(F("Starting ITimer0 OK, millis() = ")); Serial.println(millis());
  }
  else
    Serial.println(F("Can't set ITimer0. Select another freq. or timer"));

  // Interval in microsecs
  if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS * 1000, TimerHandler1))
  {
    Serial.print(F("Starting ITimer1 OK, millis() = ")); Serial.println(millis());
  }
  else
    Serial.println(F("Can't set ITimer1. Select another freq. or timer"));

  Serial.flush();  
}

void loop()
{
  static unsigned long lastTimer0 = 0;
  static unsigned long lastTimer1 = 0;

  static bool timer0Stopped         = false;
  static bool timer1Stopped         = false;

  if (millis() - lastTimer0 > TIMER0_DURATION_MS)
  {
    lastTimer0 = millis();

    if (timer0Stopped)
    {
      Serial.print(F("Start ITimer0, millis() = ")); Serial.println(millis());
      ITimer0.restartTimer();
    }
    else
    {
      Serial.print(F("Stop ITimer0, millis() = ")); Serial.println(millis());
      ITimer0.stopTimer();
    }
    timer0Stopped = !timer0Stopped;
  }

  if (millis() - lastTimer1 > TIMER1_DURATION_MS)
  {
    lastTimer1 = millis();

    if (timer1Stopped)
    {
      Serial.print(F("Start ITimer1, millis() = ")); Serial.println(millis());
      ITimer1.restartTimer();
    }
    else
    {
      Serial.print(F("Stop ITimer1, millis() = ")); Serial.println(millis());
      ITimer1.stopTimer();
    }
    
    timer1Stopped = !timer1Stopped;
  }
}
