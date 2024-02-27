// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      ports.cpp
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      21st February 2024
//      Reviewed :  No
//      Purpose :   UEXT Handling code.
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

// ***************************************************************************************
//
//								Set GPIO Direction
//
// ***************************************************************************************

int UEXTSetGPIODirection(int gpio,int pinType) {
	gpio_init(gpio);
    gpio_set_dir(gpio, (pinType == UEXT_INPUT) ? GPIO_IN : GPIO_OUT);  		
    return 0;
}

// ***************************************************************************************
//
//										Write to GPIO
//
// ***************************************************************************************

int UEXTSetGPIO(int gpio,bool isOn) {
    gpio_put(gpio,isOn ? 1 : 0);  		
    return 0;
}

// ***************************************************************************************
//
//									   Read from GPIO
//
// ***************************************************************************************

int UEXTGetGPIO(int gpio,bool *pIsHigh) {
    *pIsHigh = gpio_get(gpio);  												
	return 0;	
}

// ***************************************************************************************
//
//                                     UEXT I2C Initialise
//
// ***************************************************************************************

#define I2C_SCL_PIN    (23)
#define I2C_SDA_PIN    (22)

#define I2C_DEVICE      (i2c1)

int UEXTI2CInitialise(void) {
    i2c_init(I2C_DEVICE, 400 * 1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);
    bi_decl(bi_2pins_with_func(I2C_SDA_PIN,I2C_SCL_PIN, GPIO_FUNC_I2C));
    return 0;
}

// ***************************************************************************************
//
//                              Write bytes to I2C device
//
// ***************************************************************************************

int UEXTI2CWriteBlock(uint8_t device, uint8_t *data,size_t size) {
    size_t nWritten = i2c_write_blocking(I2C_DEVICE, device,data, size, false);
    return (nWritten == size) ? 0 : 1;    
}

// ***************************************************************************************
//
//                              Read bytes from I2C device
//
// ***************************************************************************************

int UEXTI2CReadBlock(uint8_t device, uint8_t *data,size_t size) {
    size_t nRead = i2c_read_blocking(I2C_DEVICE, device,data, size, false);
    return (nRead == size) ? 0 : 1;    
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
