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

#include "tinysid/sid.h"

#include "interface/console.h"

#include <format>


#define SOUND_PIN 	(20) 					// Beeper pin.

//static uint sliceNumber,channel;


/*
 * PWM Interrupt Handler which outputs PWM level and advances the 
 * current sample. 
 * 
 * We repeat the same value for 8 cycles this means sample rate etc
 * adjust by factor of 8   (this is what bitshifting <<3 is doing)
 * 
 */

void pwm_interrupt_handler() {
    pwm_clear_irq(pwm_gpio_to_slice_num(SOUND_PIN));
	// SIDExecute();
	//sid_write(18, 0xFF, 0, false);
    uint16_t buffer;
    SIDCalcBuffer((uint8_t*)&buffer, 2);
	// CONWriteHex(((uint16_t*)buffer)[0]);
	// CONWriteHex(((uint16_t*)buffer)[1]);
	// CONWriteHex(((uint16_t*)buffer)[2]);
	// CONWriteHex(((uint16_t*)buffer)[3]);
    // if(buffer[0] != 0x0)
    // {
    //     CONWriteHex(*(uint16_t*)buffer);
    //     CONWrite('\r');
    // }
	//pwm_set_gpio_level(SOUND_PIN, buffer[0]);
    pwm_set_gpio_level(SOUND_PIN, buffer);
}

// ***************************************************************************************
//
//						Set up code, borrowed from Harry Fairhead
//
// ***************************************************************************************

// static int32_t SNDSetPWMFrequencyDuty(uint slice_num,uint chan,uint32_t f, int d)
// {
// 	uint32_t clock = DVI_TIMING.bit_clk_khz * 1024;
// 	uint32_t divider16 = clock / f / 4096 + (clock % (f * 4096) != 0);
// 	if (divider16 / 16 == 0) divider16 = 16;
// 	uint32_t wrap = clock * 16 / divider16 / f - 1;
// 	pwm_set_clkdiv_int_frac(slice_num, divider16/16,divider16 & 0xF);
// 	pwm_set_wrap(slice_num, wrap);
// 	pwm_set_chan_level(slice_num, chan, wrap * d / 100);
// 	return wrap;
// }



// ***************************************************************************************
//
//				Initialise sound channel, return # of supported channels
//
// ***************************************************************************************

void SNDInitialise(void) {
	CONWriteString("SIDInit\r");
	SIDInit();
	gpio_set_function(SOUND_PIN, GPIO_FUNC_PWM);

    int audio_pin_slice = pwm_gpio_to_slice_num(SOUND_PIN);

    // Setup PWM interrupt to fire when PWM cycle is complete
    pwm_clear_irq(audio_pin_slice);
    pwm_set_irq_enabled(audio_pin_slice, true);
    // set the handle function above
    irq_set_exclusive_handler(PWM_IRQ_WRAP, pwm_interrupt_handler); 
    irq_set_enabled(PWM_IRQ_WRAP, true);

    // Setup PWM for audio output
    pwm_config config = pwm_get_default_config();
    /* Base clock 176,000,000 Hz divide by wrap 250 then the clock divider further divides
     * to set the interrupt rate. 
     * 
     * 11 KHz is fine for speech. Phone lines generally sample at 8 KHz
     * 
     * 
     * So clkdiv should be as follows for given sample rate
     *  8.0f for 11 KHz
     *  4.0f for 22 KHz
     *  2.0f for 44 KHz etc
     */
    pwm_config_set_clkdiv(&config, 23.0f); 
    pwm_config_set_wrap(&config, 250); 
    pwm_init(audio_pin_slice, &config, true);

    pwm_set_gpio_level(SOUND_PIN, 0);

}

// ***************************************************************************************
//
//									Play note on channel
//
// ***************************************************************************************

void SNDUpdateSoundChannel(uint8_t channel,SOUND_CHANNEL *c) {
// 	if (channel < SOUND_CHANNELS) {
// 		SNDSetPWMFrequencyDuty(sliceNumber,channel, c->currentFrequency, 50);
// 		pwm_set_enabled(sliceNumber,c->isPlayingNote);
// 	}
// void SNDSetFrequency(uint8_t channel,uint16_t frequency,bool isNoise) {
// 	// if (channel < SOUND_CHANNELS) {
// 	// 	SNDSetPWMFrequencyDuty(sliceNumber,channel, frequency, 50);
// 	// 	pwm_set_enabled(sliceNumber,(frequency != 0));
// 	// }
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//		16-01-24	Moved tick callback stuff to tick source for generalised usage
//
// ***************************************************************************************
