// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      GamepadController.h
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      25th March 2024
//      Reviewed :  No
//      Purpose :   Gamepad Controller class.
//
// ***************************************************************************************
// ***************************************************************************************

#pragma once

#include "gamepads/Gamepad.h"

#include <cstdint>
#include <unordered_map>
#include <memory>

class GamepadController {
public:
	bool add(uint16_t vid, uint16_t pid, uint8_t dev_addr, uint8_t instance, uint8_t const *desc_report, uint16_t desc_len);
	bool remove(uint8_t dev_addr, uint8_t instance);
	bool update(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len);
	uint8_t getCount();
	uint32_t readDigital(uint8_t index);

private:
	std::unordered_map<uint16_t, std::unique_ptr<Gamepad>> m_gamepads;
};