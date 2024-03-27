// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      GamepadController.cpp
//      Authors :   Sascha Schneider
//      Date :      25th March 2024
//      Reviewed :  No
//      Purpose :   Gamepad controller class
//
// ***************************************************************************************
// ***************************************************************************************

#include "GamepadController.h"

#include "gamepads/Gamepad054C0CDA.h"
#include "gamepads/Gamepad081FE401.h"
#include "gamepads/Gamepad0079181C.h"
#include "gamepads/Gamepad07382217.h"
#include "gamepads/Gamepad007918D2.h"
#include "gamepads/Gamepad04284001.h"

#include "interface/console.h"

// ***************************************************************************************
//
//				Map device information to key for controller dictionary
//
// ***************************************************************************************

inline static uint16_t key(uint8_t dev_addr, uint8_t instance) {
	 return dev_addr + ((uint16_t)instance << 8);
}

// ***************************************************************************************
//
//		  Add (possibly) a new controller device. All unknown USBs register here
//
// ***************************************************************************************

bool GamepadController::add(uint16_t vid, uint16_t pid, uint8_t dev_addr, uint8_t instance, uint8_t const *desc_report, uint16_t desc_len) {
	auto old_gamepad_count = m_gamepads.size();

	switch (vid)
	{
	case 0x054C:
		switch (pid) {
		case 0x0CDA: m_gamepads.insert({key(dev_addr, instance), std::make_unique<Gamepad054C0CDA>()}); break;
		}
		break;
	case 0x081F:
		switch (pid) {
		case 0xE401: m_gamepads.insert({key(dev_addr, instance), std::make_unique<Gamepad081FE401>()}); break;
		}
		break;
	case 0x0079:
		switch (pid) {
			case 0x181C:
				m_gamepads.insert({key(dev_addr, instance), std::make_unique<Gamepad0079181C>()}); break;
			case 0x18D2:
				m_gamepads.insert({key(dev_addr, instance), std::make_unique<Gamepad007918D2>()}); break;
			}
		break;
	case 0x0738:
		switch (pid) {
		case 0x2217: m_gamepads.insert({key(dev_addr, instance), std::make_unique<Gamepad07382217>()}); break;
		}
		break;

	case 0x0428:
		switch (pid) {
		case 0x4001: m_gamepads.insert({key(dev_addr, instance), std::make_unique<Gamepad04284001>()}); break;
		}
		break;

	}

	auto driver_found = old_gamepad_count < m_gamepads.size();
	CONWriteString(driver_found ? "Gamepad driver found":"No driver found for");
	CONWriteHex(vid);CONWriteHex(pid);CONWrite('\r');
	return true;
}

// ***************************************************************************************
//
//						Controller (or other USB device) removed
//
// ***************************************************************************************

bool GamepadController::remove(uint8_t dev_addr, uint8_t instance) {
	m_gamepads.erase(key(dev_addr, instance));
	return true;
}

// ***************************************************************************************
//
//							Update the status of device
//
// ***************************************************************************************

bool GamepadController::update(uint8_t dev_addr, uint8_t instance, uint8_t const *report, uint16_t len) {
	if (auto search = m_gamepads.find(key(dev_addr, instance)); search != m_gamepads.end()) {
		search->second->update(dev_addr, instance, report, len);
		return true;
	}
	return false;
}

// ***************************************************************************************
//
//								Return the number of controllers
//
// ***************************************************************************************

uint8_t GamepadController::getCount() {
	return m_gamepads.size();
}

// ***************************************************************************************
//
//		Return the status of digital controller buttons, or analogue sticks mapped to
//		digital directions.
//
// ***************************************************************************************

uint32_t GamepadController::readDigital(uint8_t index) {
	uint32_t state = 0;
	for (const auto& [key, gamepad] : m_gamepads) {
		if (index == 0) return gamepad->getState();
		index--;
	}
	return 0;
}
