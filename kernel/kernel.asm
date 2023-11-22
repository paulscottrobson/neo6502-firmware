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

	DCommand = $FF00
	DFunction = $FF01
	DError = $FF02
	DControl = $FF03
	DParameters = $FF04
	DTopOfStack = $FF0C

	.include 	"support.asm"
	.include 	"wozmon.asm"

start
	cld
	cli
	ldx #$ff
	txs
	lda 	#42
	jsr 	KWriteCharacter
	jsr 	KReadCharacter
	jsr 	PrintHexByte
	lda 	#-1
	sta 	$81
loop2:
	jsr 	alphabet	
	inc 	$81
	lda 	$81
	jsr 	PrintHexByte
	lda 	#13
	jsr 	KWriteCharacter
	lda 	#10
	jsr 	delay
_opcode:	
	bra 	loop2			

delay:
	ldy 	#0
	ldx 	#0
loop1:
	dey
	bne 	loop1
	dex 
	bne 	loop1	
	dec		a
	bne 	loop1
	rts

alphabet:	
	ldx 	#5
	phx
	ply
_loop1:
	tya
	clc
	adc 	#$1C
	jsr PrintHexByte
	dey
	bpl 	_loop1
	lda 	#42
	jsr 	PrintHexByte
	rts

	* = $FF00
	.word 	0,0,0,0,0,0,0,0

	.include "build/_vectors.inc"
	
	* = $FFFA
	.word 	start
	.word 	start
	.word 	start
