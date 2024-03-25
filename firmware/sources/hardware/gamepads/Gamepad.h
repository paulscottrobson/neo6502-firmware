// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      Gamepad.h
//      Authors :   Sascha Schneider
//      Date :      25th March 2024
//      Reviewed :  No
//      Purpose :   Gamepad class
//
// ***************************************************************************************
// ***************************************************************************************

#pragma once
#include <cstdint>

class Gamepad {
public:
	virtual ~Gamepad(){};
	virtual uint32_t getState() = 0;
	virtual void update(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len) = 0;
};