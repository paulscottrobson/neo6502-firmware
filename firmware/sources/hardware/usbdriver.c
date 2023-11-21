// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      usbdriver.c
//      Authors :   Tsvetan Usunov (Olimex)
//                  Paul Robson (paul@robsons.org.uk)
//      Date :      20th November 2023
//      Reviewed :  No
//      Purpose :   USB interface and HID->Event mapper.
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"
#include "bsp/board.h"
#include "tusb.h"
#include "interface/kbdcodes.h"

// ***************************************************************************************
//
//                          Process USB HID Keyboard Report
//
//                  This converts it to a series of up/down key events
//
// ***************************************************************************************

static short lastReport[KBD_MAX_KEYCODE] = { 0 };                               // state at last HID report.

static void usbProcessReport(uint8_t const *report) {
	for (int i = 0;i < KBD_MAX_KEYCODE;i++) lastReport[i] = -lastReport[i];     // So if -ve was present last time.
	for (int i = 2;i < 8;i++) {                                                 // Scan the key press array.        
		uint8_t key = report[i];
		if (key >= KEY_KP1 && key < KEY_KP1+10) {                               // Numeric keypad numbers will work.
			key = key - KEY_KP1 + KEY_1;
		}
		if (key == KEY_102ND) key = KEY_BACKSLASH; 								// Non US /| mapped.

		if (key != 0 && key < KBD_MAX_KEYCODE) {                                // If key is down, and not too high.
			if (lastReport[key] == 0) KBDEvent(1,key,report[0]);                // It wasn't down before so key press.
			lastReport[key] = 1;                                                // Flag it as now being down.
		}
	} 
	for (int i = 0;i < KBD_MAX_KEYCODE;i++) {                                   // Any remaining -ve keys are up actions.
		if (lastReport[i] < 0) {
			KBDEvent(0,i,0);                                                    // Flag going up.
			lastReport[i] = 0;                                                  // Mark as now up
		}
	}
}

// ***************************************************************************************
//
//                              USB Callback functions
//
// ***************************************************************************************

void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* desc_report, uint16_t desc_len) {
 
	switch(tuh_hid_interface_protocol(dev_addr, instance)) {

		case HID_ITF_PROTOCOL_KEYBOARD:
			tuh_hid_receive_report(dev_addr, instance);
		break;

		case HID_ITF_PROTOCOL_MOUSE:
			tuh_hid_receive_report(dev_addr, instance);
		break;
	}
}

void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance) {
}

void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len) {

  switch(tuh_hid_interface_protocol(dev_addr, instance)) {
	case HID_ITF_PROTOCOL_KEYBOARD:  
	  tuh_hid_receive_report(dev_addr, instance);
	  usbProcessReport(report);
	  tuh_hid_receive_report(dev_addr, instance);     
	break;
	
	case HID_ITF_PROTOCOL_MOUSE:
	  tuh_hid_receive_report(dev_addr, instance);
	  tuh_hid_receive_report(dev_addr, instance);
	break;
  }
}

// ***************************************************************************************
//
//                               Keyboard initialisation
//
// ***************************************************************************************

void KBDInitialise(void) {
	board_init();                                                               // USB Initialise.
	tusb_init();
	for (int i = 0;i < KBD_MAX_KEYCODE;i++) lastReport[i] = 0;                  // No keys currently known
	KBDEvent(0,0xFF,0);                                                         // Reset the keyboard manager
}

// ***************************************************************************************
//
//                                  Keyboard polling
//
// ***************************************************************************************

void KBDSync(void) {
	tuh_task();
}
