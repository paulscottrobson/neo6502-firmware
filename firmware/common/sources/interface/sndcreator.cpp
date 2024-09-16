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
#include "interface/whitenoise.h"

#define CHANNEL_COUNT   (4)

struct _ChannelStatus {
    int limit;
    int adder;
    int wrapper;
    int state;
    int soundType;
    int volume;
    int samplePos;
} audio[CHANNEL_COUNT];

// ***************************************************************************************
//
//            Return number of channels supported by this implementation
//
// ***************************************************************************************

int SNDGetChannelCount(void) {
    return CHANNEL_COUNT;
}

// ***************************************************************************************
//
//                                  Mute all channels
//
// ***************************************************************************************

void SNDMuteAllChannels(void) {    
    for (int i = 0;i < CHANNEL_COUNT;i++) {
        struct _ChannelStatus *cs = &audio[i];
        cs->limit = cs->wrapper = cs->state = cs->soundType = cs->volume = 0;
    }
}

// ***************************************************************************************
//
//            	Get the next sample for the driver provided hardware rate.
//
// ***************************************************************************************

int16_t SNDGetNextSample(void) {

    static int activeCount = 0;

    int level = 0;                                                                  // Summative limit
    int channelsActive = activeCount;                                               // We have a very simple form of AGC, more than one channel scales volume
    activeCount = 0;                                                                // Reset the count

    for (int i = 0;i < CHANNEL_COUNT;i++) {                                         // Scan the channels
        struct _ChannelStatus *cs = &audio[i];                                          
        if (cs->volume != 0) {                                                      // Channel on.
            activeCount++;                                                          // Bump active count
            cs->wrapper = cs->wrapper + cs->adder;
            if (cs->wrapper >= cs->limit) {                                         // Time to change the output level.
                cs->wrapper = cs->wrapper - cs->limit;                              // Fix up the new limit.
                cs->state ^= 0xFF;
                switch (cs->soundType) {
                    case SOUNDTYPE_NOISE:   
                        level += ((noisePattern[cs->samplePos] - 0x80) * cs->volume / 128);
                        if (++cs->samplePos == NOISE_SIZE) cs->samplePos = 0;
                        break;
                    default:                                                        // Square wave
                        level += cs->state ? cs->volume : -cs->volume;break;
                }
            }
        }
	}      
    if (channelsActive > 1) {                                                       // If >= 2 channels scale output by 75% to reduce clipping.
        level = level * 3 / 4;  
    }
    if (level < -127) level = -127;                                                 // Clip into range
    if (level > 127) level = 127;
  	return level;
}

// ***************************************************************************************
//
//									Play note on channel
//
// ***************************************************************************************

void SNDUpdateSoundChannel(uint8_t channel,SOUND_CHANNEL *c) {
    if (c->isPlayingNote && c->currentFrequency != 0) {  
        //
        //      This doesn't work. It warbles on the real hardware. Why ?
        //  
        audio[channel].limit = 1000000;
        audio[channel].adder = (int)(1000000.0 * c->currentFrequency / SNDGetSampleFrequency() * 2);
        //
        //      This does work.
        //
        audio[channel].adder = 1;
        audio[channel].limit = SNDGetSampleFrequency()/c->currentFrequency/2;

        audio[channel].soundType = c->currentType;
        audio[channel].volume = c->currentVolume;
        audio[channel].samplePos = 0;
    } else {
        audio[channel].volume = 0;
    }
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
