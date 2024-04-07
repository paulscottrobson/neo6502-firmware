// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      Gamepad05832060.h
//      Authors :   Joonas Viskari
//      Date :      7th April 2024
//      Reviewed :  No
//      Purpose :   Gamepad header (0583:2060 Padix Co., Ltd (Rockfire) 2-axis 8-button gamepad)
//
// ***************************************************************************************
// ***************************************************************************************

#pragma once
#include "Gamepad.h"

class Gamepad05832060 : public Gamepad {
	
public:
	uint32_t getState() override;
	void update(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len) override;

private:
    bool m_is_the_pad;
	bool m_up;
	bool m_down;
	bool m_left;
	bool m_right;

	bool m_start;
	bool m_select;
	bool m_turbo;
	bool m_clear;
	bool m_sl;
	bool m_sr;
	bool m_a;
	bool m_b;
	bool m_x;
	bool m_y;
};
