// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      processor_pio.c
//      Authors :   Sascha Schneider
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
            uint8_t data;
            uint8_t flags;
        } data;
    } value;
    uint16_t count = 0;
    uint8_t data = 0;

    while (1) {
        value.value = pio_sm_get_blocking(pio1, 0);

        if (value.data.flags & 0x8) {

            data = cpuMemory[value.data.address];

        } else {

            cpuMemory[value.data.address] = value.data.data;
            if (value.data.address == CONTROLPORT && value.data.data) {
                DSPHandler(cpuMemory + controlPort, cpuMemory);
            }

        }

        pio_sm_put(pio1, 0, data);

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
