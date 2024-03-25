// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      Gamepad081FE401.cpp
//      Authors :   Sascha Schneider
//      Date :      25th March 2024
//      Reviewed :  No
//      Purpose :   Gamepad interface (Olimex SNES Style Gamepad and similar)
//
// ***************************************************************************************
// ***************************************************************************************

#include "Gamepad081FE401.h"

#include "interface/console.h"

// ***************************************************************************************
//
//				Calculate gamepad bit settings from controller state
//
// ***************************************************************************************

uint32_t Gamepad081FE401::getState() {
	 uint32_t state = 0;

	if (m_a) state |= 0x10;
	if (m_b) state |= 0x20;
	if (m_x) state |= 0x40;
	if (m_y) state |= 0x80;

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

void Gamepad081FE401::update(uint8_t dev_addr, uint8_t instance, uint8_t const *report, uint16_t len) {
	if (len != 8) {
		return;
	}

	m_dpad_left = report[0] == 0x00;
	m_dpad_right = report[0] == 0xFF;
	m_dpad_up = report[1] == 0x00;
	m_dpad_down = report[1] == 0xFF;

	m_select = (report[6] & 0x10) != 0;
	m_start = (report[6] & 0x20) != 0;

	m_x = (report[5] & 0x10) != 0;
	m_y = (report[5] & 0x80) != 0;

	m_a = (report[5] & 0x20) != 0;
	m_b = (report[5] & 0x40) != 0;
	m_r = (report[6] & 0x02) != 0;
	m_l = (report[6] & 0x01) != 0;
}
