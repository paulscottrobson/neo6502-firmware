// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      keyboard.cpp
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      21st November 2023
//      Reviewed :  No
//      Purpose :   Converts keyboard events to a queue and key state array.
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"
#include "interface/kbdcodes.h"

#define MAX_QUEUE_SIZE (64) 													// Max size of keyboard queue.
#define MAX_FKEY_SIZE (48)   													// Max length of function key.

//
//		Bit patterns for the key states. These represent the key codes (see kbdcodes.h)
//		at 8 states per byte, so keycode 0 is byte 0 bit 0, keycode 7 is byte 0 bit 7
//		keycode 8 is byte 1 bit 0 etc.
//
static uint8_t keyboardState[KBD_MAX_KEYCODE+1];
//
//		Queue of ASCII keycode presses.
//
static uint8_t queue[MAX_QUEUE_SIZE+1];
static uint8_t queueSize = 0;

static uint8_t currentASCII = 0,currentKeyCode = 0; 							// Current key pressed.
static uint32_t nextRepeat = 9999;  											// Time of next repeat.

static uint8_t KBDMapToASCII(uint8_t keyCode,uint8_t modifiers);
static uint8_t KBDDefaultASCIIKeys(uint8_t keyCode,uint8_t isShift);
static uint8_t KBDDefaultControlKeys(uint8_t keyCode,uint8_t isShift);
static void KBDFunctionKey(uint8_t funcNum,uint8_t modifiers);

// ***************************************************************************************
//
//					Handle a key event. Note keyCode = 0xFF is reset
//
// ***************************************************************************************

void KBDEvent(uint8_t isDown,uint8_t keyCode,uint8_t modifiers) {

	if (isDown && keyCode == KEY_ESC) {   										// Pressed ESC
		cpuMemory[controlPort+3] |= 0x80;  										// Set that flag.
	}

	if (keyCode == 0xFF) { 														// Reset request
		queueSize = 0; 															// Empty keyboard queue
		for (unsigned int i = 0;i < sizeof(keyboardState);i++) {   				// No keys down.
			keyboardState[i] = 0; 
		}
		return;
	}

	if (keyCode != 0 && keyCode < KBD_MAX_KEYCODE) { 							// Legitimate keycode.
		if (isDown) {
			keyboardState[keyCode] = 0xFF; 										// Set down flag.
			uint8_t ascii = KBDMapToASCII(keyCode,modifiers);  					// What key ?
			if (ascii != 0) {
				currentASCII = ascii;  											// Remember code and time.
				currentKeyCode = keyCode;
				KBDInsertQueue(ascii);  										// Push in the queue
				nextRepeat = TMRRead()+KBD_REPEAT_START;
			}
		} else {
			keyboardState[keyCode] = 0x00; 										// Clear flag
			if (keyCode == currentKeyCode) currentASCII = 0; 					// Autorepeat off, key released.
		}
	}
}

// ***************************************************************************************
//
//								Handle Repeat
//
// ***************************************************************************************

void KBDCheckTimer(void) {
	if (currentASCII != 0) {  													// Key pressed
		if (TMRRead() >= nextRepeat) {  										// Time up ?
			KBDInsertQueue(currentASCII);  										// Put in queue
			nextRepeat = TMRRead()+KBD_REPEAT_AFTER; 							// Quicker repeat after first time.
		}
	}

//	int ascii = getchar_timeout_us(0);
//	if (ascii != PICO_ERROR_TIMEOUT) {
//		KBDInsertQueue(ascii);
//	}
}

// ***************************************************************************************
//
//								Access the keyboard state
//
// ***************************************************************************************

uint8_t *KBDGetStateArray(void) {
	return keyboardState;
}

// ***************************************************************************************
//
//						Insert ASCII character into keyboard queue
//
// ***************************************************************************************

void KBDInsertQueue(uint8_t ascii) {
	if (queueSize < MAX_QUEUE_SIZE) {   										// Do we have a full queue ?
		queue[queueSize] = ascii;  												// If not insert it.
		queueSize++;
	}
}

// ***************************************************************************************
//
//							  Key available in ASCII queue
//
// ***************************************************************************************

bool KBDIsKeyAvailable(void) {
	return queueSize != 0;
}

// ***************************************************************************************
//
//						Dequeue first key, or 0 if no key available
//
// ***************************************************************************************

uint8_t KBDGetKey(void) {
	if (queueSize == 0) return 0; 												// Queue empty.
	uint8_t key = queue[0];
	for (uint8_t i = 0;i < queueSize;i++) queue[i] = queue[i+1];  				// Dequeue it
	queueSize--;
	return key;
}

// ***************************************************************************************
//
//							Map scancode and modifier to ASCII
//
// ***************************************************************************************

static uint8_t KBDMapToASCII(uint8_t keyCode,uint8_t modifiers) {
	uint8_t ascii = 0;
	uint8_t isShift = (modifiers & KEY_SHIFT) != 0;
	uint8_t isControl = (modifiers & KEY_CONTROL) != 0;

	if (keyCode >= KEY_F1 && keyCode < KEY_F1+10) {  							// Do whatever with function keys.
		KBDFunctionKey(keyCode - KEY_F1 + 1,modifiers);
	}
	if (keyCode >= KEY_A && keyCode < KEY_A+26) { 								// Handle alphabet.
		ascii = keyCode - KEY_A + 'A';  										// Make ASCII
		if (!isShift) ascii += 'a'-'A'; 										// Handle shift
		if (isControl) ascii &= 0x1F; 											// Handle control
	}

	if (ascii == 0 && keyCode >= KEY_1 && keyCode < KEY_1+10) { 				// Handle numbers - slightly mangled.
		ascii = (keyCode == KEY_1+9) ? '0' : keyCode - KEY_1 + '1'; 			// because the order on the keyboard isn't 0-9!
		if (isShift) ascii = ")!@#$%^&*("[ascii - '0']; 						// Standard US mapping. of the top row
	}

	if (ascii == 0) {															// This maps all the other ASCII keys.
		ascii = KBDDefaultASCIIKeys(keyCode,modifiers); 						
	}

	if (ascii == 0) {															// This maps all the control keys
		ascii = KBDDefaultControlKeys(keyCode,modifiers); 						
	}

	return LOCLocaleMapping(ascii,keyCode,modifiers); 							// Special mapping for locales.
}

// ***************************************************************************************
//
//						Work out ASCII keys (including space)
//
// ***************************************************************************************

#define KEY(code,normal,shifted) code,normal,shifted

static const uint8_t defaultShift[] = {
	KEY(KEY_MINUS,'-','_'),			KEY(KEY_EQUAL,'=','+'), 		KEY(KEY_LEFTBRACE,'[','{'), 
	KEY(KEY_RIGHTBRACE,']','}'), 	KEY(KEY_BACKSLASH,'\\','|'), 	KEY(KEY_HASHTILDE,'#','~'),
	KEY(KEY_SEMICOLON,';',':'), 	KEY(KEY_APOSTROPHE,'\'','"'), 	KEY(KEY_GRAVE,'`','~'),
	KEY(KEY_COMMA,',','<'), 		KEY(KEY_DOT,'.','>'), 			KEY(KEY_SLASH,'/','?'),
	KEY(KEY_SPACE,' ',' '),			KEY(KEY_102ND,'\\','|'),
	0	
};

static uint8_t KBDDefaultASCIIKeys(uint8_t keyCode,uint8_t isShift) {
	uint8_t ascii = 0;
	uint8_t index = 0;
	while (defaultShift[index] != 0 && defaultShift[index] != keyCode) 			// Look up in table of standard mappings (US)
		index += 3;
	if (defaultShift[index] == keyCode) {  										// found a match.
		ascii = isShift ? defaultShift[index+2] : defaultShift[index+1]; 	
	}
	return ascii;
}

// ***************************************************************************************
//
//						Work out standard controls (include CHR(127))
//
// ***************************************************************************************

static const uint8_t defaultControlKeys[] = {
	KEY_LEFT,CC_LEFT,KEY_RIGHT,CC_RIGHT,KEY_INSERT,CC_INSERT,
	KEY_PAGEDOWN,CC_PAGEDOWN,KEY_END,CC_END,KEY_DELETE,CC_DELETE,
	KEY_TAB,CC_TAB,KEY_ENTER,CC_ENTER,KEY_PAGEUP,CC_PAGEUP,KEY_DOWN,CC_DOWN,
	KEY_HOME,CC_HOME,KEY_UP,CC_UP,KEY_ESC,CC_ESC, 
	KEY_BACKSPACE, CC_BACKSPACE, 0
};	

static uint8_t KBDDefaultControlKeys(uint8_t keyCode,uint8_t isShift) {
	uint8_t index = 0;
	while (defaultControlKeys[index] != 0) {
		if (defaultControlKeys[index] == keyCode) {
			return defaultControlKeys[index+1];
		}
		index += 2;
	} 
	return 0;
}	

// ***************************************************************************************
//
//								Define function keys
//
// ***************************************************************************************

static char functionKeyText[10*(MAX_FKEY_SIZE+1)] = {0};

#define FKEYTEXT(n) (functionKeyText + (n-1)*(MAX_FKEY_SIZE+1))

uint8_t KBDSetFunctionKey(int fKey,const char *keyText) {
	if (strlen(keyText) <= MAX_FKEY_SIZE && fKey >= 1 && fKey <= 10) {
		strcpy(FKEYTEXT(fKey),keyText);
		return 0;
	}
	return 1;
}

// ***************************************************************************************
//
//								Process function keys
//
// ***************************************************************************************

static void KBDFunctionKey(uint8_t funcNum,uint8_t modifiers) {
	const char *txt = FKEYTEXT(funcNum);
	while (*txt != '\0') {
		KBDInsertQueue(*txt++);		
	}
}


// ***************************************************************************************
//
//								   List function keys
//
// ***************************************************************************************

void KBDShowFunctionKeys(void) {
	char szBuffer[16];
	for (int i = 1;i <= 10;i++) {
		char *k = FKEYTEXT(i);
		if (*k != '\0') {
			sprintf(szBuffer,"F%-2d : \"",i);
			CONWriteString(szBuffer);
			while (*k != '\0') {
				if (*k == 13) {
					CONWriteString("<Enter>");
				} else {
					CONWrite(*k);
				}
				k++;
			}
			CONWrite(34);CONWrite(13);
		}
	}	
}


// ***************************************************************************************
//
//							Read the keyboard controller
//
// ***************************************************************************************

uint8_t KBDKeyboardController(void) {
	uint8_t ck = 0;
	if (keyboardState[29]) ck |= 0x01; 											// Z bit 0
	if (keyboardState[27]) ck |= 0x02; 											// X bit 1
	if (keyboardState[14]) ck |= 0x04; 											// K bit 2
	if (keyboardState[16]) ck |= 0x08; 											// M bit 3
	if (keyboardState[15]) ck |= 0x10; 											// L bit 4
	if (keyboardState[51]) ck |= 0x20; 											// ; bit 5 (key imm right of L)
	return ck;
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//		07-02-24 	Added ability to list function keys
//
// ***************************************************************************************
