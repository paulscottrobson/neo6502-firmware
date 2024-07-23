// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      Gamepad04284001.cpp
//      Authors :   Joonas Viskari
//      Date :      27th March 2024
//      Reviewed :  No
//      Purpose :   Gamepad header (Advanced Gravis Computer Tech, Ltd GamePad Pro)
//
// ***************************************************************************************
// ***************************************************************************************

#include "Gamepad04284001.h"

// ***************************************************************************************
//
//				Calculate gamepad bit settings from controller state
//
// ***************************************************************************************

uint32_t Gamepad04284001::getState() {
	uint32_t state = 0;

	if (m_b1) state |= 0x10;
	if (m_b2) state |= 0x20;
	if (m_b3) state |= 0x40;
	if (m_b4) state |= 0x80;

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

void Gamepad04284001::update(uint8_t dev_addr, uint8_t instance, uint8_t const *report, uint16_t len) {

	if (len != 4) {
		return;
	}

	m_dpad_left = (report[0] == 0x00);
	m_dpad_right = (report[0] == 0xff);
	m_dpad_up = (report[1] == 0x00);
	m_dpad_down = (report[1] == 0xff);

	m_b1 = (report[2] & 0x1) != 0;
	m_b2 = (report[2] & 0x2) != 0;
	m_b3 = (report[2] & 0x4) != 0;
	m_b4 = (report[2] & 0x8) != 0;

	m_l2 = (report[2] & 0x10) != 0;
	m_r2 = (report[2] & 0x20) != 0;
	m_l1 = (report[2] & 0x40) != 0;
	m_r1 = (report[2] & 0x80) != 0;

	m_select = (report[3] & 0x1) != 0;
	m_start = (report[3] & 0x2) != 0;

}
