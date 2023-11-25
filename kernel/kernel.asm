; ***************************************************************************************
; ***************************************************************************************
;
;      Name :      kernel.asm
;      Authors :   Paul Robson (paul@robsons.org.uk)
;      Date :      23rd November 2023
;      Reviewed :  No
;      Purpose :   Monitor main program
;
; ***************************************************************************************
; ***************************************************************************************

	* = $fc00

; ***************************************************************************************
;
;							Access the message control port
;
; ***************************************************************************************

	ControlPort = $FF00

	DCommand = ControlPort+0
	DFunction = ControlPort+1
	DError = ControlPort+2
	DStatus = ControlPort+3
	DParameters = ControlPort+4
	DTopOfStack = ControlPort+12

	Test = 1

start
	cld 								; set up
	sei
	ldx 	#$ff
	txs
	jmp 	WozMonStart

	.include 	"support.asm"
	.include 	"wozmon.asm"

	* = ControlPort
	.word 	0,0,0,0,0,0,0,0

	.include "build/_vectors.inc"
	
	* = $FFFA
	.word 	start
	.word 	start
	.word 	start
