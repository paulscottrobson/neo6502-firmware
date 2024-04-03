// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      Gamepad07382217.h
//      Authors :   Sascha Schneider
//      Date :      25th March 2024
//      Reviewed :  No
//      Purpose :   Gamepad header (Joystick CompetitionPro)
//
// ***************************************************************************************
// ***************************************************************************************

#pragma once
#include "Gamepad.h"

class Gamepad07382217 : public Gamepad {
public:
	uint32_t getState() override;
	void update(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len) override;

private:
	bool m_dpad_up;
	bool m_dpad_down;
	bool m_dpad_left;
	bool m_dpad_right;

	bool m_a;
	bool m_b;
	bool m_x;
	bool m_y;
};
