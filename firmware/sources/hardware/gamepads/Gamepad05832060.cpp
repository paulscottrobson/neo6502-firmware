// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      Gamepad05832060.cpp
//      Authors :   Joonas Viskari
//      Date :      7th April 2024
//      Reviewed :  No 
//
// ***************************************************************************************
// ***************************************************************************************

#include "Gamepad05832060.h"

// ***************************************************************************************
//
//				Calculate gamepad bit settings from controller state
//
// ***************************************************************************************

uint32_t Gamepad05832060::getState() {
	uint32_t state = 0;

    //SNES Like button scheme
	if (m_x) state |= 0x10;       //fire1
	if (m_y) state |= 0x10;       //fire1
	if (m_sl) state |= 0x10;      //fire1		
	if (m_a) state |= 0x20;       //fire2
	if (m_b) state |= 0x20;       //fire2
	if (m_sr) state |= 0x20;      //fire2			
	if (m_start) state |= 0x40;   //menu
	if (m_select) state |= 0x80;  //select	

    //not suppported 
	//if (m_turbo) state |= 0x80; // turbo		
	//if (m_clear) state |= 0x80; // clear		

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

void Gamepad05832060::update(uint8_t dev_addr, uint8_t instance, uint8_t const *report, uint16_t len) {

	if (len < 4) {
		return;
	}

	m_left = (report[0] == 0x00);
	m_right = (report[0] == 0xff);
	m_up = (report[1] == 0x00);
	m_down = (report[1] == 0xff);
	
	m_a = (report[2] & 0x01)  != 0;       // a 
	m_b = (report[2] & 0x02)  != 0;       // b
	m_x = (report[2] & 0x04)  != 0;       // x
	m_y = (report[2] & 0x08)  != 0;       // y
	m_sl = (report[2] & 0x10) != 0;	      // shoulder left
	m_sr = (report[2] & 0x20) != 0;       // shoulder right
	m_select = (report[2] & 0x40) != 0;   // select
	m_start = (report[2] & 0x80) != 0;    // start  

	m_turbo = (report[3] & 0x10) != 0;    // start  
	m_clear = (report[3] & 0x20) != 0;    // select
	
}

