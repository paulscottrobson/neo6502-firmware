// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      serial.cpp
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      24th January 2024
//      Reviewed :  No
//      Purpose :   Serial interface
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"
//#include "hardware/uart.h"

#define UART_ID uart0

#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY UART_PARITY_NONE

#define UART_TX_PIN 28
#define UART_RX_PIN 29

// ***************************************************************************************
//
//								Initialise the serial port
//
// ***************************************************************************************

bool SERInitialise(void) {
	uart_init(UART_ID, 9600);
	gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
	gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
	uart_set_hw_flow(UART_ID, false, false);
	uart_set_fifo_enabled(UART_ID, true);
	return true;
}

// ***************************************************************************************
//
//					Set the Serial Protocol. Only 8N1 currently supported.
//
// ***************************************************************************************

void SERSetSerialFormat(uint32_t baudRate,uint32_t protocol) {
	uart_set_baudrate(UART_ID,baudRate);
	uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);
}

// ***************************************************************************************
//
//						Check if byte is available from UART 
//
// ***************************************************************************************

bool SERIsByteAvailable(void) {
	return uart_is_readable_within_us(UART_ID,0);
}

// ***************************************************************************************
//
//								Read Byte from UART
//
// ***************************************************************************************

uint8_t SERReadByte(void) {
	return uart_getc(UART_ID);
}

// ***************************************************************************************
//
//								Write Byte to UART
//
// ***************************************************************************************

void SERWriteByte(uint8_t b) {
	uart_putc_raw(UART_ID,b);
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//		29-01-24 	Added timeout to readbyte.
//
// ***************************************************************************************
