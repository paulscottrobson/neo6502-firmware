// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      Gamepad054C0CDA.h
//      Authors :   Sascha Schneider
//      Date :      25th March 2024
//      Reviewed :  No
//      Purpose :   Gamepad header (Playstation classic controller)
//
// ***************************************************************************************
// ***************************************************************************************

#pragma once
#include "Gamepad.h"

class Gamepad054C0CDA : public Gamepad {
	
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

	bool m_triangle;
	bool m_circle;
	bool m_cross;
	bool m_square;

	bool m_r1;
	bool m_r2;

	bool m_l1;
	bool m_l2;
};
