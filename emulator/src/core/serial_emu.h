#ifndef SERIAL_EMU_H
#define SERIAL_EMU_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    bool (*isByteAvailable)(void);
    uint8_t (*readByte)(void);
    void (*writeByte)(uint8_t b);
    void (*setSerialFormat)(uint32_t baudRate, uint32_t protocol);
    void (*cleanup)(void);
} SerialInterface;

enum SerialType {
    SER_STUB,
    SER_TCP
};

// Function to open and return the appropriate SerialInterface
const SerialInterface* SerialInterfaceOpen(enum SerialType type);

#ifdef __cplusplus
}
#endif

#endif // SERIAL_EMU_H
