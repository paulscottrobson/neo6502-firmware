// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      Gamepad00798181.h
//      Authors :   Sascha Schneider
//      Date :      25th March 2024
//      Reviewed :  No
//      Purpose :   Gamepad header (Olimex wireless playstation gamepad)
//
// ***************************************************************************************
// ***************************************************************************************

#pragma once
#include "Gamepad.h"

class Gamepad0079181C : public Gamepad {
public:
	uint32_t getState() override;
	void update(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len) override;

private:
	bool m_dpad_up;
	bool m_dpad_up_right;
	bool m_dpad_right;
	bool m_dpad_down_right;
	bool m_dpad_down;
	bool m_dpad_down_left;
	bool m_dpad_left;
	bool m_dpad_up_left;

	bool m_start;
	bool m_select;
	bool m_home;

	bool m_back;
	bool m_menu;

	bool m_x;
	bool m_y;
	bool m_a;
	bool m_b;

	bool m_r1;
	bool m_r2;
	bool m_l1;
	bool m_l2;

	uint8_t m_left_thumb_x;
	uint8_t m_left_thumb_y;
	bool m_left_thumb;

	uint8_t m_right_thumb_x;
	uint8_t m_right_thumb_y;
	bool m_right_thumb;
};
