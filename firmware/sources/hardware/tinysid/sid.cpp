/*
 *  sid.cpp - 6581 SID emulation
 *
 *  Frodo (C) Copyright 1994-2004 Christian Bauer
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include <time.h>
#include <pico/stdlib.h>
#include <cstdint>
#include "interface/console.h"

#include "sys.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "sid.h"
#include "main.h"
#include "prefs.h"

#include "mem.h"
#include "cpu.h"

#define DEBUG 0
#include "debug.h"

#include "fixedpointmath.h"


// Some constants
const fp8p24_t FP8P24_0 = itofp8p24(0);
const fp8p24_t FP8P24_1 = itofp8p24(1);
const fp8p24_t FP8P24_2 = itofp8p24(2);
const fp8p24_t FP8P24_4 = itofp8p24(4);

const fp24p8_t FP24P8_0 = itofp24p8(0);
const fp24p8_t FP24P8_1 = itofp24p8(1);

const fp16p16_t FP16P16_PI = dtofp16p16(3.14159265358979323846);

// Cosine in degrees XXX replace with lut
// cos(deg), where deg is 16.16 and return is 8.24
#define FP8P24_COS_DEG_FP16P16(x) (dtofp8p24(cos(fp16p16tod(mulfp16p16(FP16P16_PI, x)))))

// Desired and obtained audio formats
typedef struct{
  int freq;
  uint16_t format;
  uint8_t channels;
  uint8_t silence;
  uint16_t samples;
  uint32_t size;
  void (*callback)(void *userdata, uint8_t *stream, int len);
  void *userdata;
} SDL_AudioSpec;

static SDL_AudioSpec desired, obtained;

// Flag: emulate SID filters
static bool enable_filters = true;

// Flag: emulate 2 SID chips
static bool dual_sid = false;

// Flag: emulate new SID chip (8580)
static bool emulate_8580 = false;

// Audio effect type (0 = none, 1 = reverb, 2 = spatial)
static int audio_effect = 0;

// Master volume (0..0x100)
static int32 master_volume;

// Volumes (0..0x100) and panning (-0x100..0x100) of voices 1..4 (both SIDs)
static int32 v1_volume, v2_volume, v3_volume, v4_volume;
static int32 v1_panning, v2_panning, v3_panning, v4_panning;

// Dual-SID stereo separation (0..0x100)
static int32 dual_sep;

// Number of SID clocks per sample frame
static uint32_t sid_cycles;        // Integer
static fp24p8_t sid_cycles_frac;    // With fractional part

// Phi2 clock frequency
static cycle_t cycles_per_second;
const fp24p8_t PAL_CLOCK = ftofp24p8(985248.444);
const fp24p8_t NTSC_OLD_CLOCK = ftofp24p8(1000000.0);
const fp24p8_t NTSC_CLOCK = ftofp24p8(1022727.143);

// Replay counter variables
static uint16_t cia_timer;        // CIA timer A latch
static int replay_count;        // Counter for timing replay routine
static int speed_adjust;        // Speed adjustment in percent

// Clock frequency changed
void SIDClockFreqChanged();

// Resonance frequency polynomials
// We use real floats here because it only runs once at startup and so the
// speed hit is acceptable. Too much precision is lost otherwise.
static inline ufp16p16_t CALC_RESONANCE_LP(float f)
{
    return ftoufp16p16(227.755 - 1.7635 * f - 0.0176385 * f * f + 0.00333484 * f * f * f);
}

static inline ufp16p16_t CALC_RESONANCE_HP(float f)
{
    return ftoufp16p16(366.374 - 14.0052 * f + 0.603212 * f * f - 0.000880196 * f * f * f);
}

// Pseudo-random number generator for SID noise waveform (don't use f_rand()
// because the SID waveform calculation runs asynchronously and the output of
// f_rand() has to be predictable inside the main emulation)
static uint32_t noise_rand_seed = 1;

inline static uint8_t noise_rand()
{
    // This is not the original SID noise algorithm (which is unefficient to
    // implement in software) but this sounds close enough
    noise_rand_seed = noise_rand_seed * 1103515245 + 12345;
    return noise_rand_seed >> 16;
}

// SID waveforms
enum {
    WAVE_NONE,
    WAVE_TRI,
    WAVE_SAW,
    WAVE_TRISAW,
    WAVE_RECT,
    WAVE_TRIRECT,
    WAVE_SAWRECT,
    WAVE_TRISAWRECT,
    WAVE_NOISE
};

// EG states
enum {
    EG_IDLE,
    EG_ATTACK,
    EG_DECAY,
    EG_RELEASE
};

// Filter types
enum {
    FILT_NONE,
    FILT_LP,
    FILT_BP,
    FILT_LPBP,
    FILT_HP,
    FILT_NOTCH,
    FILT_HPBP,
    FILT_ALL
};

// Voice 4 states (SIDPlayer only)
enum {
    V4_OFF,
    V4_GALWAY_NOISE,
    V4_SAMPLE
};

// Structure for one voice
typedef struct voice_t voice_t;
struct voice_t {
    int wave;            // Selected waveform
    int eg_state;        // Current state of EG
    voice_t *mod_by;    // Voice that modulates this one
    voice_t *mod_to;    // Voice that is modulated by this one

    uint32_t count;        // Counter for waveform generator, 8.16 fixed
    uint32_t add;            // Added to counter in every sample frame

    uint16_t freq;        // SID frequency value
    uint16_t pw;            // SID pulse-width value

    uint32_t a_add;        // EG parameters
    uint32_t d_sub;
    uint32_t s_level;
    uint32_t r_sub;
    uint32_t eg_level;    // Current EG level, 8.16 fixed

    uint32_t noise;        // Last noise generator output value

    uint16_t left_gain;    // Gain on left channel (12.4 fixed)
    uint16_t right_gain;    // Gain on right channel (12.4 fixed)

    bool gate;            // EG gate bit
    bool ring;            // Ring modulation bit
    bool test;            // Test bit
    bool filter;        // Flag: Voice filtered

                        // The following bit is set for the modulating
                        // voice, not for the modulated one (as the SID bits)
    bool sync;            // Sync modulation bit
    bool mute;            // Voice muted (voice 3 only)
};

// Data structures for both SIDs
typedef struct osid_t osid_t;
struct osid_t {
    int sid_num;                        // SID number (0 or 1)

    voice_t voice[3];                    // Data for 3 voices

    uint8_t regs[128];                    // Copies of the 25 write-only SID registers (SIDPlayer uses 128 registers)
    uint8_t last_written_byte;            // Byte last written to SID (for emulation of read accesses to write-only registers)
    uint8_t volume;                        // Master volume (0..15)

    uint8_t f_type;                        // Filter type
    uint8_t f_freq;                        // SID filter frequency (upper 8 bits)
    uint8_t f_res;                        // Filter resonance (0..15)

    fp8p24_t f_ampl;                        // IIR filter input attenuation
    fp8p24_t d1, d2, g1, g2;                // IIR filter coefficients
    fp24p8_t xn1_l, xn2_l, yn1_l, yn2_l;    // IIR filter previous input/output signal (left and right channel)
    fp24p8_t xn1_r, xn2_r, yn1_r, yn2_r;

    uint16_t v4_left_gain;                // Gain of voice 4 on left channel (12.4 fixed)
    uint16_t v4_right_gain;                // Gain of voice 4 on right channel (12.4 fixed)

    int v4_state;                        // State of voice 4 (Galway noise/samples)
    uint32_t v4_count;                    // Counter for voice 4
    uint32_t v4_add;                        // Added to counter in every frame

    uint16_t gn_adr;                        // C64 address of tone list
    uint16_t gn_tone_length;                // Length of each tone in samples
    uint32_t gn_volume_add;                // Added to SID volume reg. for every sample
    int    gn_tone_counter;                // Number of tones in list
    uint16_t gn_base_cycles;                // Cycles before sample
    uint16_t gn_loop_cycles;                // Cycles between samples
    uint32_t gn_last_count;                // Value of v4_count (lower 16 bits cleared) at end of tone

    uint32_t sm_adr;                        // C64 nybble address of sample
    uint32_t sm_end_adr;                    // C64 nybble address of end of sample
    uint32_t sm_rep_adr;                    // C64 nybble address of sample repeat point
    uint16_t sm_volume;                    // Sample volume (0..2, 0=loudest)
    uint8_t sm_rep_count;                    // Sample repeat counter (0xff=continous)
    bool sm_big_endian;                    // Flag: Sample is big-endian
};
osid_t *sid1 = NULL;

void osid_reset(osid_t *sid);
uint32_t osid_read(osid_t *sid, uint32_t adr, cycle_t now);
void osid_write(osid_t *sid, uint32_t adr, uint32_t byte, cycle_t now, bool rmw);
void osid_calc_gains(osid_t *sid, bool is_left_sid, bool is_right_sid);
void osid_calc_filter(osid_t *sid);
void osid_chunk_read(osid_t *sid, size_t size);
void osid_chunk_write(osid_t *sid);

static void osid_calc_gain_voice(int32 volume, int32 panning, uint16_t *left_gain, uint16_t *right_gain);

// Waveform tables
static uint16_t tri_table[0x1000*2];
static const uint16_t *tri_saw_table;
static const uint16_t *tri_rect_table;
static const uint16_t *saw_rect_table;
static const uint16_t *tri_saw_rect_table;

// Sampled from a 6581R4
static const uint16_t tri_saw_table_6581[0x100] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x0808,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x1010, 0x3C3C,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x0808,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x1010, 0x3C3C
};

static const uint16_t tri_rect_table_6581[0x100] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x8080,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x8080,
    0, 0, 0, 0, 0, 0, 0x8080, 0xC0C0, 0, 0x8080, 0x8080, 0xE0E0, 0x8080, 0xE0E0, 0xF0F0, 0xFCFC,
    0xFFFF, 0xFCFC, 0xFAFA, 0xF0F0, 0xF6F6, 0xE0E0, 0xE0E0, 0x8080, 0xEEEE, 0xE0E0, 0xE0E0, 0x8080, 0xC0C0, 0, 0, 0,
    0xDEDE, 0xC0C0, 0xC0C0, 0, 0x8080, 0, 0, 0, 0x8080, 0, 0, 0, 0, 0, 0, 0,
    0xBEBE, 0x8080, 0x8080, 0, 0x8080, 0, 0, 0, 0x8080, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0x7E7E, 0x4040, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static const uint16_t saw_rect_table_6581[0x100] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x7878,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x7878
};

static const uint16_t tri_saw_rect_table_6581[0x100] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// Sampled from an 8580R5
static const uint16_t tri_saw_table_8580[0x100] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x0808,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x1818, 0x3C3C,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x1C1C,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x8080, 0, 0x8080, 0x8080,
    0xC0C0, 0xC0C0, 0xC0C0, 0xC0C0, 0xC0C0, 0xC0C0, 0xC0C0, 0xE0E0, 0xF0F0, 0xF0F0, 0xF0F0, 0xF0F0, 0xF8F8, 0xF8F8, 0xFCFC, 0xFEFE
};

static const uint16_t tri_rect_table_8580[0x100] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0xFFFF, 0xFCFC, 0xF8F8, 0xF0F0, 0xF4F4, 0xF0F0, 0xF0F0, 0xE0E0, 0xECEC, 0xE0E0, 0xE0E0, 0xC0C0, 0xE0E0, 0xC0C0, 0xC0C0, 0xC0C0,
    0xDCDC, 0xC0C0, 0xC0C0, 0xC0C0, 0xC0C0, 0xC0C0, 0x8080, 0x8080, 0xC0C0, 0x8080, 0x8080, 0x8080, 0x8080, 0x8080, 0, 0,
    0xBEBE, 0xA0A0, 0x8080, 0x8080, 0x8080, 0x8080, 0x8080, 0, 0x8080, 0x8080, 0, 0, 0, 0, 0, 0,
    0x8080, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0x7E7E, 0x7070, 0x6060, 0, 0x4040, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static const uint16_t saw_rect_table_8580[0x100] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x8080,
    0, 0, 0, 0, 0, 0, 0x8080, 0x8080, 0, 0x8080, 0x8080, 0x8080, 0x8080, 0x8080, 0xB0B0, 0xBEBE,
    0, 0, 0, 0, 0, 0, 0, 0x8080, 0, 0, 0, 0x8080, 0x8080, 0x8080, 0x8080, 0xC0C0,
    0, 0x8080, 0x8080, 0x8080, 0x8080, 0x8080, 0x8080, 0xC0C0, 0x8080, 0x8080, 0xC0C0, 0xC0C0, 0xC0C0, 0xC0C0, 0xC0C0, 0xDCDC,
    0x8080, 0x8080, 0x8080, 0xC0C0, 0xC0C0, 0xC0C0, 0xC0C0, 0xC0C0, 0xC0C0, 0xC0C0, 0xC0C0, 0xE0E0, 0xE0E0, 0xE0E0, 0xE0E0, 0xECEC,
    0xC0C0, 0xE0E0, 0xE0E0, 0xE0E0, 0xE0E0, 0xF0F0, 0xF0F0, 0xF4F4, 0xF0F0, 0xF0F0, 0xF8F8, 0xF8F8, 0xF8F8, 0xFCFC, 0xFEFE, 0xFFFF
};

static const uint16_t tri_saw_rect_table_8580[0x100] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x8080, 0x8080,
    0x8080, 0x8080, 0x8080, 0x8080, 0x8080, 0x8080, 0xC0C0, 0xC0C0, 0xC0C0, 0xC0C0, 0xE0E0, 0xE0E0, 0xE0E0, 0xF0F0, 0xF8F8, 0xFCFC
};

// Envelope tables
static uint32_t eg_table[16];

static const uint8_t eg_dr_shift[256] = {
    5,5,5,5,5,5,5,5,4,4,4,4,4,4,4,4,
    3,3,3,3,3,3,3,3,3,3,3,3,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

// Filter tables
static fp16p16_t ffreq_lp[256];    // Low-pass resonance frequency table
static fp16p16_t ffreq_hp[256];    // High-pass resonance frequency table

// Table for sampled voices
static const uint16_t sample_tab[16 * 3] = {
    0x8000, 0x9111, 0xa222, 0xb333, 0xc444, 0xd555, 0xe666, 0xf777,
    0x0888, 0x1999, 0x2aaa, 0x3bbb, 0x4ccc, 0x5ddd, 0x6eee, 0x7fff,

    0xc444, 0xc444, 0xd555, 0xd555, 0xe666, 0xe666, 0xf777, 0xf777,
    0x0888, 0x0888, 0x1999, 0x1999, 0x2aaa, 0x2aaa, 0x3bbb, 0x3bbb,

    0xe666, 0xe666, 0xe666, 0xe666, 0xf777, 0xf777, 0xf777, 0xf777,
    0x0888, 0x0888, 0x0888, 0x0888, 0x1999, 0x1999, 0x1999, 0x1999
};

static int16_t galway_tab[16 * 64];

// Work buffer and variables for audio effects
#define WORK_BUFFER_SIZE 0x100
static int16_t work_buffer[WORK_BUFFER_SIZE];
static int wb_read_offset = 0, wb_write_offset = 0;
static int rev_feedback = 0;

// Prototypes
static void calc_buffer(void *userdata, uint8_t *buf, int count);


/*
 *  Init SID emulation
 */

void osid_init(osid_t *sid, int n)
{
    sid->sid_num = n;

    // Link voices
    sid->voice[0].mod_by = &sid->voice[2];
    sid->voice[1].mod_by = &sid->voice[0];
    sid->voice[2].mod_by = &sid->voice[1];
    sid->voice[0].mod_to = &sid->voice[1];
    sid->voice[1].mod_to = &sid->voice[2];
    sid->voice[2].mod_to = &sid->voice[0];

    osid_reset(sid);
}

static void set_desired_samples(int32 sample_rate)
{
    // Music replay doesn't need low latency
    desired.samples *= 8;
}

static void set_rev_delay(int32 delay_ms)
{
    int delay = (delay_ms * obtained.freq / 1000) & ~1;
    if (delay == 0)
        delay = 2;
    wb_read_offset = (wb_write_offset - delay) & (WORK_BUFFER_SIZE - 1);
}

static void calc_gains()
{
        osid_calc_gains(sid1, false, false);
}

static void set_sid_data()
{
    if (emulate_8580) {
        tri_saw_table = tri_saw_table_8580;
        tri_rect_table = tri_rect_table_8580;
        saw_rect_table = saw_rect_table_8580;
        tri_saw_rect_table = tri_saw_rect_table_8580;
    } else {
        tri_saw_table = tri_saw_table_6581;
        tri_rect_table = tri_rect_table_6581;
        saw_rect_table = saw_rect_table_6581;
        tri_saw_rect_table = tri_saw_rect_table_6581;
    }
}

static void set_cycles_per_second(const char *to)
{
    if (strncmp(to, "6569", 4) == 0)
        cycles_per_second = fp24p8toi(PAL_CLOCK);
    else if (strcmp(to, "6567R5") == 0)
        cycles_per_second = fp24p8toi(NTSC_OLD_CLOCK);
    else
        cycles_per_second = fp24p8toi(NTSC_CLOCK);
}

void SIDInit()
{
    CONWrite('A');
    int i, j;

    if (sid1) free(sid1);
    sid1 = (osid_t*)malloc(sizeof(osid_t));
    osid_init(sid1, 0);
    CONWrite('B');
    // Read preferences ("obtained" is set to have valid values in it if SDL_OpenAudio() fails)
    emulate_8580 = false;//(strncmp(PrefsFindString("sidtype", 0), "8580", 4) == 0);
    set_sid_data();
    desired.freq = 44100;//obtained.freq = PrefsFindInt32("samplerate");
    desired.format = 0;//obtained.format = PrefsFindBool("audio16bit") ? AUDIO_S16SYS : AUDIO_U8;
    desired.channels = 1;//obtained.channels = PrefsFindBool("stereo") ? 2 : 1;
    enable_filters = false;//PrefsFindBool("filters");
    dual_sid = false;//PrefsFindBool("dualsid");
    // PrefsSetCallbackString("sidtype", prefs_sidtype_changed);
    // PrefsSetCallbackInt32("samplerate", prefs_samplerate_changed);
    // PrefsSetCallbackBool("audio16bit", prefs_audio16bit_changed);
    // PrefsSetCallbackBool("stereo", prefs_stereo_changed);
    // PrefsSetCallbackBool("filters", prefs_filters_changed);
    // PrefsSetCallbackBool("dualsid", prefs_dualsid_changed);
    CONWrite('C');
    set_cycles_per_second("6569");
    speed_adjust = 100;//PrefsFindInt32("speed");
    // PrefsSetCallbackString("victype", prefs_victype_changed);
    // PrefsSetCallbackInt32("speed", prefs_speed_changed);

    audio_effect = 0;//PrefsFindInt32("audioeffect");
    rev_feedback = 0x50;//PrefsFindInt32("revfeedback");

    master_volume = 0x100;//PrefsFindInt32("volume");
    v1_volume = 0x100;//PrefsFindInt32("v1volume");
    v2_volume = 0x100;//PrefsFindInt32("v2volume");
    v3_volume = 0x100;//PrefsFindInt32("v3volume");
    v4_volume = 0x100;//PrefsFindInt32("v4volume");
    v1_panning = -0x40;//PrefsFindInt32("v1pan");
    v2_panning = 0;//PrefsFindInt32("v2pan");
    v3_panning = 0x40;//PrefsFindInt32("v3pan");
    v4_panning = 0;//PrefsFindInt32("v4pan");
    dual_sep = 0x80;//PrefsFindInt32("dualsep");
    calc_gains();
    CONWrite('D');
    // Set sample buffer size
    set_desired_samples(desired.freq);
    CONWrite('E');

    // Open audio device
    desired.callback = calc_buffer;
    desired.userdata = NULL;

    obtained = desired;

    // if (SDL_OpenAudio(&desired, &obtained) < 0) {
    //     fprintf(stderr, "Couldn't initialize audio (%s)\n", SDL_GetError());
    //     exit(1);
    // }

    // Convert reverb delay to sample frame count
    set_rev_delay(125);

    // Compute number of cycles per sample frame and envelope table
    CONWrite('X');
    SIDClockFreqChanged();
    CONWrite('F');

    // Compute triangle table
    for (i=0; i<0x1000; i++) {
        tri_table[i] = (i << 4) | (i >> 8);
        tri_table[0x1fff-i] = (i << 4) | (i >> 8);
    }

    // Compute filter tables
    for (i=0; i<256; i++) {
        ffreq_lp[i] = CALC_RESONANCE_LP(i);
        ffreq_hp[i] = CALC_RESONANCE_HP(i);
    }

    // Compute galway noise table
    for (i=0; i<16; i++)
        for (j=0; j<64; j++)
            galway_tab[i * 64 + j] = sample_tab[(i * j) & 0x0f];
    CONWrite('-');

    // sid1->voice[0].freq = 440;
    // sid1->voice[0].wave = WAVE_SAW;
    // sid1->voice[0].gate = true;
    // sid1->voice[0].a_add = 100;

    // sid1->voice[1].freq = 440;
    // sid1->voice[1].wave = WAVE_SAW;
    // sid1->voice[1].gate = true;
    // sid1->voice[1].add = 10000;
    //sid1->voice[0].test = true;
}


/*
 *  Exit SID emulation
 */

void SIDExit()
{
    //SDL_CloseAudio();

    if (sid1) free(sid1);
}


/*
 *  Reset SID emulation
 */

void osid_reset(osid_t *sid)
{
    memset(sid->regs, 0, sizeof(sid->regs));
    sid->last_written_byte = 0;

    sid->volume = 15;
    sid->regs[24] = 0x0f;

    int v;
    for (v=0; v<3; v++) {
        sid->voice[v].wave = WAVE_NONE;
        sid->voice[v].eg_state = EG_IDLE;
        sid->voice[v].count = sid->voice[v].add = 0;
        sid->voice[v].freq = sid->voice[v].pw = 0;
        sid->voice[v].eg_level = sid->voice[v].s_level = 0;
        sid->voice[v].a_add = sid->voice[v].d_sub = sid->voice[v].r_sub = eg_table[0];
        sid->voice[v].gate = sid->voice[v].ring = sid->voice[v].test = false;
        sid->voice[v].filter = sid->voice[v].sync = sid->voice[v].mute = false;
    }

    sid->f_type = FILT_NONE;
    sid->f_freq = sid->f_res = 0;
    sid->f_ampl = FP8P24_1;
    sid->d1 = sid->d2 = sid->g1 = sid->g2 = FP8P24_0;
    sid->xn1_l = sid->xn2_l = sid->yn1_l = sid->yn2_l = FP24P8_0;
    sid->xn1_r = sid->xn2_r = sid->yn1_r = sid->yn2_r = FP24P8_0;

    sid->v4_state = V4_OFF;
    sid->v4_count = sid->v4_add = 0;

    sid->gn_adr = sid->gn_tone_length = 0;
    sid->gn_volume_add = 0;
    sid->gn_tone_counter = 0;
    sid->gn_base_cycles = sid->gn_loop_cycles = 0;
    sid->gn_last_count = 0;

    sid->sm_adr = sid->sm_end_adr = sid->sm_rep_adr = 0;
    sid->sm_volume = 0;
    sid->sm_rep_count = 0;
    sid->sm_big_endian = false;
}

void SIDReset(cycle_t now)
{
    //SDL_LockAudio();
    osid_reset(sid1);

    memset(work_buffer, 0, sizeof(work_buffer));

    //SDL_UnlockAudio();
}


/*
 *  Clock overflow
 */

void SIDClockOverflow(cycle_t sub)
{
}


/*
 *  Clock frequency changed (result of VIC type change)
 */

void SIDClockFreqChanged()
{
    // Compute number of cycles per sample frame
    sid_cycles = cycles_per_second / obtained.freq;
    sid_cycles_frac = divfp24p8(itofp24p8(cycles_per_second), itofp24p8(obtained.freq));
    CONWrite('1');
    // Compute envelope table
    static const uint32_t div[16] = {
        9, 32, 63, 95, 149, 220, 267, 313,
        392, 977, 1954, 3126, 3906, 11720, 19531, 31251
    };
    int i;
    for (i=0; i<16; i++)
        eg_table[i] = (sid_cycles << 16) / div[i];
    CONWrite('.');
    // Recompute voice_t::add values
    osid_write(sid1, 0, sid1->regs[0], 0, false);
    osid_write(sid1, 7, sid1->regs[7], 0, false);
    osid_write(sid1, 14, sid1->regs[14], 0, false);
}


/*
 *  Set replay frequency
 */

void SIDSetReplayFreq(int freq)
{
    cia_timer = cycles_per_second / freq - 1;
}

/*
 *  Set speed adjustment
 */

void SIDAdjustSpeed(int percent)
{
    PrefsReplaceInt32("speed", percent);
}

/*
 *  Write to CIA timer A (changes replay frequency)
 */

void cia_tl_write(uint8_t byte)
{
    cia_timer = (cia_timer & 0xff00) | byte;
}

void cia_th_write(uint8_t byte)
{
    cia_timer = (cia_timer & 0x00ff) | (byte << 8);
}


/*
 *  Fill audio buffer with SID sound
 */

static void calc_sid(osid_t *sid, int32 *sum_output_left, int32 *sum_output_right)
{
    // Sampled voice (!! todo: gain/panning)
#if 0    //!!
    uint8_t master_volume = sid->sample_buf[(sample_count >> 16) % SAMPLE_BUF_SIZE];
    sample_count += ((0x138 * 50) << 16) / obtained.freq;
#else
    uint8_t master_volume = sid->volume;
#endif

    int32 sum_output_filter_left = 0, sum_output_filter_right = 0;

    // Loop for all three voices
    int j;
    for (j=0; j<1; j++) {
        voice_t *v = sid->voice + j;

        // Envelope generator
        uint16_t envelope;

        switch (v->eg_state) {
            case EG_ATTACK:
                //CONWriteString("ATTACK\r");
                v->eg_level += v->a_add;
                if (v->eg_level > 0xffffff) {
                    v->eg_level = 0xffffff;
                    v->eg_state = EG_DECAY;
                }
                break;
            case EG_DECAY:
                //CONWriteString("DECAY\r");
                if (v->eg_level <= v->s_level || v->eg_level > 0xffffff)
                    v->eg_level = v->s_level;
                else {
                    v->eg_level -= v->d_sub >> eg_dr_shift[v->eg_level >> 16];
                    if (v->eg_level <= v->s_level || v->eg_level > 0xffffff)
                        v->eg_level = v->s_level;
                }
                break;
            case EG_RELEASE:
                //CONWriteString("RELEASE\r");
                v->eg_level -= v->r_sub >> eg_dr_shift[v->eg_level >> 16];
                if (v->eg_level > 0xffffff) {
                    v->eg_level = 0;
                    v->eg_state = EG_IDLE;
                }
                break;
            case EG_IDLE:
                //CONWriteString("IDLE\r");
                v->eg_level = 0;
                break;
        }
        envelope = (v->eg_level * master_volume) >> 20;

        // Waveform generator
        uint16_t output;

        if (!v->test)
            v->count += v->add;

        if (v->sync && (v->count >= 0x1000000))
            v->mod_to->count = 0;

        v->count &= 0xffffff;

        switch (v->wave) {
            case WAVE_TRI:
                if (v->ring)
                    output = tri_table[(v->count ^ (v->mod_by->count & 0x800000)) >> 11];
                else
                    output = tri_table[v->count >> 11];
                break;
            case WAVE_SAW:
                output = v->count >> 8;
                break;
            case WAVE_RECT:
                if (v->count > (uint32)(v->pw << 12))
                    output = 0xffff;
                else
                    output = 0;
                break;
            case WAVE_TRISAW:
                output = tri_saw_table[v->count >> 16];
                break;
            case WAVE_TRIRECT:
                if (v->count > (uint32)(v->pw << 12))
                    output = tri_rect_table[v->count >> 16];
                else
                    output = 0;
                break;
            case WAVE_SAWRECT:
                if (v->count > (uint32)(v->pw << 12))
                    output = saw_rect_table[v->count >> 16];
                else
                    output = 0;
                break;
            case WAVE_TRISAWRECT:
                if (v->count > (uint32)(v->pw << 12))
                    output = tri_saw_rect_table[v->count >> 16];
                else
                    output = 0;
                break;
            case WAVE_NOISE:
                if (v->count >= 0x100000) {
                    output = v->noise = noise_rand() << 8;
                    v->count &= 0xfffff;
                } else
                    output = v->noise;
                break;
            default:
                output = 0x8000;
                break;
        }
        int32 x = (int16)(output ^ 0x8000) * envelope;
        if (v->filter) {
            sum_output_filter_left += (x * v->left_gain) >> 4;
            sum_output_filter_right += (x * v->right_gain) >> 4;
        } else if (!(v->mute)) {
            *sum_output_left += (x * v->left_gain) >> 4;
            *sum_output_right += (x * v->right_gain) >> 4;
        }
    }

    // Galway noise/samples
    int32 v4_output = 0;
    switch (sid->v4_state) {

        case V4_GALWAY_NOISE:
            v4_output = galway_tab[(sid->gn_volume_add << 6) + (((sid->gn_last_count + sid->v4_count) >> 16) & 0x3f)] << 8;
            sid->v4_count += sid->v4_add;
            if ((sid->v4_count >> 16) >= sid->gn_tone_length) {
                if (sid->gn_tone_counter) {
                    sid->gn_tone_counter--;
                    sid->gn_last_count = sid->v4_count & 0xffff0000;
                    sid->v4_count &= 0xffff;
                    int div = 0;//ram[sid->gn_adr + sid->gn_tone_counter] * sid->gn_loop_cycles + sid->gn_base_cycles;
                    if (div == 0)
                        sid->v4_add = 0;
                    else
                        sid->v4_add = sid_cycles * 0x10000 / div;
                } else
                    sid->v4_state = V4_OFF;
            }
            break;

        case V4_SAMPLE: {
            uint8_t sample = 0;//ram[sid->sm_adr >> 1];
            if (sid->sm_big_endian)
                if (sid->sm_adr & 1)
                    sample = sample & 0xf;
                else
                    sample = sample >> 4;
            else
                if (sid->sm_adr & 1)
                    sample = sample >> 4;
                else
                    sample = sample & 0xf;
            v4_output = sample_tab[sid->sm_volume * 16 + sample] << 8;
            sid->v4_count += sid->v4_add;
            sid->sm_adr += sid->v4_count >> 16;
            sid->v4_count &= 0xffff;
            if (sid->sm_adr >= sid->sm_end_adr) {
                if (sid->sm_rep_count) {
                    if (sid->sm_rep_count != 0xff)
                        sid->sm_rep_count--;
                    sid->sm_adr = sid->sm_rep_adr;
                } else
                    sid->v4_state = V4_OFF;
            }
            break;
        }
    }
    *sum_output_left += (v4_output * sid->v4_left_gain) >> 4;
    *sum_output_right += (v4_output * sid->v4_right_gain) >> 4;

    // Filter
    if (enable_filters) {
        //float xn = ((float) sum_output_filter_left) * sid->f_ampl;
        fp24p8_t xn = mulfp24p8(itofp24p8(sum_output_filter_left), sid->f_ampl);
        //float yn = xn + sid->d1 * sid->xn1_l + sid->d2 * sid->xn2_l - sid->g1 * sid->yn1_l - sid->g2 * sid->yn2_l;
        fp24p8_t yn = xn + mulfp24p8(sid->d1, sid->xn1_l) + mulfp24p8(sid->d2, sid->xn2_l) - mulfp24p8(sid->g1, sid->yn1_l) - mulfp24p8(sid->g2, sid->yn2_l);
        sum_output_filter_left = fp24p8toi(yn);
        sid->yn2_l = sid->yn1_l; sid->yn1_l = yn; sid->xn2_l = sid->xn1_l; sid->xn1_l = xn;

        //xn = ((float) sum_output_filter_right) * sid->f_ampl;
        xn = mulfp24p8(itofp24p8(sum_output_filter_right), sid->f_ampl);
        //yn = xn + sid->d1 * sid->xn1_r + sid->d2 * sid->xn2_r - sid->g1 * sid->yn1_r - sid->g2 * sid->yn2_r;
        yn = xn + mulfp24p8(sid->d1, sid->xn1_r) + mulfp24p8(sid->d2, sid->xn2_r) - mulfp24p8(sid->g1, sid->yn1_r) - mulfp24p8(sid->g2, sid->yn2_r);
        sum_output_filter_right = fp24p8toi(yn);
        sid->yn2_r = sid->yn1_r; sid->yn1_r = yn; sid->xn2_r = sid->xn1_r; sid->xn1_r = xn;
    }

    // Add filtered and non-filtered output
    *sum_output_left += sum_output_filter_left;
    *sum_output_right += sum_output_filter_right;
}

static void calc_buffer(void *userdata, uint8_t *buf, int count)
{
    uint16_t *buf16 = (uint16_t *)buf;

    //int replay_limit = (obtained.freq * 100) / (cycles_per_second / (cia_timer + 1) * speed_adjust);

    // Convert buffer length (in bytes) to frame count
    bool is_stereo = false;//(obtained.channels == 2);
    bool is_16_bit = true;//!(obtained.format == AUDIO_U8 || obtained.format == AUDIO_S8);
    if (is_stereo)
        count >>= 1;
    if (is_16_bit)
        count >>= 1;

    // Main calculation loop
    while (count--) {
        int32 sum_output_left = 0, sum_output_right = 0;

        // // Execute 6510 play routine if due
        // if (++replay_count >= replay_limit) {
        //     replay_count = 0;
        //     UpdatePlayAdr();
        //     CPUExecute(play_adr, 0, 0, 0, 1000000);
        // }

        // Calculate output of voices from both SIDs
        calc_sid(sid1, &sum_output_left, &sum_output_right);

        // Apply audio effects (post-processing)
        if (audio_effect) {
            sum_output_left >>= 11;
            sum_output_right >>= 11;
            if (audio_effect == 1) {    // Reverb
                sum_output_left += (rev_feedback * work_buffer[wb_read_offset++]) >> 8;
                work_buffer[wb_write_offset++] = sum_output_left;
                sum_output_right += (rev_feedback * work_buffer[wb_read_offset]) >> 8;
                work_buffer[wb_write_offset] = sum_output_right;
            } else {                    // Spatial
                sum_output_left += (rev_feedback * work_buffer[wb_read_offset++]) >> 8;
                work_buffer[wb_write_offset++] = sum_output_left;
                sum_output_right -= (rev_feedback * work_buffer[wb_read_offset]) >> 8;
                work_buffer[wb_write_offset] = sum_output_right;
            }
            wb_read_offset = (wb_read_offset + 1) & (WORK_BUFFER_SIZE - 1);
            wb_write_offset = (wb_write_offset + 1) & (WORK_BUFFER_SIZE - 1);
        } else {
            sum_output_left >>= 10;
            sum_output_right >>= 10;
        }

        // Clip to 16 bits
        if (sum_output_left > 32767)
            sum_output_left = 32767;
        else if (sum_output_left < -32768)
            sum_output_left = -32768;
        if (sum_output_right > 32767)
            sum_output_right = 32767;
        else if (sum_output_right < -32768)
            sum_output_right = -32768;

        // Write to output buffer
        if (is_16_bit) {
            if (is_stereo) {
                *buf16++ = sum_output_left;
                *buf16++ = sum_output_right;
            } else
                *buf16++ = (sum_output_left + sum_output_right) / 2;
        } else {
            if (is_stereo) {
                *buf++ = (sum_output_left >> 8) ^ 0x80;
                *buf++ = (sum_output_right >> 8) ^ 0x80;
            } else
                *buf++ = ((sum_output_left + sum_output_right) >> 9) ^ 0x80;
        }
    }
}

void SIDCalcBuffer(uint8_t *buf, int count)
{
    calc_buffer(NULL, buf, count);
}

uint64 replay_start_time = 0;    // Start time of last replay
int32 over_time = 0;            // Time the last replay was too long

void SIDExecute()
{

    // Delay to maintain proper replay frequency
    uint64 now = time_us_64();
    if (replay_start_time == 0)
        replay_start_time = now;
    uint32_t replay_time = now - replay_start_time;
    //uint32_t adj_nominal_replay_time = (uint32) ((cia_timer + 1) * 100000000.0 / (cycles_per_second * speed_adjust));
    uint32_t adj_nominal_replay_time = (cia_timer + 1) * 100000000 / (cycles_per_second * speed_adjust);
    int32 delay = adj_nominal_replay_time - replay_time - over_time;
    over_time = -delay;
    if (over_time < 0)
        over_time = 0;
    if (delay > 0) {
        sleep_us(delay);
        int32 actual_delay = time_us_64() - now;
        if (actual_delay + 500 < delay)
            sleep_us(1);
        actual_delay = time_us_64() - now;
        over_time += actual_delay - delay;
        if (over_time < 0)
            over_time = 0;
    }
    replay_start_time = time_us_64();

    // Execute 6510 play routine
    //UpdatePlayAdr();
    //CPUExecute(play_adr, 0, 0, 0, 1000000);
}


/*
 *  Calculate IIR filter coefficients
 */

void osid_calc_filter(osid_t *sid)
{
    // Filter off? Then reset all coefficients
    if (sid->f_type == FILT_NONE) {
        sid->f_ampl = FP8P24_0;
        sid->d1 = sid->d2 = sid->g1 = sid->g2 = FP24P8_0;
        return;
    }

    // Calculate resonance frequency
    ufp16p16_t fr;
    if (sid->f_type == FILT_LP || sid->f_type == FILT_LPBP)
        fr = ffreq_lp[sid->f_freq];
    else
        fr = ffreq_hp[sid->f_freq];

    // Limit to <1/2 sample frequency, avoid div by 0 in case FILT_NOTCH below
    //filt_t arg = fr / ((float) (obtained.freq >> 1));
    fp16p16_t arg = divufp16p16(fr, itofp16p16(obtained.freq >> 1));
    if (arg > ftofp16p16(0.99))
        arg = ftofp16p16(0.99);
    if (arg < ftofp16p16(0.01))
        arg = ftofp16p16(0.01);

    //fp8p24_t argfp8p24 = _fp16p16tofp8p24(arg);

    // Calculate poles (resonance frequency and resonance)
    // The (complex) poles are at
    //   zp_1/2 = (-g1 +/- sqrt(g1^2 - 4*g2)) / 2
    //sid->g2 = 0.55 + 1.2 * arg * arg - 1.2 * arg + ((float) sid->f_res) * 0.0133333333;
    
    sid->g2 = ftofp8p24(0.55) + _fp16p16tofp8p24(mulfp16p16(mulfp16p16(ftofp16p16(1.2), arg), arg)) - _fp16p16tofp8p24(mulfp16p16(ftofp16p16(1.2), arg)) + mulfp8p24(itofp8p24(sid->f_res), ftofp8p24(0.0133333333));

    //sid->g2 = ftofp24p8(0.55) + mulfp24p8(mulfp24p8(ftofp24p8(1.2), arg), arg) - mulfp24p8(ftofp24p8(1.2), arg) + mulfp24p8(itofp24p8(sid->f_res), ftofp24p8(0.0133333333));
    //sid->g1 = -2.0 * sqrt(sid->g2) * cos(M_PI * arg);
    sid->g1 = mulfp8p24(mulfp8p24(-FP8P24_2, sqrtufp8p24(sid->g2)), FP8P24_COS_DEG_FP16P16(arg));

    // Increase resonance if LP/HP combined with BP
    if (sid->f_type == FILT_LPBP || sid->f_type == FILT_HPBP)
        sid->g2 += ftofp8p24(0.1);

    // Stabilize filter
    if (abs(sid->g1) >= sid->g2 + FP8P24_1) {
        if (sid->g1 > FP8P24_0)
            sid->g1 = sid->g2 + ftofp8p24(0.99);
        else
            sid->g1 = -(sid->g2 + ftofp8p24(0.99));
    }

    // Calculate roots (filter characteristic) and input attenuation
    // The (complex) roots are at
    //   z0_1/2 = (-d1 +/- sqrt(d1^2 - 4*d2)) / 2
    switch (sid->f_type) {

        case FILT_LPBP:
        case FILT_LP:        // Both roots at -1, H(1)=1
            //sid->d1 = 2.0; sid->d2 = 1.0;
            sid->d1 = FP8P24_2; sid->d2 = FP8P24_1;
            //f_ampl = 0.25 * (1.0 + sid->g1 + sid->g2);
            sid->f_ampl = mulfp8p24(ftofp8p24(0.25), (FP8P24_1 + sid->g1 + sid->g2));
            break;

        case FILT_HPBP:
        case FILT_HP:        // Both roots at 1, H(-1)=1
            //sid->d1 = -2.0; sid->d2 = 1.0;
            sid->d1 = -FP8P24_2; sid->d2 = FP8P24_1;
            //f_ampl = 0.25 * (1.0 - sid->g1 + sid->g2);
            sid->f_ampl = mulfp24p8(ftofp8p24(0.25), (FP8P24_1 - sid->g1 + sid->g2));
            break;

        case FILT_BP: {        // Roots at +1 and -1, H_max=1
            sid->d1 = FP8P24_0; sid->d2 = -FP8P24_1;
            //float c = sqrt(sid->g2*sid->g2 + 2.0*sid->g2 - sid->g1*sid->g1 + 1.0);
            fp24p8_t c = sqrtufp8p24((mulfp8p24(sid->g2, sid->g2) + mulfp8p24(FP8P24_2, sid->g2) - mulfp8p24(sid->g1, sid->g1) + FP8P24_1));
            //sid->f_ampl = 0.25 * (-2.0*sid->g2*sid->g2 - (4.0+2.0*c)*sid->g2 - 2.0*c + (c+2.0)*sid->g1*sid->g1 - 2.0) / (-sid->g2*sid->g2 - (c+2.0)*sid->g2 - c + sid->g1*sid->g1 - 1.0);
            sid->f_ampl = divfp8p24(mulfp8p24(ftofp8p24(0.25), (mulfp8p24(mulfp8p24(-FP8P24_2, sid->g2), sid->g2) - mulfp8p24(FP8P24_4+mulfp8p24(FP8P24_2, c), sid->g2) - mulfp8p24(FP8P24_2, c) + mulfp8p24(mulfp8p24(c+FP8P24_2, sid->g1), sid->g1) - FP8P24_2)), mulfp8p24(-sid->g2, sid->g2) - mulfp8p24(c+FP8P24_2, sid->g2) - c + mulfp8p24(sid->g1, sid->g1) - FP8P24_1);
            break;
        }

        case FILT_NOTCH:    // Roots at exp(i*pi*arg) and exp(-i*pi*arg), H(1)=1 (arg>=0.5) or H(-1)=1 (arg<0.5)
            //sid->d1 = -2.0 * cos(M_PI * arg); sid->d2 = 1.0;
            sid->d1 = mulfp8p24(-FP8P24_2, FP8P24_COS_DEG_FP16P16(arg)); sid->d2 = FP8P24_1;
            if (arg >= ftofp8p24(0.5))
                //sid->f_ampl = 0.5 * (1.0 + sid->g1 + sid->g2) / (1.0 - cos(M_PI * arg));
                sid->f_ampl = divfp8p24(mulfp8p24(ftofp8p24(0.5), (FP8P24_1 + sid->g1 + sid->g2)), FP8P24_1 - FP8P24_COS_DEG_FP16P16(arg));
            else
                //sid->f_ampl = 0.5 * (1.0 - sid->g1 + sid->g2) / (1.0 + cos(M_PI * arg));
                sid->f_ampl = divfp8p24(mulfp8p24(ftofp8p24(0.5), (FP8P24_1 - sid->g1 + sid->g2)), FP8P24_1 + FP8P24_COS_DEG_FP16P16(arg));
            break;

        // The following is pure guesswork...
        case FILT_ALL:        // Roots at 2*exp(i*pi*arg) and 2*exp(-i*pi*arg), H(-1)=1 (arg>=0.5) or H(1)=1 (arg<0.5)
            //sid->d1 = -4.0 * cos(M_PI * arg); sid->d2 = 4.0;
            sid->d1 = mulfp8p24(-FP8P24_4, FP8P24_COS_DEG_FP16P16(arg)); sid->d2 = FP8P24_4;
            if (arg >= 0.5)
                //sid->f_ampl = (1.0 - sid->g1 + sid->g2) / (5.0 + 4.0 * cos(M_PI * arg));
                sid->f_ampl = divfp8p24(FP8P24_1 - sid->g1 + sid->g2, ftofp8p24(5.0) + mulfp8p24(FP8P24_4, FP8P24_COS_DEG_FP16P16(arg)));
            else
                //sid->f_ampl = (1.0 + sid->g1 + sid->g2) / (5.0 - 4.0 * cos(M_PI * arg));
                sid->f_ampl = divfp8p24(FP8P24_1 + sid->g1 + sid->g2, ftofp8p24(5.0) - mulfp8p24(FP8P24_4, FP8P24_COS_DEG_FP16P16(arg)));
            break;

        default:
            break;
    }
}


/*
 *  Calculate gain values for all voices
 */

static void osid_calc_gain_voice(int32 volume, int32 panning, uint16_t *left_gain, uint16_t *right_gain)
{
    int gain;
    if (panning < -0x100)
        panning = -0x100;
    if (panning > 0x100)
        panning = 0x100;
    gain = (volume * (-panning + 0x100) * master_volume) >> 20;
    if (gain > 0x200)
        gain = 0x200;
    if (gain < 0)
        gain = 0;
    *left_gain = gain;
    gain = (volume * (panning + 0x100) * master_volume) >> 20;
    if (gain > 0x200)
        gain = 0x200;
    if (gain < 0)
        gain = 0;
    *right_gain = gain;
}

void osid_calc_gains(osid_t *sid, bool is_left_sid, bool is_right_sid)
{
    int32 pan_offset = 0;
    if (is_left_sid)
        pan_offset = -dual_sep;
    else if (is_right_sid)
        pan_offset = dual_sep;
    osid_calc_gain_voice(v1_volume, v1_panning + pan_offset, &sid->voice[0].left_gain, &sid->voice[0].right_gain);
    osid_calc_gain_voice(v2_volume, v2_panning + pan_offset, &sid->voice[1].left_gain, &sid->voice[1].right_gain);
    osid_calc_gain_voice(v3_volume, v3_panning + pan_offset, &sid->voice[2].left_gain, &sid->voice[2].right_gain);
    osid_calc_gain_voice(v4_volume, v4_panning + pan_offset, &sid->v4_left_gain, &sid->v4_right_gain);
}


/*
 *  Read from SID register
 */

uint32_t osid_read(osid_t *sid, uint32_t adr, cycle_t now)
{
    D(bug("sid_read from %04x at cycle %d\n", adr, now));

    switch (adr) {
        case 0x19:    // A/D converters
        case 0x1a:
            sid->last_written_byte = 0;
            return 0xff;
        case 0x1b:    // Voice 3 oscillator/EG readout
        case 0x1c:
            sid->last_written_byte = 0;
            return f_rand();
        default: {    // Write-only register: return last value written to SID
            uint8_t ret = sid->last_written_byte;
            sid->last_written_byte = 0;
            return ret;
        }
    }
}

uint32_t sid_read(uint32_t adr, cycle_t now)
{
    return osid_read(sid1, adr & 0x7f, now);
}


/*
 *  Write to SID register
 */

void osid_write(osid_t *sid, uint32_t adr, uint32_t byte, cycle_t now, bool rmw)
{
    D(bug("sid_write %02x to %04x at cycle %d\n", byte, adr, now));

    // Writing to standard SID mirrored registers
    if ((adr & 0x1f) < 0x1d)
        adr &= 0x1f;

    sid->last_written_byte = sid->regs[adr] = byte;
    int v = adr/7;    // Voice number

    switch (adr) {
        case 0:
        case 7:
        case 14:
            sid->voice[v].freq = (sid->voice[v].freq & 0xff00) | byte;
            //sid->voice[v].add = (uint32) ((float) sid->voice[v].freq) * sid_cycles_frac;
            sid->voice[v].add = fp24p8toi(mulfp24p8(itofp24p8(sid->voice[v].freq), sid_cycles_frac));
            break;

        case 1:
        case 8:
        case 15:
            sid->voice[v].freq = (sid->voice[v].freq & 0xff) | (byte << 8);
            //sid->voice[v].add = (uint32) ((float) sid->voice[v].freq) * sid_cycles_frac;
            sid->voice[v].add = fp24p8toi(mulfp24p8(itofp24p8(sid->voice[v].freq), sid_cycles_frac));
            break;

        case 2:
        case 9:
        case 16:
            sid->voice[v].pw = (sid->voice[v].pw & 0x0f00) | byte;
            break;

        case 3:
        case 10:
        case 17:
            sid->voice[v].pw = (sid->voice[v].pw & 0xff) | ((byte & 0xf) << 8);
            break;

        case 4:
        case 11:
        case 18:
            sid->voice[v].wave = (byte >> 4) & 0xf;
            if ((byte & 1) != sid->voice[v].gate) {
                if (byte & 1)    // Gate turned on
                    sid->voice[v].eg_state = EG_ATTACK;
                else            // Gate turned off
                    if (sid->voice[v].eg_state != EG_IDLE)
                        sid->voice[v].eg_state = EG_RELEASE;
            }
            sid->voice[v].gate = byte & 1;
            sid->voice[v].mod_by->sync = byte & 2;
            sid->voice[v].ring = byte & 4;
            if ((sid->voice[v].test = byte & 8))
                sid->voice[v].count = 0;
            break;

        case 5:
        case 12:
        case 19:
            sid->voice[v].a_add = eg_table[byte >> 4];
            sid->voice[v].d_sub = eg_table[byte & 0xf];
            break;

        case 6:
        case 13:
        case 20:
            sid->voice[v].s_level = (byte >> 4) * 0x111111;
            sid->voice[v].r_sub = eg_table[byte & 0xf];
            break;

        case 22:
            if (byte != sid->f_freq) {
                sid->f_freq = byte;
                if (enable_filters)
                    osid_calc_filter(sid);
            }
            break;

        case 23:
            sid->voice[0].filter = byte & 1;
            sid->voice[1].filter = byte & 2;
            sid->voice[2].filter = byte & 4;
            if ((byte >> 4) != sid->f_res) {
                sid->f_res = byte >> 4;
                if (enable_filters)
                    osid_calc_filter(sid);
            }
            break;

        case 24:
            sid->volume = byte & 0xf;
            sid->voice[2].mute = byte & 0x80;
            if (((byte >> 4) & 7) != sid->f_type) {
                sid->f_type = (byte >> 4) & 7;
                sid->xn1_l = sid->xn2_l = sid->yn1_l = sid->yn2_l = FP24P8_0;
                sid->xn1_r = sid->xn2_r = sid->yn1_r = sid->yn2_r = FP24P8_0;
                if (enable_filters)
                    osid_calc_filter(sid);
            }
            break;

        case 29:
            if (byte) {
                if (byte < 0xfc) {            // Galway noise
                    sid->gn_adr = (sid->regs[0x1f] << 8) | sid->regs[0x1e];
                    sid->gn_tone_length = sid->regs[0x3d];
                    sid->gn_volume_add = sid->regs[0x3e] & 15;
                    sid->gn_tone_counter = byte;
                    sid->gn_base_cycles = sid->regs[0x5d];
                    sid->gn_loop_cycles = sid->regs[0x3f];
                    sid->gn_last_count = 0;
                    sid->v4_count = 0;
                    int div = 0;//ram[sid->gn_adr + sid->gn_tone_counter] * sid->gn_loop_cycles + sid->gn_base_cycles;
                    if (div == 0)
                        sid->v4_add = 0;
                    else
                        sid->v4_add = sid_cycles * 0x10000 / div;
                    sid->v4_state = V4_GALWAY_NOISE;

                } else if (byte == 0xfd) {    // Sample off
                    sid->v4_state = V4_OFF;

                } else {                    // Sample on
                    sid->sm_adr = ((sid->regs[0x1f] << 8) | sid->regs[0x1e]) << 1;
                    sid->sm_end_adr = ((sid->regs[0x3e] << 8) | sid->regs[0x3d]) << 1;
                    sid->sm_rep_adr = ((sid->regs[0x7f] << 8) | sid->regs[0x7e]) << 1;
                    sid->sm_rep_count = sid->regs[0x3f];
                    sid->sm_big_endian = sid->regs[0x7d];
                    switch (byte) {
                        case 0xfc:
                            sid->sm_volume = 2;
                            break;
                        case 0xfe:
                            sid->sm_volume = 1;
                            break;
                        case 0xff:
                            sid->sm_volume = 0;
                            break;
                    };
                    int div = (sid->regs[0x5e] << 8) | sid->regs[0x5d];
                    if (sid->regs[0x5f])
                        div >>= sid->regs[0x5f];
                    if (div == 0) {
                        sid->v4_state = V4_OFF;
                    } else {
                        sid->v4_count = 0;
                        sid->v4_add = sid_cycles * 0x10000 / div;
                        sid->v4_state = V4_SAMPLE;
                    }
                }
            }
            break;
    }
}

void sid_write(uint32_t adr, uint32_t byte)
{
    //SDL_LockAudio();
    osid_write(sid1, adr & 0x7f, byte, 0, false);
    //SDL_UnlockAudio();
}
