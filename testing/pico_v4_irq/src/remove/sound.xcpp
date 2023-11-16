// 
// 
// 

#include "sound.h"

#define _PWM_LOGLEVEL_          0
#define TIMER_INTERRUPT_DEBUG   0
#define TIMER0_INTERVAL_MS      1000

#include "pins.h"
#include "RP2040_PWM.h"
#include "RPi_Pico_TimerInterrupt.h"
#include <cppQueue.h>

#define PLAY_INTRO

//creates pwm instance
RP2040_PWM* PWM_Instance;
RPI_PICO_Timer ITimer0(0);

float gFrequency = 440;
float gDutyCycle = 50;
uint8_t gBPM = 120;
uint8_t gNPS = 4;  // whole note

//
typedef struct _sTone {
  char     Tone[4];
  uint16_t Freq;
} sTone;

//
typedef struct _sNote{
  char     Note[4];
  uint8_t  Duration;
} sNote;

sNote gNote;

//
cppQueue	sound_queue(sizeof(sNote), 64, FIFO);	// Instantiate queue

static sTone tones[] = {
  { "P" , 0 },
  { "B0" , 31 },
  { "C1" , 33 },
  { "CS1" , 35 },
  { "D1" , 37 },
  { "DS1" , 39 },
  { "E1" , 41 },
  { "F1" , 44 },
  { "FS1" , 46 },
  { "G1" , 49 },
  { "GS1" , 52 },
  { "A1" , 55 },
  { "AS1" , 58 },
  { "B1" , 62 },
  { "C2" , 65 },
  { "CS2" , 69 },
  { "D2" , 73 },
  { "DS2" , 78 },
  { "E2" , 82 },
  { "F2" , 87 },
  { "FS2" , 93 },
  { "G2" , 98 },
  { "GS2" , 104 },
  { "A2" , 110 },
  { "AS2" , 117 },
  { "B2" , 123 },
  { "C3" , 131 },
  { "CS3" , 139 },
  { "D3" , 147 },
  { "DS3" , 156 },
  { "E3" , 165 },
  { "F3" , 175 },
  { "FS3" , 185 },
  { "G3" , 196 },
  { "GS3" , 208 },
  { "A3" , 220 },
  { "AS3" , 233 },
  { "B3" , 247 },
  { "C4" , 262 },
  { "CS4" , 277 },
  { "D4" , 294 },
  { "DS4" , 311 },
  { "E4" , 330 },
  { "F4" , 349 },
  { "FS4" , 370 },
  { "G4" , 392 },
  { "GS4" , 415 },
  { "A4" , 440 },
  { "AS4" , 466 },
  { "B4" , 494 },
  { "C5" , 523 },
  { "CS5" , 554 },
  { "D5" , 587 },
  { "DS5" , 622 },
  { "E5" , 659 },
  { "F5" , 698 },
  { "FS5" , 740 },
  { "G5" , 784 },
  { "GS5" , 831 },
  { "A5" , 880 },
  { "AS5" , 932 },
  { "B5" , 988 },
  { "C6" , 1047 },
  { "CS6" , 1109 },
  { "D6" , 1175 },
  { "DS6" , 1245 },
  { "E6" , 1319 },
  { "F6" , 1397 },
  { "FS6" , 1480 },
  { "G6" , 1568 },
  { "GS6" , 1661 },
  { "A6" , 1760 },
  { "AS6" , 1865 },
  { "B6" , 1976 },
  { "C7" , 2093 },
  { "CS7" , 2217 },
  { "D7" , 2349 },
  { "DS7" , 2489 },
  { "E7" , 2637 },
  { "F7" , 2794 },
  { "FS7" , 2960 },
  { "G7" , 3136 },
  { "GS7" , 3322 },
  { "A7" , 3520 },
  { "AS7" , 3729 },
  { "B7" , 3951 },
  { "C8" , 4186 },
  { "CS8" , 4435 },
  { "D8" , 4699 },
  { "DS8" , 4978 },
  { "" , 10000}
};

/// <summary>
/// 
/// </summary>
/// <param name="vTone"></param>
/// <returns></returns>
uint16_t getFreq(String vTone) {
  uint8_t t = 0;

  while (tones[t].Freq < 10000) {
    if (strcmp(tones[t].Tone, vTone.c_str()) == 0)
      return tones[t].Freq;
    t++;
  }

  return 0;
}

/// <summary>
/// 
/// </summary>
/// <param name="t"></param>
/// <returns></returns>
bool TimerHandler(struct repeating_timer* t)
{
  (void)t;

  sNote n;

  if (!sound_queue.isEmpty()) {

    sound_queue.pull(&n);

//    Serial.printf("Note [%s][%d]\n", n.Note, n.Duration);

    gFrequency = getFreq(n.Note);

    if (gFrequency > 0)
      PWM_Instance->setPWM(uP_BUZZ, gFrequency, gDutyCycle);
    else
      PWM_Instance->setPWM(uP_BUZZ, 440, 0);

    ITimer0.setInterval(6000000L * n.Duration / gBPM * gNPS, TimerHandler);
  }
  else {
    PWM_Instance->setPWM(uP_BUZZ, 440, 0);
    ITimer0.setInterval(1000 * 1000, TimerHandler);
  }

  return true;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
uint8_t SoundQueueIsEmpty() {
  return sound_queue.isEmpty();
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
uint8_t SoundQueueIsFull() {
  return sound_queue.isFull();
}

/// <summary>
/// 
/// </summary>
/// <param name="vNote"></param>
void SoundSetNote(uint8_t vNote) {
  strcpy(gNote.Note, tones[vNote].Tone);
}

/// <summary>
/// 
/// </summary>
/// <param name="vDur"></param>
void SoundSetDuration(uint8_t vDur) {
  gNote.Duration = vDur;
}

/// <summary>
/// 
/// </summary>
/// <param name="vTone"></param>
/// <param name="vDuration"></param>
/// <returns></returns>
boolean pushNote(String vNote, uint8_t vDuration) {
  sNote n;

  strcpy(n.Note, vNote.c_str());
  n.Duration = vDuration;

  sound_queue.push(&n);
  return sound_queue.isFull();
}

/// <summary>
/// 
/// </summary>
void SoundReset() {
  //  Serial.println("SOUND: reset");
  sound_queue.clean();
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
boolean SoundPushTheNote() {
//  Serial.printf("SOUND: play %s %02d\n", gNote.Note, gNote.Duration);
  if ((gNote.Note[0] == 'P') and (gNote.Duration == 0)) {
    SoundReset();
    return true;
  }
  else
    return pushNote(gNote.Note, gNote.Duration);
}

#ifdef PLAY_INTRO
//
sNote sIntro[] = {
  {"C3", 1},
  {"G3", 1},
  {"E3", 1},
  {"C4", 2},
  {"P",  1}
};
#endif

/// <summary>
/// 
/// </summary>
void initSound() {
  // Interval in microsecs
  if (ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS * 1000, TimerHandler)) {
    Serial.print(F("SOUND driver installed\n"));
  }
  else
    Serial.println(F("Can't set Timer. Select another freq"));

  PWM_Instance = new RP2040_PWM(uP_BUZZ, 440, 0);

#ifdef PLAY_INTRO
  // play intro
  for (uint8_t i = 0; i < 5; i++)
    pushNote(sIntro[i].Note, sIntro[i].Duration);
#endif
}
