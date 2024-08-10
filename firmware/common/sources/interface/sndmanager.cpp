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
static SOUND_CHANNEL channel[SOUND_CHANNELS_MAX];

// ***************************************************************************************
//
//								Reset all channels
//
// ***************************************************************************************

void SNDResetAll(void) {
	for (int i = 0;i < SNDGetChannelCount();i++) {
		SNDResetChannel(i);
	}
}

// ***************************************************************************************
//
//							Reset a specific channel
//
// ***************************************************************************************

uint8_t SNDResetChannel(int channelID) {
	if (channelID >= SNDGetChannelCount()) return 1;
	SOUND_CHANNEL *c = &channel[channelID];  									// Initialise channel
	c->isPlayingNote = false;
	c->tick50Remaining = 0;
	c->queueCount = 0;
	SNDUpdateSoundChannel(channelID,c); 				 						// Sound off
	return 0;
}

// ***************************************************************************************
//
//								Sound startup beep
//
// ***************************************************************************************

void SNDStartup(void) {
	#ifdef PICO
	SOUND_UPDATE u;
	u.frequency = 220;u.timeCS = 50;u.slide = 0;
	SNDPlay(0,&u);
	u.frequency = 440;u.timeCS = 25;u.slide = 0;
	SNDPlay(0,&u);
	#endif
}

// ***************************************************************************************
//
//								Return number of pending notes
//
// ***************************************************************************************

int SNDGetNoteCount(int channelID) {
	if (channelID < 0 || channelID > SNDGetChannelCount()) return -1;
	SOUND_CHANNEL *c = &channel[channelID];
	return c->queueCount + (c->isPlayingNote ? 1 : 0);  						// # in queue + 1 if playing
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
	c->currentFrequency = qe->frequency;
	c->currentSlide = qe->slide;
	c->currentType = qe->type;
	c->currentVolume = qe->volume;
	c->isPlayingNote = true;  													// Set up the channel data
	c->tick50Remaining = qe->timeCS / 2;  
	SNDUpdateSoundChannel(channelID,c);  										// Update it
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

uint8_t SNDPlay(int channelID,SOUND_UPDATE *u) {
	if (channelID >= SNDGetChannelCount()) return 1;
	SOUND_CHANNEL *c = &channel[channelID];
	if (c->queueCount != SOUND_QUEUE_SIZE) {  									// If queue not full
		SOUND_QUEUE_ELEMENT *qe = &(c->queue[c->queueCount]);  					// Add to queue.
		qe->frequency = u->frequency;		
		qe->slide = (u->slide & 0x8000) ? u->slide/2-0x8000:u->slide/2;  		// Adjust to 50Hz tick rate
		if (u->slide != 0 && qe->slide == 0) { 									// Rounded to zero.
			qe->slide = (u->slide & 0x8000)?-1:1;
		}
		qe->timeCS = u->timeCS;
		qe->type = u->type;
		qe->volume = u->volume;
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
	for (int channelID = 0;channelID < SNDGetChannelCount();channelID++) {  	// Process each.
		SOUND_CHANNEL *c = &channel[channelID];
		if (c->isPlayingNote) {  												// Playing a note.
			if (c->tick50Remaining == 0) {  									// End of note ?
				c->isPlayingNote = false;  										// Now not playing note.
				if (c->queueCount == 0) { 										// Queue is empty ?
					if (c->currentFrequency > 4000) c->currentFrequency = 0;
					SNDUpdateSoundChannel(channelID,c);							// Silence the channel.
				} else {
					SNDPlayNextNote(channelID); 								// Queue has data, play next note ?
				}
			} else {  															// Decrement the timer.
				c->tick50Remaining--;
				if (c->currentSlide != 0) {
					c->currentFrequency += c->currentSlide;
					if (c->currentFrequency < 100) c->currentFrequency += 1100;
					if (c->currentFrequency > 1200) c->currentFrequency -= 1100;
					if (c->currentFrequency > 4000) c->currentFrequency = 0;
					SNDUpdateSoundChannel(channelID,c);
				}
			}
		}
	}
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
// 		13/01/24 	Beep only on Pico version.
//
// ***************************************************************************************
