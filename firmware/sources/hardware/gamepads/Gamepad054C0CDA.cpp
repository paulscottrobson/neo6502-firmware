// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      Gamepad054C0CDA.cpp
//      Authors :   Sascha Schneider
//      Date :      25th March 2024
//      Reviewed :  No
//      Purpose :   Gamepad interface for 054C:0CDA (Playstation classic controller)
//
// ***************************************************************************************
// ***************************************************************************************

#include "Gamepad054C0CDA.h"

// ***************************************************************************************
//
//				Calculate gamepad bit settings from controller state
//
// ***************************************************************************************

uint32_t Gamepad054C0CDA::getState() {
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

void Gamepad054C0CDA::update(uint8_t dev_addr, uint8_t instance, uint8_t const *report, uint16_t len) {
	if (len != 2) {
		return;
	}

	m_dpad_left = (report[1] & 0xC) == 0;
	m_dpad_right = (report[1] & 0xC) == 8;
	m_dpad_up = (report[1] & 0x30) == 0;
	m_dpad_down = (report[1] & 0x30) == 32;

	m_select = (report[1] & 0x1) != 0;
	m_start = (report[1] & 0x2) != 0;

	m_triangle = (report[0] & 0x1) != 0;
	m_circle = (report[0] & 0x2) != 0;
	m_cross = (report[0] & 0x4) != 0;
	m_square = (report[0] &  0x8) != 0;
	m_l2 = (report[0] & 0x10) != 0;
	m_r2 = (report[0] & 0x20) != 0;
	m_l1 = (report[0] & 0x40) != 0;
	m_r1 = (report[0] & 0x80) != 0;
}
