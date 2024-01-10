// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      sfxmanager.c
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      9th January 2024
//      Reviewed :  No
//      Purpose :   Sound Effects Manager
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

#include "data/_sfxdata.h"

// ***************************************************************************************
//
//									Play a sound effect
//
// ***************************************************************************************

uint8_t SFXPlay(int channelID,int effect) {
	if (channelID >= SOUND_CHANNELS) return 1;  								// Check legal channel/sfx
	if (effect >= SFX_COUNT) return 2;
	const uint16_t *notes = sfxData[effect];  									// List to queue
	while (*notes != 0xFFFF) {   												// Queue them
		SNDPlay(channelID,notes[0],notes[1],false);
		notes += 2;
	}
	return 0;

}
// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
