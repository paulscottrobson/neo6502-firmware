// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      palette.h
//      Authors :   Paul Robson (paul@robsons.org.uk)
//                 	Joseph White 
//      Date :      21st November 2023
//      Reviewed :  No
//      Purpose :   Default Palette. 
//
// ***************************************************************************************
// ***************************************************************************************

// ***************************************************************************************
//
//				The default palette. Colour set is the Pico-8 project colours
//						  First 8 match BBC Micro colours (roughly)
//
// ***************************************************************************************

const uint8_t default_palette[] = {

	0, 0, 0,       		// #000000 black / transparent for sprites.
	255, 0, 77,   		// #FF004D red
	0, 228, 54,   		// #00E436 green
	255, 236, 39,    	// #FFEC27 yellow
	29, 43, 83,   		// #1D2B53 dark-blue
	126, 37, 83,     	// #7E2553 dark-purple
	41, 173, 255,    	// #29ADFF blue
	255, 241, 232,   	// #FFF1E8 white

	0, 0, 0,       		// #000000 solid black
	95, 87, 79,   		// #5F574F dark-grey
	0, 135, 81,   		// #008751 dark-green
	255, 163, 0,     	// #FFA300 orange
	171, 82, 54,     	// #AB5236 brown
	131, 118, 156,   	// #83769C lavender
	255, 204, 170,   	// #FFCCAA light-peach
	194, 195, 199,   	// #C2C3C7 light-grey

} ;