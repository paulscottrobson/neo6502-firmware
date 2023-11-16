/****************************************************************************************************************************
  Argument_Complex.ino
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

// Init RPI_PICO_Timer
RPI_PICO_Timer ITimer1(1);

struct pinStruct
{
  unsigned int Pin1;
  unsigned int Pin2;
  unsigned int Pin3;
};

volatile pinStruct myOutputPins = { LED_BUILTIN, 0, 1 };

bool TimerHandler(struct repeating_timer *t)
{
  (void) t;
  
  static bool toggle = false;

  //timer interrupt toggles pins
#if (TIMER_INTERRUPT_DEBUG > 0)
  Serial.print("Toggle pin1 = "); Serial.println( myOutputPins.Pin1 );
#endif
  
  digitalWrite(myOutputPins.Pin1, toggle);

#if (TIMER_INTERRUPT_DEBUG > 0)
  Serial.print("Read pin2  ("); Serial.print( myOutputPins.Pin2 );
  Serial.print(") = ");
  Serial.println(digitalRead(myOutputPins.Pin2) ? "HIGH" : "LOW" );                          

  Serial.print("Read pin3  ("); Serial.print( myOutputPins.Pin1 );
  Serial.print(") = ");
  Serial.println(digitalRead(myOutputPins.Pin3) ? "HIGH" : "LOW" );  
#endif
                 
  toggle = !toggle;

  return true;
}

#define TIMER_INTERVAL_MS    1000

void setup()
{
  pinMode(myOutputPins.Pin1, OUTPUT);
  pinMode(myOutputPins.Pin2, OUTPUT);
  pinMode(myOutputPins.Pin3, OUTPUT);
    
  Serial.begin(115200);
  while (!Serial);

  Serial.print(F("\nStarting Argument_Complex on ")); Serial.println(BOARD_NAME);
  Serial.println(RPI_PICO_TIMER_INTERRUPT_VERSION);
  //Serial.print(F("CPU Frequency = ")); Serial.print(F_CPU / 1000000); Serial.println(F(" MHz"));

  // Interval in microsecs
  if (ITimer1.attachInterruptInterval(TIMER_INTERVAL_MS  * 1000, TimerHandler))
  {
    Serial.print(F("Starting ITimer1 OK, millis() = ")); Serial.println(millis());
  }
  else
    Serial.println(F("Can't set ITimer1. Select another freq. or timer"));
}

void loop()
{
}
