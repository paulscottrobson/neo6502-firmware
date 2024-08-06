// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      sound.cpp
//      Authors :   Paul Robson (paul@robsons.org.uk)
//					Harry Fairhead
//      Date :      21st November 2023
//      Reviewed :  No
//      Purpose :   Audio support for Neo6502
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"
#include "stdlib.h"
#include "hardware/pwm.h"
#include "dvi.h"
#include "system/dvi_video.h"

#define AUDIO_PIN 	(20) 					// Beeper pin.

// static uint sliceNumber,channel;

// ***************************************************************************************
//
//						Set up code, borrowed from Harry Fairhead
//
// ***************************************************************************************

// static int32_t SNDSetPWMFrequencyDuty(uint slice_num,uint chan,uint32_t f, int d)
// {
// 	// uint32_t clock = DVI_TIMING.bit_clk_khz * 1024;
// 	// uint32_t divider16 = clock / f / 4096 + (clock % (f * 4096) != 0);
// 	// if (divider16 / 16 == 0) divider16 = 16;
// 	// uint32_t wrap = clock * 16 / divider16 / f - 1;
// 	// pwm_set_clkdiv_int_frac(slice_num, divider16/16,divider16 & 0xF);
// 	// pwm_set_wrap(slice_num, wrap);
// 	// pwm_set_chan_level(slice_num, chan, wrap * d / 100);
// 	// return wrap;
// }

static int wav_position;

void pwm_interrupt_handler() {
    wav_position++;
    pwm_clear_irq(pwm_gpio_to_slice_num(AUDIO_PIN));    
    pwm_set_gpio_level(AUDIO_PIN,(wav_position & 0x100) ? 0x00:0xFF);
}

// ***************************************************************************************
//
//				Initialise sound channel, return # of supported channels
//
// ***************************************************************************************

void SNDInitialise(void) {
    gpio_set_function(AUDIO_PIN, GPIO_FUNC_PWM);
    int audio_pin_slice = pwm_gpio_to_slice_num(AUDIO_PIN);
    // Setup PWM interrupt to fire when PWM cycle is complete
    pwm_clear_irq(audio_pin_slice);
    pwm_set_irq_enabled(audio_pin_slice, true);
    // set the handle function above
    irq_set_exclusive_handler(PWM_IRQ_WRAP, pwm_interrupt_handler); 
    irq_set_enabled(PWM_IRQ_WRAP, true);
    // Setup PWM for audio output
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 8.0f); 
    pwm_config_set_wrap(&config, 250); 
    pwm_init(audio_pin_slice, &config, true);
    //
    pwm_set_gpio_level(AUDIO_PIN, 0);
	// gpio_set_function(SOUND_PIN, GPIO_FUNC_PWM);
	// sliceNumber = pwm_gpio_to_slice_num(SOUND_PIN);
	// channel = pwm_gpio_to_channel(SOUND_PIN);
	// pwm_set_enabled(sliceNumber,false);
}

// ***************************************************************************************
//
//									Play note on channel
//
// ***************************************************************************************

void SNDUpdateSoundChannel(uint8_t channel,SOUND_CHANNEL *c) {
	// if (channel < SOUND_CHANNELS) {
	// 	SNDSetPWMFrequencyDuty(sliceNumber,channel, c->currentFrequency, 50);
	// 	pwm_set_enabled(sliceNumber,c->isPlayingNote);
	// }
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//		16-01-24	Moved tick callback stuff to tick source for generalised usage
//
// ***************************************************************************************
