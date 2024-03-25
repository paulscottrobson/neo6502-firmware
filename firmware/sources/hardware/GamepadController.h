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
	uint8_t getState();

private:
	std::unordered_map<uint16_t, std::unique_ptr<Gamepad>> m_gamepads;
};