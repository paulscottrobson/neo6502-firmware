// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      sndcreator.cpp
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      7th August 2024
//      Reviewed :  No
//      Purpose :   Waveform generator, default sound system.
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

static int adder = 0;
static int wrapper = 0;
static int state = 0;
static int soundType = 0;
static int level = 0;
static int volume = 127;

// ***************************************************************************************
//
//            Return number of channels supported by this implementation
//
// ***************************************************************************************

int SNDGetChannelCount(void) {
    return 1;
}

void SNDMuteAllChannels(void) {    
}

// ***************************************************************************************
//
//            	Get the next sample for the driver provided hardware rate.
//
// ***************************************************************************************

int16_t SNDGetNextSample(void) {

    if (adder == 0) return 0;

    if (wrapper++ >= adder) {
        wrapper = 0;
        state = state ^ 0xFF;
        level = state ? volume : -volume;
        if (soundType == SOUNDTYPE_NOISE) {
            level = rand() & 0xFF;
        }
	}      
  	return level;
}

// ***************************************************************************************
//
//									Play note on channel
//
// ***************************************************************************************

void SNDUpdateSoundChannel(uint8_t channel,SOUND_CHANNEL *c) {
    if (c->isPlayingNote) {    
        adder = SNDGetSampleFrequency() / c->currentFrequency / 2;
        soundType = c->currentType;
    } else {
        adder = 0;
    }
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
