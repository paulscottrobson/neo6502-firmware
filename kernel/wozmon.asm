; ***************************************************************************************
; ***************************************************************************************
;
;      Name :      wozmon.asm
;      Authors :   Paul Robson (paul@robsons.org.uk)
;      Date :      23rd November 2023
;      Reviewed :  No
;      Purpose :   Wozmon routines.
;
; ***************************************************************************************
; ***************************************************************************************

PrintHexByte:
	pha
	lsr 	a
	lsr 	a
	lsr 	a
	lsr 	a
	jsr 	_PrintHexNibble
	pla

_PrintHexNibble:	
	and 	#15
	cmp 	#10
	bcc 	_nothex
	adc 	#6
_nothex:
	adc 	#48
	jsr 	KWriteCharacter
	rts
