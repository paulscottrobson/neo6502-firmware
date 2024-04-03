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

static bool isADCInitialised = false;

int UEXTSetGPIODirection(int gpio,int pinType) {
	gpio_init(gpio);
    if (pinType == UEXT_ANALOGUE) {
        if (!isADCInitialised) adc_init();
        isADCInitialised = true;
        adc_gpio_init(gpio);
    } else {
        gpio_set_dir(gpio, (pinType == UEXT_INPUT) ? GPIO_IN : GPIO_OUT);  		
    }
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
//                                     Read from ADC
//
// ***************************************************************************************

int UEXTGetGPIOAnalogue(int gpio,uint16_t *pLevel) {
    adc_select_input(gpio-26);
    *pLevel = adc_read();
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
//                               UEXT SPI Initialise helper function
//
// ***************************************************************************************

static int reg_read(spi_inst_t *spi,const uint cs,const uint8_t reg,uint8_t *buf,const uint8_t nbytes) {
    int num_bytes_read = 0;
    uint8_t mb = 0;
    if (nbytes < 1) {
        return -1;
    } else if (nbytes == 1) {
        mb = 0;
    } else {
        mb = 1;
    }
    uint8_t msg = 0x80 | (mb << 6) | reg;
    gpio_put(cs, 0);
    spi_write_blocking(spi, &msg, 1);
    num_bytes_read = spi_read_blocking(spi, 0, buf, nbytes);
    gpio_put(cs, 1);
    return num_bytes_read;
}

// ***************************************************************************************
//
//                                     UEXT SPI Initialise
//
// ***************************************************************************************

#define SPI_MISO_PIN    (24)
#define SPI_MOSI_PIN    (27)
#define SPI_SCK_PIN     (26)
#define SPI_CS_PIN      (25)

#define SPI_DEVICE      (spi1)

int UEXTSPIInitialise(void) {
    gpio_init(SPI_CS_PIN);                                                      // Set SPI pin high
    gpio_set_dir(SPI_CS_PIN, GPIO_OUT);
    gpio_put(SPI_CS_PIN, 1);

    spi_init(SPI_DEVICE, 1000 * 1000);                                          // Initialize SPI port at 1 MHz

                                                                                // Set SPI format
    spi_set_format( SPI_DEVICE,                                                 // SPI instance  
                    8,                                                          // Number of bits per transfer
                    SPI_CPOL_1,                                                  // Polarity (CPOL)
                    SPI_CPHA_1,                                                 // Phase (CPHA)
                    SPI_MSB_FIRST);

    gpio_set_function(SPI_SCK_PIN, GPIO_FUNC_SPI);                              // Initialize SPI pins
    gpio_set_function(SPI_MOSI_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SPI_MISO_PIN, GPIO_FUNC_SPI);

    uint8_t data;
    reg_read(SPI_DEVICE, SPI_CS_PIN, 0, &data, 1);                              // Workaround: perform throw-away read to make SCK idle high
    return 0;
}

// ***************************************************************************************
//
//                              Write bytes to SPI device
//
// ***************************************************************************************

int UEXTSPIWriteBlock(uint8_t *data,size_t size) {
    gpio_put(SPI_CS_PIN, 0);
    size_t nWritten = spi_write_blocking(SPI_DEVICE, data,size);
    gpio_put(SPI_CS_PIN, 1);   
    return (nWritten == size) ? 0 : 1;    
}

// ***************************************************************************************
//
//                              Read bytes from SPI device
//
// ***************************************************************************************

int UEXTSPIReadBlock(uint8_t *data,size_t size) {
    gpio_put(SPI_CS_PIN, 0);
    size_t nRead = spi_read_blocking(SPI_DEVICE, 0, data,size);
    gpio_put(SPI_CS_PIN, 1);   
    return (nRead == size) ? 0 : 1;    
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
