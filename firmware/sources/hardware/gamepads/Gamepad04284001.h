// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      Gamepad04284001.h
//      Authors :   Joonas Viskari
//      Date :      27th March 2024
//      Reviewed :  No
//      Purpose :   Gamepad header (Advanced Gravis Computer Tech, Ltd GamePad Pro)
//
// ***************************************************************************************
// ***************************************************************************************

#pragma once
#include "Gamepad.h"

class Gamepad04284001 : public Gamepad {
	
public:
	uint32_t getState() override;
	void update(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len) override;

private:
	bool m_dpad_up;
	bool m_dpad_down;
	bool m_dpad_left;
	bool m_dpad_right;

	bool m_start;
	bool m_select;

	bool m_b1;
	bool m_b2;
	bool m_b3;
	bool m_b4;

	bool m_r1;
	bool m_r2;

	bool m_l1;
	bool m_l2;
};
