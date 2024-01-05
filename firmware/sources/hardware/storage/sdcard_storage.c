// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      sdcard_storage.c
//      Author :    Carl John Kugler III
//                  Paul Robson
//      Date :      20th December 2023
//      Reviewed :  No
//      Purpose :   SDCard configuration
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"
#include <string.h>
#include "my_debug.h"
#include "hw_config.h"
#include "ff.h" 
#include "diskio.h" 
#include "f_util.h"

// SDCard requires fatfs_spi instead of fatfs in libraries.
// and sdcard_storage enabled rather than usb_storage

static bool isInitialised = false;

void STOInitialise(void) {
    if (!isInitialised) {
        isInitialised = true;
        sd_card_t *pSD = sd_get_by_num(0);
        f_mount(&pSD->fatfs, pSD->pcName, 1);
        CONWriteString("Initialised.\n");
    }
}

void STOSynchronise(void) {    
    CONWriteString("SDCard Storage\r");
}

// Hardware Configuration of SPI "objects"
// Note: multiple SD cards can be driven by one SPI if they use different slave
// selects.

static spi_t spis[] = {  // One for each SPI.
    {
        .hw_inst = spi1,  // SPI component
        .miso_gpio = 24, // GPIO number (not pin number)
        .mosi_gpio = 27,
        .sck_gpio = 26,
        .baud_rate = 2500 * 1000,   // was 12500
        //.baud_rate = 25 * 1000 * 1000, // Actual frequency: 20833333. 
        }
};

// Hardware Configuration of the SD Card "objects"

static sd_card_t sd_cards[] = {  // One for each SD card
    {
        .pcName = "0:",   // Name used to mount device
        .spi = &spis[0],  // Pointer to the SPI driving this card
        .ss_gpio = 25,    // The SPI slave select GPIO for this SD card
        .use_card_detect = false,
        .card_detect_gpio = -1,   // Card detect
        .card_detected_true = -1,  // What the GPIO read returns when a card is
                                 // present. Use -1 if there is no card detect.
 
        .m_Status = STA_NOINIT
    }
};

size_t sd_get_num() { return count_of(sd_cards); }

sd_card_t *sd_get_by_num(size_t num) {
    if (num <= sd_get_num()) {
        return &sd_cards[num];
    } else {
        return NULL;
    }
}

size_t spi_get_num() { return count_of(spis); }
spi_t *spi_get_by_num(size_t num) {
    if (num <= sd_get_num()) {
        return &spis[num];
    } else {
        return NULL;
    }
}

// ***************************************************************************************
//
//      Date        Revision
//      ====        ========
//
// ***************************************************************************************

