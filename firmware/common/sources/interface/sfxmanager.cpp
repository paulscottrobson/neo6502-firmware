// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      sfxmanager.cpp
//      Authors :   Paul Robson (paul@robsons.org.uk)
//				   	Turi Scandurra
//      Date :      9th January 2024
//      Reviewed :  No
//      Purpose :   Sound Effects Manager
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

#include "data/sfxdata.h"

// ***************************************************************************************
//
//									Play a sound effect
//
// ***************************************************************************************

uint8_t SFXPlay(int channelID,int effect) {
	if (channelID >= SNDGetChannelCount()) return 1;  							// Check legal channel/sfx
	if (effect >= SFX_COUNT) return 2;
	const uint16_t *notes = sfxData[effect];  									// List to queue
	while (*notes != 0xFFFF) {   												// Queue them
		SOUND_UPDATE u;
		u.timeCS = notes[1];u.slide = 0;u.volume = 100;u.type = 0;
		switch(notes[0]) {
			case 0xFFFE:
				u.frequency = 100;u.type = 1;
				break;
			case 0xFFFD:
				u.frequency = 200;u.type = 1;u.slide = 40;	
				break;
			default:
				u.frequency = notes[0];
				break;
		}
		SNDPlay(channelID,&u);		
		notes += 2;
	}
	return 0;
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//		15-01-24 	Added sliding option.
//
// ***************************************************************************************
