#include <cstdint>

uint8_t gamepadState = 0;

uint8_t GMPGamepadController(void) {
    return gamepadState;
}