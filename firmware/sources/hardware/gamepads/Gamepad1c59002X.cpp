// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      Gamepad1c59002X.cpp
//      Authors :   Joonas Viskari
//      Date :      1st April 2024
//      Reviewed :  No 
//
// ***************************************************************************************
// ***************************************************************************************

#include "Gamepad1c59002X.h"

// ***************************************************************************************
//
//				Calculate gamepad bit settings from controller state
//
// ***************************************************************************************

uint32_t Gamepad1c590026::getState() {
	uint32_t state = 0;

    if (!m_is_the_pad)
	{
		if (m_b3) state |= 0x10; //fire1
		if (m_b6) state |= 0x20; //fire2
		if (m_b2) state |= 0x40; //ring top
		if (m_b4) state |= 0x80; //ring right
		if (m_b5) state |= 0x80; //ring down
		if (m_b1) state |= 0x80; //ring left
	}
	else 
	{
		if (m_x) state |= 0x10; //fire1
		if (m_y) state |= 0x10; //fire1
		if (m_sl) state |= 0x10; //fire1		
		if (m_a) state |= 0x20; //fire2
		if (m_b) state |= 0x20; //fire2
		if (m_sr) state |= 0x20; //fire2			
		if (m_menu) state |= 0x40; //menu
		if (m_select) state |= 0x80; //select		
	}

	if (m_left) state |= 0x1;
	if (m_right) state |= 0x2;

	if (m_up) state |= 0x4;
	if (m_down) state |= 0x8;

	return state;
}

// ***************************************************************************************
//
//				Process USB HID Report converting data to button presses
//
// ***************************************************************************************

void Gamepad1c590026::update(uint8_t dev_addr, uint8_t instance, uint8_t const *report, uint16_t len) {

	if (len < 6) {
		return;
	}

	m_left = (report[0] == 0x00);
	m_right = (report[0] == 0xff);
	m_up = (report[1] == 0x00);
	m_down = (report[1] == 0xff);

	if (( report[5] &0x0f ) == 0x0f)
	{
		m_is_the_pad = true;
	}
	else 
	{
		m_is_the_pad = false;
	}

    //THECXSTICK mapping
    if ( !m_is_the_pad )
	{
		m_b1 = (report[6] & 0x01) != 0; //ring left
		m_b2 = (report[6] & 0x02) != 0; //ring top
		m_b3 = (report[6] & 0x04) != 0; //fire1 
		m_b4 = (report[6] & 0x08) != 0; //ring right
		m_b5 = (report[6] & 0x10) != 0;	//ring down
		m_b6 = (report[6] & 0x20) != 0; //fire2
	} else 
	{
		m_y = (report[5] & 0x10) != 0;      // y
		m_b = (report[5] & 0x20) != 0;      // b
		m_a = (report[5] & 0x40) != 0;      // a 
		m_x = (report[5] & 0x80) != 0;      // x
		m_sl = (report[6] & 0x01) != 0;	    // shoulder left
		m_sr = (report[6] & 0x02) != 0;     // shoulder right
		m_menu = (report[6] & 0x04) != 0;   // menu  
		m_select = (report[6] & 0x08) != 0; // select
	}
}

