// *******************************************************************************************************************************
// *******************************************************************************************************************************
//
//		Name:		beeper.cpp
//		Purpose:	SoundSupport library for SDL.
//		Created:	7th April 2024
//		Author:		Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************************************************
// *******************************************************************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <cstdint>
#include <memory.h>
#include <ctype.h>
#include "gfx.h"
#include <queue>
#include <cmath>
#include "sys_processor.h"
#include <hardware.h>
#include "SDL_mixer.h"

#ifdef EMSCRIPTEN
#include "emscripten.h"
#endif

// *******************************************************************************************************************************
//
//													Audio : 3 channel + noise.
//
// *******************************************************************************************************************************

#include <iostream>
#include <string>
#include <math.h>

const int sample_rate = 44100;
const int wave_freq = 440;
const int channel = 1;
int16_t wave_samples[44100];

Mix_Chunk wave_chunk{};
bool channelPlaying;

void SetWaveFrequency(uint16_t frequency) {
	int wave_length = sample_rate / frequency;
	for (int i = 0; i < wave_length; ++i)
	{
		wave_samples[i] = (i < wave_length / 2 ? -32768:32767);
	}
	wave_chunk.abuf = (std::uint8_t*) wave_samples;
	wave_chunk.alen = 2 * wave_length;
}	

void Beeper::open() {
	printf("[Beeper] Open\n");
	Mix_OpenAudio(sample_rate, AUDIO_S16SYS, 1, 4096);
	channelPlaying = false;
}

void Beeper::close() {
	if (channelPlaying) Mix_HaltChannel(channel);
	Mix_CloseAudio();
	Mix_Quit();
	printf("[Beeper] Close\n");
}

void Beeper::setFrequency(double frequency) {
	if (channelPlaying) Mix_HaltChannel(channel);
	printf("[Beeper] Freq %f\n",frequency);
	SetWaveFrequency(frequency);
	Mix_PlayChannel(channel, &wave_chunk, -1);
	channelPlaying = true;
}

void Beeper::setVolume(double volume) {
	printf("[Beeper] Vol %f\n",volume);
	wave_chunk.volume = (volume > 0.5) ? MIX_MAX_VOLUME:0;
}

void Beeper::play() {
	printf("[Beeper] Play\n");
}

void Beeper::stop() {
	printf("[Beeper] Stop\n");
}
