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

	jsr 	KSendMessage
	.byte 	3,1
	jsr 	KWaitMessage

set16 	.macro
	lda 	#\2 & $FF
	sta 	\1
	lda 	#\2 >> 8
	sta 	1+\1
	.endm

	.set16 	$FF04,name
	.set16 	$FF06,$800
	.set16 	$FF08,$8000

	jsr 	KSendMessage
	.byte 	3,3
	jsr 	KWaitMessage

	jsr 	KSendMessage
	.byte 	3,1
	jsr 	KWaitMessage


name:	
	.byte 	8
	.text 	"demo.015"

	.include 	"support.asm"
	.include 	"wozmon.asm"

	* = ControlPort
	.word 	0,0,0,0,0,0,0,0

	.include "build/_vectors.inc"
	
	* = $FFFA
	.word 	start
	.word 	start
	.word 	start
