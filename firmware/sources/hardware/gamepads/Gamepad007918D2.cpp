// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      Gamepad00791D2C.cpp
//      Authors :   TurBoss
//      Date :      25th March 2024
//      Reviewed :  No
//      Purpose :   Gamepad header (NS-Magic D-INPUT)
//
// ***************************************************************************************
// ***************************************************************************************

#include "Gamepad007918D2.h"
#include "interface/console.h"

// ***************************************************************************************
//
//				Calculate gamepad bit settings from controller state
//
// ***************************************************************************************

uint32_t Gamepad007918D2::getState() {
	uint32_t state = 0;

	if (m_a) state |= 0x10;
	if (m_b) state |= 0x20;
	if (m_x) state |= 0x40;
	if (m_y) state |= 0x80;

	if (m_dpad_left) state |= 0x1;
	if (m_dpad_right) state |= 0x2;

	if (m_dpad_up) state |= 0x4;
	if (m_dpad_down) state |= 0x8;

	if (m_dpad_up_right) state |= 0x6;
	if (m_dpad_up_left) state |= 0x5;
	if (m_dpad_down_right) state |= 0xA;
	if (m_dpad_down_left) state |= 0x9;

	return state;
}

// ***************************************************************************************
//
//				Process USB HID Report converting data to button presses
//
// ***************************************************************************************

void Gamepad007918D2::update(uint8_t dev_addr, uint8_t instance, uint8_t const *report, uint16_t len) {

	if (len != 27) {
		return;
	}

//	CONWriteHex(report[0]);  // buttons
//	CONWriteHex(report[1]);  // extra
//	CONWriteHex(report[2]);  // pas
//	CONWriteHex(report[3]);  // joy L X
//	CONWriteHex(report[4]);  // joy L Y
//	CONWriteHex(report[5]);  // joy R X
//	CONWriteHex(report[6]);  // joy R Y
//	CONWrite('\r');

	m_dpad_up = (report[2] & 0xF) == 0x0;
	m_dpad_up_right = (report[2] & 0xF) == 0x1;
	m_dpad_right = (report[2] & 0xF) == 0x2;
	m_dpad_down_right = (report[2] & 0xF) == 0x3;
	m_dpad_down = (report[2] & 0xF) == 0x4;
	m_dpad_down_left = (report[2] & 0xF) == 0x5;
	m_dpad_left = (report[2] & 0xF) == 0x6;
	m_dpad_up_left = (report[2] & 0xF) == 0x7;

	m_select = (report[1] & 0x1) != 0;
	m_start = (report[1] & 0x2) != 0;

	m_back = (report[2] & 0x20) != 0;
	m_home = (report[2] & 0x80) != 0;
	m_menu = (report[2] & 0x40) != 0;

	m_x = (report[0] & 0x08) != 0;
	m_y = (report[0] & 0x01) != 0;
	m_a = (report[0] & 0x04) != 0;
	m_b = (report[0] & 0x02) != 0;

	m_r1 = (report[0] & 0x20) != 0;
	m_r2 = (report[0] & 0x80) != 0;
	m_l1 = (report[0] & 0x00) != 0;
	m_l2 = (report[0] & 0x40) != 0;

	m_left_thumb = (report[1] & 0x04) != 0;
	m_right_thumb = (report[1] & 0x08) != 0;

	m_left_thumb_x = report[3];
	m_left_thumb_y = report[4];

	m_right_thumb_x = report[5];
	m_right_thumb_y = report[6];

}
