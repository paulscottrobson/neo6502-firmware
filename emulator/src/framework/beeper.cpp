// *******************************************************************************************************************************
// *******************************************************************************************************************************
//
//		Name:		beeper.cpp
//		Purpose:	SoundSupport library for SDL.
//		Created:	12th February 2024.
//		Author:		qxxxb (https://github.com/qxxxb/sdl2-beeper)
//					Paul Robson (paul@robsons.org.uk)
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
const int wave_length = sample_rate / wave_freq;
const int channel = 1;
std::vector<int16_t> wave_samples;
Mix_Chunk wave_chunk{};

void Beeper::open() {
	printf("[Beeper] Open\n");
	Mix_OpenAudio(sample_rate, AUDIO_S16SYS, 1, 4096);
	
	for (int i = 0; i < wave_length; ++i)
	{
		wave_samples.push_back(i < wave_length / 2 ? std::numeric_limits<std::int16_t>::max() : std::numeric_limits<std::int16_t>::min());
	}
	wave_chunk.abuf = (std::uint8_t*) wave_samples.data();
	wave_chunk.alen = 2 * wave_samples.size();
	wave_chunk.volume = MIX_MAX_VOLUME;
	const int channel = 1;
	Mix_PlayChannel(channel, &wave_chunk, -1);
}

void Beeper::close() {
	Mix_HaltChannel(channel);
	Mix_CloseAudio();
	Mix_Quit();
	printf("[Beeper] Close\n");
}

void Beeper::setFrequency(double frequency) {
	printf("[Beeper] Freq %f\n",frequency);
}

void Beeper::setVolume(double volume) {
	printf("[Beeper] Vol %f\n",volume);
}

void Beeper::play() {
	printf("[Beeper] Play\n");
}

void Beeper::stop() {
	printf("[Beeper] Stop\n");
}

