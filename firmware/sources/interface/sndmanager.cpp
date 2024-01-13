// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      sndmanager.cpp
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      9th January 2024
//      Reviewed :  No
//      Purpose :   Sound Manager
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

static bool isInitialised = false;
static SOUND_CHANNEL channel[SOUND_CHANNELS];

// ***************************************************************************************
//
//								Reset all channels
//
// ***************************************************************************************

void SNDResetAll(void) {
	for (int i = 0;i < SOUND_CHANNELS;i++) {
		SNDResetChannel(i);
	}
}

// ***************************************************************************************
//
//							Reset a specific channel
//
// ***************************************************************************************

uint8_t SNDResetChannel(int channelID) {
	if (channelID >= SOUND_CHANNELS) return 1;
	SNDSetFrequency(channelID,0,false); 				 						// Sound off
	SOUND_CHANNEL *c = &channel[channelID];  									// Initialise channel
	c->isPlayingNote = false;
	c->tick50Remaining = 0;
	c->queueCount = 0;
	return 0;
}

// ***************************************************************************************
//
//								Sound startup beep
//
// ***************************************************************************************

void SNDStartup(void) {
	SNDPlay(0,220,500,false);
	SNDPlay(0,440,250,false);
}

// ***************************************************************************************
//
//								Play the next note, dequeuing
//
// ***************************************************************************************

void SNDPlayNextNote(int channelID) {
	SOUND_CHANNEL *c = &channel[channelID];
	if (c->queueCount == 0 || c->isPlayingNote) return;  						// Queue empty / note being played.
	SOUND_QUEUE_ELEMENT *qe = &(c->queue[0]); 									// Head of the queue.
	c->isPlayingNote = true;  													// Set up the channel data
	c->tick50Remaining = qe->timeMS / 20;  
	SNDSetFrequency(channelID,qe->frequency,false);								// Play the note.
	c->queueCount--; 															// Dequeue
	for (int i = 0;i < c->queueCount;i++) {
		c->queue[i] = c->queue[i+1];
	}
}

// ***************************************************************************************
//
//								Insert note in a channel
//
// ***************************************************************************************

uint8_t SNDPlay(int channelID,uint16_t frequency,uint16_t timems,bool isNoise) {
	//printf("%d %d %d %d\n",channelID,frequency,timems,isNoise);
	if (channelID >= SOUND_CHANNELS) return 1;
	SOUND_CHANNEL *c = &channel[channelID];
	if (c->queueCount != SOUND_QUEUE_SIZE) {  									// If queue not full
		SOUND_QUEUE_ELEMENT *qe = &(c->queue[c->queueCount]);  					// Add to queue.
		qe->frequency = frequency;
		qe->timeMS = timems;
		qe->soundType = 0;
		c->queueCount++;
	}
	if (!c->isPlayingNote) {  													// Not playing anything, try a new note.
		SNDPlayNextNote(channelID);
	}
	return 0;
}

// ***************************************************************************************
//
//									Called at 50 Hz
//
// ***************************************************************************************

void SNDManager(void) {
	if (!isInitialised) {  														// Initialise first time.
		SNDResetAll();
		isInitialised = true;
	}
	for (int channelID = 0;channelID < SOUND_CHANNELS;channelID++) {  			// Process each.
		SOUND_CHANNEL *c = &channel[channelID];
		if (c->isPlayingNote) {  												// Playing a note.
			if (c->tick50Remaining == 0) {  									// End of note ?
				c->isPlayingNote = false;  										// Now not playing note.
				if (c->queueCount == 0) { 										// Queue is empty ?
					SNDSetFrequency(channelID,0,false);							// Silence the channel.
				} else {
					SNDPlayNextNote(channelID); 								// Queue has data, play next note ?
				}
			} else {  															// Decrement the timer.
				c->tick50Remaining--;
			}
		}
	}
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
