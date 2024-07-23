// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      Gamepad07382217.cpp
//      Authors :   Sascha Schneider & Jobond
//      Date :      25th March 2024
//      Reviewed :  No
//      Purpose :   Gamepad interface (Joystick CompetitionPro)
//
// ***************************************************************************************
// ***************************************************************************************

#include "Gamepad07382217.h"

// ***************************************************************************************
//
//				Calculate gamepad bit settings from controller state
//
// ***************************************************************************************

uint32_t Gamepad07382217::getState() {
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

void Gamepad07382217::update(uint8_t dev_addr, uint8_t instance, uint8_t const *report, uint16_t len) {
	if (len != 3) {
		return;
	}

	m_dpad_left = report[1] == 0x00;
	m_dpad_right = report[1] == 0xFF;
	m_dpad_up = report[2] == 0x00;
	m_dpad_down = report[2] == 0xFF;


	m_a = (report[0] & 0x0F) == 1;
	m_b = (report[0] & 0x0F) == 2;
	m_x = (report[0] & 0x0F) == 3;
	m_y = (report[0] & 0x0F) == 4;
	
	
}
