// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      processor_pio.c
//      Authors :   Sascha Schneider
//                  Rien Matthijsse
//      Date :      26th November 2023
//      Reviewed :  No
//      Purpose :   Drive the 65C02 processor via PIO
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

#define PICO_NEO6502
#define CHIPS_IMPL

#include "system/wdc65C02cpu.h"

uint16_t iCount;

// ***************************************************************************************
#include "sm0_memory_emulation_with_clock.pio.h"

void irq_handler() {
  union u32
  {
    uint32_t value;
    struct {
      uint16_t address;
      uint8_t data;
      uint8_t flags;
    } data;
  } value;

  value.value = pio1->rxf[0];

  bool write = value.data.flags == 0x3;
  if(write)
  {
    *(cpuMemory + value.data.address) = value.data.data;
  }

  uint8_t* address = cpuMemory + value.data.address;
  uint8_t data = *address;
  pio1->txf[0] = data;
  iCount++; //tick main loop
  if ((uint32_t)address == CONTROLPORT && data != 0) {       // Message passed
    DSPHandler(cpuMemory+controlPort,cpuMemory);   // Go do it. Synchronous, could be Async
  }
}

void init6502() {

  // RESET
  //pinMode(uP_RESET, OUTPUT);

  // RW
  //pinMode(uP_RW, INPUT_PULLUP);

  // PIO
  PIO pio = pio1;
  uint offset = 0;

  offset = pio_add_program(pio, &memory_emulation_with_clock_program);
  //printf("Loaded program at %d\n", offset);
  
  pio_set_irq0_source_enabled(pio1,  pis_sm0_rx_fifo_not_empty, true);
  irq_set_exclusive_handler(PIO1_IRQ_0, irq_handler);
  irq_set_priority(PIO1_IRQ_0, 0);
  irq_set_enabled(PIO1_IRQ_0, true);

  memory_emulation_with_clock_program_init(pio, 0, offset);
  pio_sm_set_enabled(pio, 0, true);

  //setReset(false);
  //sleep_ms(2000);
  //setReset(true);
}

// ***************************************************************************************


// ***************************************************************************************
//
//                      Start and run the CPU. Does not have to return.
//
// ***************************************************************************************

void CPUExecute(void) {
    wdc65C02cpu_init();                                                         // Set up the 65C02
    //wdc65C02cpu_reset();
    init6502();
    while(1) {
        if (!iCount) DSPSync();                                               // 1 time in 64k. About 25-30Hz.
    }
}

// ***************************************************************************************
//
//      Date        Revision
//      ====        ========
//
// ***************************************************************************************
