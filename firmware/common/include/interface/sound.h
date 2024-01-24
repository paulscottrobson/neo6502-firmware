// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      sound.h
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      21st November 2023
//      Reviewed :  No
//      Purpose :   Audio support for Neo6502
//
// ***************************************************************************************
// ***************************************************************************************

#ifndef _XSOUND_H
#define _XSOUND_H

#define SOUND_CHANNELS 	(1)
#define SOUND_QUEUE_SIZE (32)

typedef struct _sound_queue_item {
	uint16_t frequency;
	uint16_t timeMS;
	uint16_t slide;
	uint8_t  soundType;
} SOUND_QUEUE_ELEMENT;

typedef struct _sound_channel {
	int  currentFrequency;
	int  currentSlide;
	bool isPlayingNote;
	int  tick50Remaining;
	int  queueCount;
	SOUND_QUEUE_ELEMENT queue[SOUND_QUEUE_SIZE];
} SOUND_CHANNEL;

void SNDInitialise(void);
void SNDSetFrequency(uint8_t channel,uint16_t frequency,bool isNoise);
void SNDManager(void);

void SNDResetAll(void);
uint8_t SNDResetChannel(int channel);
uint8_t SNDPlay(int channelID,uint16_t frequency,uint16_t timems,uint16_t slide,bool isNoise);
void SNDStartup(void);
int SNDGetNoteCount(int channelID);

uint8_t SFXPlay(int channelID,int effect);
#endif

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
