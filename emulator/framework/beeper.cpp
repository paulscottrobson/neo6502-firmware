// *******************************************************************************************************************************
// *******************************************************************************************************************************
//
//		Name:		beeper.cpp
//		Purpose:	SoundSupport library for SDL.
//		Created:	22nd November 2023
//		Author:		Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************************************************
// *******************************************************************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <ctype.h>
#include "gfx.h"
#include <queue>
#include <cmath>
#include "sys_processor.h"
#include <hardware.h>

#ifdef EMSCRIPTEN
#include "emscripten.h"
#endif

// *******************************************************************************************************************************
//
//													Audio : 3 channel + noise.
//
// *******************************************************************************************************************************

const int AMPLITUDE = 9000;
const int FREQUENCY = 44100;

void audio_callback(void*, Uint8*, int);

Beeper::Beeper()
{ 
}

void Beeper::setup(void) {
    SDL_AudioSpec desiredSpec;
    SDL_zero(desiredSpec);
    desiredSpec.freq = FREQUENCY;
    desiredSpec.format = AUDIO_S16LSB;
    desiredSpec.channels = 1;
    desiredSpec.samples = 512;
    desiredSpec.callback = audio_callback;
    desiredSpec.userdata = this;

    SDL_AudioSpec obtainedSpec;
    dev = SDL_OpenAudioDevice(NULL,0,&desiredSpec, &obtainedSpec,0);
    if (dev == 0) 
    	printf("Couldn't open audio %s\n",SDL_GetError());
    if (desiredSpec.format != obtainedSpec.format) 
    	printf("Wrong format. %x %x\n",desiredSpec.format,obtainedSpec.format);
    SDL_PauseAudioDevice(dev,0);
 	freq1 = 0;
}

Beeper::~Beeper()
{
    SDL_CloseAudioDevice(dev);
}

void Beeper::generateSamples(Sint16 *stream, int length)
{
    int i = 0;
    for (int n = 0;n < length;n++) stream[n] = 0;
    while (i < length) {
    	if (freq1 != 0) {
    		stream[i] += AMPLITUDE * ((((int)(v1*2/FREQUENCY)) % 2) ? -1 : 1);
    	    v1 += freq1;
        }
        i++;
    }
}

void Beeper::setFrequency(double f,int channel) {
	if (channel == 1) freq1 = f;
}

void audio_callback(void *_beeper, Uint8 *_stream, int _length)
{
    Sint16 *stream = (Sint16*) _stream;
    int length = _length / 2;
    Beeper* beeper = (Beeper*) _beeper;
    beeper->generateSamples(stream, length);
}

