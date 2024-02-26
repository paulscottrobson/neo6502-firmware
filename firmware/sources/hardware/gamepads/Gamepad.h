#pragma once
#include <cstdint>

class Gamepad {
public:
	virtual ~Gamepad(){};
	virtual uint8_t getState() = 0;
	virtual void update(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len) = 0;
};