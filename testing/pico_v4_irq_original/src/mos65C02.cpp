// 
// Author: Rien Matthijsse
// 
#include "hardware/dma.h"
#include "RPi_Pico_TimerInterrupt.h"
#include "mos65C02.h"
#include "memory_sm1_address.pio.h"

void writeChar(uint8_t vChar);

#define DELAY_FACTOR_SHORT() asm volatile("nop\nnop\nnop\nnop\n");

// # of clock cycles to keep rest pin low
#define RESET_COUNT  4

// mask used for the mux address/data bus: GP0-7
constexpr uint32_t BUS_MASK = 0xFF;

volatile uint32_t  clockCount = 0UL;
uint8_t   resetCount;
boolean   inReset = false;
uint8_t   dataDir = 2;

bool addressValid = false;
bool clockActive = false;

/// <summary>
/// 
/// </summary>
/// <param name="vDebug"></param>
inline __attribute__((always_inline))
void setDebug(boolean vDebug) {
  gpio_put(pDebug, vDebug);
}

/// <summary>
/// drive the clockpin
/// </summary>
/// <param name="enable"></param>
inline __attribute__((always_inline))
void setClock(boolean clock) {
  gpio_put(uP_CLOCK, clock);
}

/// <summary>
/// drive the reset pin
/// </summary>
/// <param name="enable"></param>
inline __attribute__((always_inline))
void setReset(boolean reset) {
  gpio_put(uP_RESET, reset);
}

/// <summary>
/// drive the enable pins
/// </summary>
/// <param name="enable"></param>
inline __attribute__((always_inline))
void setEnable(uint32_t enable) {
  gpio_put_masked(en_MASK, enable);
}

/// <summary>
/// set direction of databus mux
/// </summary>
/// <param name="direction"></param>
inline __attribute__((always_inline))
void setDirInput() {
  if (dataDir != INPUT) {
    gpio_set_dir_masked(BUS_MASK, (uint32_t)0UL);
    dataDir = INPUT;
  }
}

  /// <summary>
/// set direction of databus mux
/// </summary>
/// <param name="direction"></param>
inline __attribute__((always_inline))
void setDirOutput() {
  if (dataDir != OUTPUT) {
    gpio_set_dir_masked(BUS_MASK, BUS_MASK);
    dataDir = OUTPUT;
  }
}

/// <summary>
/// read the R/W pin
/// </summary>
/// <returns></returns>
inline __attribute__((always_inline))
bool getRW() {
  return gpio_get(uP_RW);
}

/// <summary>
/// read the databus
/// </summary>
/// <returns></returns>
inline __attribute__((always_inline))
void getData() {
  // we are already in INPUT
  //setDir(INPUT);

  setEnable(en_D0_7);
  DELAY_FACTOR_SHORT();
  data = gpio_get_all() & BUS_MASK;
}

/// <summary>
/// write the databus
/// </summary>
/// <param name="data"></param>
inline __attribute__((always_inline))
void putData() {
  setDirOutput();
  setEnable(en_D0_7);
  gpio_put_masked(BUS_MASK, (uint32_t)data);
}

void start_address_program(PIO pio, uint sm, uint offset) {
    address_program_init(pio, sm, offset);
    pio_sm_set_enabled(pio, sm, true);
}

int dma_chan;

void dma_handler() {
  //Serial.printf(__FUNCTION__);
  // Clear the interrupt request.
  //dma_hw->ints1 = 1u << dma_chan;
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
  //uint16_t address  = (uint16_t) (( value >> 16) & 0xFFFFUL);
  bool write = value.data.flags == 0x3;
  if(write)
  {
    *(mem + value.data.address) = value.data.data;
    //if (value.data.address == 0xD012) writeChar(value.data.data);
  }
  uint8_t* address = mem + value.data.address;
  uint8_t data = *address;
  pio1->txf[0] = data;
  //dma_channel_set_read_addr(dma_chan, address, true);
  //Serial.printf("Value: %08X Address: %04X Data: %02X Type: %s Send Data: %02X\n", value.value, value.data.address, value.data.data, write ? "W" : "R" , data);
}

/// <summary>
/// initialise the 65C02
/// </summary>
void init6502() {

  // RESET
  pinMode(uP_RESET, OUTPUT);

  // RW
  pinMode(uP_RW, INPUT_PULLUP);

  // PIO
  PIO pio = pio1;
  uint offset = 0;

  offset = pio_add_program(pio, &address_program);
  Serial.printf("Loaded program at %d\n", offset);
  
  pio_set_irq0_source_enabled(pio1,  pis_sm0_rx_fifo_not_empty, true);
  irq_set_exclusive_handler(PIO1_IRQ_0, dma_handler);
  irq_set_priority(PIO1_IRQ_0, 0);
  irq_set_enabled(PIO1_IRQ_0, true);

  start_address_program(pio, 0, offset);

  setReset(false);
  sleep_ms(200);
  setReset(true);
}

/// <summary>
/// reset the 65C02
/// </summary>
// void reset6502() {
//   setReset(RESET_LOW);
//   resetCount = RESET_COUNT;
//   inReset = true;
// }
