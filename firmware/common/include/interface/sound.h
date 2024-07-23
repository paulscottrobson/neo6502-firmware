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
	uint16_t timeCS;
	int16_t slide;
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

typedef struct _sound_update {
	int frequency;
	int slide;
	int timeCS;
} SOUND_UPDATE;

void SNDInitialise(void);
void SNDUpdateSoundChannel(uint8_t channel,SOUND_CHANNEL *c);
void SNDManager(void);

void SNDResetAll(void);
uint8_t SNDResetChannel(int channel);
uint8_t SNDPlay(int channelID,SOUND_UPDATE *u);
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
