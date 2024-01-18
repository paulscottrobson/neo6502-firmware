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
	#ifdef PICO
	SNDPlay(0,220,500,0,false);
	SNDPlay(0,440,250,0,false);
	#endif
}

// ***************************************************************************************
//
//								Return number of pending notes
//
// ***************************************************************************************

int SNDGetNoteCount(int channelID) {
	if (channelID < 0 || channelID > SOUND_CHANNELS) return -1;
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

uint8_t SNDPlay(int channelID,uint16_t frequency,uint16_t timems,uint16_t slide,bool isNoise) {
	//printf("%d %d %d %d %d\n",channelID,frequency,timems,slide,isNoise);
	if (channelID >= SOUND_CHANNELS) return 1;
	SOUND_CHANNEL *c = &channel[channelID];
	if (c->queueCount != SOUND_QUEUE_SIZE) {  									// If queue not full
		SOUND_QUEUE_ELEMENT *qe = &(c->queue[c->queueCount]);  					// Add to queue.
		qe->frequency = frequency;
		qe->slide = slide/20;  													// Adjust to 50Hz tick rate
		if (slide != 0 && qe->slide == 0) qe->slide = (slide & 0x8000) ? -1:1;  // Rounded to zero.
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
				if (c->currentSlide != 0) {
					c->currentFrequency += c->currentSlide;
					if (c->currentFrequency < 200) c->currentFrequency += 1000;
					if (c->currentFrequency > 1200) c->currentFrequency -= 1000;
					SNDSetFrequency(channelID,c->currentFrequency,false);
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
