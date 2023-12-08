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

PIO pio = pio1;
uint pio_irq = PIO1_IRQ_1;
uint pio_sm = 0;
enum pio_interrupt_source irq_source = pis_sm0_rx_fifo_not_empty;

void __time_critical_func(irq_handler)() {
  union u32
  {
    uint32_t value;
    struct {
      uint16_t address;
      uint8_t data;
      uint8_t flags;
    } data;
  } value;

  value.value = pio->rxf[pio_sm];

  const bool write = (value.data.flags & 0x8) == 0;

  if(write)
  {
    *(cpuMemory + value.data.address) = value.data.data;
  }

  uint8_t* address = cpuMemory + value.data.address;
  uint8_t data = *address;

  pio->txf[pio_sm] = data;
}

void initPio() {
  uint offset = 0;

  offset = pio_add_program(pio, &memory_emulation_with_clock_program);
  
  pio_set_irq1_source_enabled(pio, irq_source, true);
  irq_set_exclusive_handler(pio_irq, irq_handler);
  irq_set_priority(pio_irq, 0);
  irq_set_enabled(pio_irq, true);

  memory_emulation_with_clock_program_init(pio, pio_sm, offset);
  pio_sm_set_enabled(pio, pio_sm, true);
}

// ***************************************************************************************
//
//                      Start and run the CPU. Does not have to return.
//
// ***************************************************************************************

void CPUExecute(void) {
    wdc65C02cpu_init();
    initPio();
    wdc65C02cpu_reset();

    uint16_t count = 0;
    while(1) {
      if(cpuMemory[CONTROLPORT] != 0) {
        DSPHandler(cpuMemory + controlPort, cpuMemory);
      }
      if(count++ == 0) {
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
