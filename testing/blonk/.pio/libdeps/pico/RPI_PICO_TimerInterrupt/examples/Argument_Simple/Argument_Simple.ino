/****************************************************************************************************************************
  Argument_Simple.ino
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

// These define's must be placed at the beginning before #include "TimerInterrupt_Generic.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         1
#define _TIMERINTERRUPT_LOGLEVEL_     4

// Can be included as many times as necessary, without `Multiple Definitions` Linker Error
#include "RPi_Pico_TimerInterrupt.h"

#if !defined(LED_BUILTIN)
  #define LED_BUILTIN     25
#endif

unsigned int outputPin1 = LED_BUILTIN;
unsigned int outputPin2 = 1;

// Init RPI_PICO_Timer
RPI_PICO_Timer ITimer1(1);
RPI_PICO_Timer ITimer2(2);

#define TIMER1_INTERVAL_MS    1000

bool TimerHandler1(struct repeating_timer *t)
{
  (void) t;
  
  static bool toggle1 = false;

#if (TIMER_INTERRUPT_DEBUG > 0)
  //timer interrupt toggles pin outputPin1
  Serial.print("Pin"); Serial.print(outputPin1); Serial.println(toggle1 ? F(" ON") : F(" OFF"));
#endif
  
  digitalWrite(outputPin1, toggle1);
  toggle1 = !toggle1;

  return true;
}

#define TIMER2_INTERVAL_MS    2000

bool TimerHandler2(struct repeating_timer *t)
{
  (void) t;
  
  static bool toggle2 = false;

#if (TIMER_INTERRUPT_DEBUG > 0)
  //timer interrupt toggles pin outputPin2
  Serial.print("Pin"); Serial.print(outputPin2); Serial.println(toggle2 ? F(" ON") : F(" OFF"));
#endif

  //timer interrupt toggles pin outputPin2
  digitalWrite(outputPin2, toggle2);
  toggle2 = !toggle2;

  return true;
}

void setup()
{
  pinMode(outputPin1, OUTPUT);
  pinMode(outputPin2, OUTPUT);
  
  Serial.begin(115200);
  while (!Serial);

  Serial.print(F("\nStarting Argument_Simple on ")); Serial.println(BOARD_NAME);
  Serial.println(RPI_PICO_TIMER_INTERRUPT_VERSION);
  Serial.print(F("CPU Frequency = ")); Serial.print(F_CPU / 1000000); Serial.println(F(" MHz"));

  if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS * 1000, TimerHandler1))
  {
    Serial.print(F("Starting ITimer1 OK, millis() = ")); Serial.println(millis());

#if (TIMER_INTERRUPT_DEBUG > 1)    
    Serial.print(F("OutputPin1 = ")); Serial.print(outputPin1);
#endif    
  }
  else
    Serial.println(F("Can't set ITimer1. Select another freq. or timer"));


  if (ITimer2.attachInterruptInterval(TIMER2_INTERVAL_MS * 1000, TimerHandler2))
  {
    Serial.print(F("Starting  ITimer2 OK, millis() = ")); Serial.println(millis());

#if (TIMER_INTERRUPT_DEBUG > 1)    
    Serial.print(F("OutputPin2 = ")); Serial.print(outputPin2);
#endif    
  }
  else
    Serial.println(F("Can't set ITimer2. Select another freq. or timer"));
}

void loop()
{
}
