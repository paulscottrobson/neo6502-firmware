/*
 *  prefs.cpp - Preferences items
 *
 *  SIDPlayer (C) Copyright 1996-2004 Christian Bauer
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

#include "sys.h"
#include "prefs.h"


// List of preferences items
prefs_desc common_prefs_items[] = {
    {"victype", TYPE_STRING, false,     "type number of VIC-II to emulate (6569, 6567 or 6567R5)"},
    {"sidtype", TYPE_STRING, false,     "type number of SID to emulate (6581 or 8580)"},
    {"samplerate", TYPE_INT32, false,   "output sample rate in Hz"},
    {"audio16bit", TYPE_BOOLEAN, false, "16-bit audio output"},
    {"stereo", TYPE_BOOLEAN, false,     "stereo audio output"},
    {"filters", TYPE_BOOLEAN, false,    "emulate SID filters"},
    {"dualsid", TYPE_BOOLEAN, false,    "emulate dual SID chips"},
    {"audioeffect", TYPE_INT32, false,  "audio effect type (0 = none, 1 = reverb, 2 = spatial)"},
    {"revdelay", TYPE_INT32, false,     "effect delay in ms"},
    {"revfeedback", TYPE_INT32, false,  "effect feedback (0..256 = 0..100%)"},
    {"volume", TYPE_INT32, false,       "master volume (0..256 = 0..100%)"},
    {"v1volume", TYPE_INT32, false,     "volume voice 1 (0..256 = 0..100%)"},
    {"v2volume", TYPE_INT32, false,     "volume voice 2 (0..256 = 0..100%)"},
    {"v3volume", TYPE_INT32, false,     "volume voice 3 (0..256 = 0..100%)"},
    {"v4volume", TYPE_INT32, false,     "volume sampled voice (0..256 = 0..100%)"},
    {"v1pan", TYPE_INT32, false,        "panning voice 1 (-256..256 = left..right)"},
    {"v2pan", TYPE_INT32, false,        "panning voice 2 (-256..256 = left..right)"},
    {"v3pan", TYPE_INT32, false,        "panning voice 3 (-256..256 = left..right)"},
    {"v4pan", TYPE_INT32, false,        "panning sampled voice (-256..256 = left..right)"},
    {"dualsep", TYPE_INT32, false,      "dual SID stereo separation (0..256 = 0..100%)"},
    {"speed", TYPE_INT32, false,        "replay speed adjustment (percent)"},
    {NULL, TYPE_END, false}    // End of list
};


/*
 *  Set default values for preferences items
 */

void AddPrefsDefaults()
{
    PrefsAddString("victype", "6569");
    PrefsAddString("sidtype", "6581");
    PrefsAddInt32("samplerate", 44100);
    PrefsAddBool("audio16bit", true);
    PrefsAddBool("stereo", true);
    PrefsAddBool("filters", true);
    PrefsAddBool("dualsid", false);
    PrefsAddInt32("audioeffect", 2);
    PrefsAddInt32("revdelay", 125);
    PrefsAddInt32("revfeedback", 0x50);
    PrefsAddInt32("volume", 0x100);
    PrefsAddInt32("v1volume", 0x100);
    PrefsAddInt32("v1pan", -0x40);
    PrefsAddInt32("v2volume", 0x100);
    PrefsAddInt32("v2pan", 0);
    PrefsAddInt32("v3volume", 0x100);
    PrefsAddInt32("v3pan", 0x40);
    PrefsAddInt32("v4volume", 0x100);
    PrefsAddInt32("v4pan", 0);
    PrefsAddInt32("dualsep", 0x80);
    PrefsAddInt32("speed", 100);
}
