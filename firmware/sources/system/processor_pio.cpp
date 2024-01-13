// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      processor_pio.cpp
//      Authors :   Sascha Schneider
//                  Oliver Schmidt
//                  Rien Matthijsse
//      Date :      8th December 2023
//      Reviewed :  No
//      Purpose :   Drive the 65C02 processor via PIO
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

#define PICO_NEO6502
#define CHIPS_IMPL

#include "system/wdc65C02cpu.h"
#include "sm0_memory_emulation_with_clock.pio.h"

void initPio() {
    uint offset = 0;

    offset = pio_add_program(pio1, &memory_emulation_with_clock_program);
    memory_emulation_with_clock_program_init(pio1, 0, offset);
    pio_sm_set_enabled(pio1, 0, true);
}

// ***************************************************************************************
//
//                      Start and run the CPU. Does not have to return.
//
// ***************************************************************************************

void __time_critical_func(CPUExecute)(void) {
    wdc65C02cpu_init();
    initPio();
    wdc65C02cpu_reset();

    union u32
    {
        uint32_t value;
        struct {
            uint16_t address;
            uint8_t flags;
        } data;
    } value;
    uint16_t count = 0;

    while (1) {
        value.value = pio_sm_get_blocking(pio1, 0);

        if (value.data.flags & 0x8) { // 65C02 Read

            pio_sm_put(pio1, 0, cpuMemory[value.data.address]);

        } else { // 65C02 Write

            uint8_t data = pio_sm_get_blocking(pio1, 0);

            cpuMemory[value.data.address] = data;
            if (value.data.address == CONTROLPORT && data) {
                DSPHandler(cpuMemory + controlPort, cpuMemory);
            }

        }

        if (!count++) {
            DSPSync();
        }
    }
}

// ***************************************************************************************
//
//      Date        Revision
//      ====        ========
//
// ***************************************************************************************
