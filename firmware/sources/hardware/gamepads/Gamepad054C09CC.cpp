// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      Gamepad054C09CC.cpp
//      Authors :   Pat Flynn (based on 054C0CDA by Sascha Schneider)
//      Date :      27 Aug 2025
//      Reviewed :  No
//      Purpose :   Gamepad interface for 054C:09CC (Playstation DualShock 4 controller)
//
// ***************************************************************************************
// ***************************************************************************************

#include "Gamepad054C09CC.h"
#include "interface/console.h"
#include <string.h>


// ***************************************************************************************
//
//				Calculate gamepad bit settings from controller state
//
// ***************************************************************************************

uint32_t Gamepad054C09CC::getState() {
	uint32_t state = 0;

	if (m_circle) state |= 0x10;
	if (m_cross) state |= 0x20;
	if (m_triangle) state |= 0x40;
	if (m_square) state |= 0x80;
	if (m_dpad_left) state |= 0x1;
	if (m_dpad_right) state |= 0x2;
	if (m_dpad_up) state |= 0x4;
	if (m_dpad_down) state |= 0x8;

	return state;
}

// ***************************************************************************************
//
//				Process USB HID Report converting data to button presses
//
// ***************************************************************************************

// (all byte indices are zero-based)
// source of info on controller interrupt events: https://www.psdevwiki.com/ps4/DS4-USB
//
// This code handles only the basic 8 buttons: Y X B A down up left right
// We'll adopt the convention used by Sascha for the PS Classic controller:
// Y is square, X is triangle, B is cross, and A is circle.

uint8_t old_report5;

void Gamepad054C09CC::update(uint8_t dev_addr, uint8_t instance, uint8_t const *report, uint16_t len) {
    int i;

    if (len != 64) { // sanity check
        return;
        };

    if (report[5] == old_report5) return;

    old_report5 = report[5];
    m_square =   report[5] & 0x10;
    m_triangle = report[5] & 0x80;
    m_cross =    report[5] & 0x20;
    m_circle =   report[5] & 0x40;

    uint8_t dpad = report[5] & 0x0f;
    if (dpad == 8) {
      m_dpad_left = m_dpad_right = m_dpad_up = m_dpad_down = 0;
      }
    else {
      m_dpad_left = (dpad >= 5) && (dpad <= 7);
      m_dpad_right = (dpad >= 1) && (dpad <= 3);
      m_dpad_up = (dpad == 0) || (dpad == 1) || (dpad == 7);
      m_dpad_down = (dpad >= 3) && (dpad <= 5);
      }
}
