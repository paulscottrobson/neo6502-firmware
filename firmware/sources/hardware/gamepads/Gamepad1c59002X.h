// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      Gamepad1c59002X.h
//      Authors :   Joonas Viskari
//      Date :      1st April 2024
//      Reviewed :  No
//      Purpose :   Gamepad header (1c59:0024 / 1c59:0026 Retro Games LTD)
//
// ***************************************************************************************
// ***************************************************************************************

#pragma once
#include "Gamepad.h"

class Gamepad1c590026 : public Gamepad {
	
public:
	uint32_t getState() override;
	void update(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len) override;

private:
    bool m_is_the_pad;
	bool m_up;
	bool m_down;
	bool m_left;
	bool m_right;

	bool m_b1;
	bool m_b2;
	bool m_b3;
	bool m_b4;
	bool m_b5;
	bool m_b6;	

	//THEPAD
	bool m_menu;
	bool m_select;
	bool m_sl;
	bool m_sr;
	bool m_a;
	bool m_b;
	bool m_x;
	bool m_y;
};
