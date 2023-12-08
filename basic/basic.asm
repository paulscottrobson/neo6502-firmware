; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		basic.asm
;		Purpose:	BASIC main program
;		Created:	25th November 2023
;		Reviewed:	No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************


; ************************************************************************************************
;
;										   Main Program
;
; ************************************************************************************************

		.section code

boot:	jmp 	ColdStart

ColdStart:	
		jmp 	CheckSpeed
		jmp 	Command_RUN

		.send 	code

		.include "_include.inc"


		.section code


CheckSpeed:
		pha
		lda 	#100
		ldx 	#0
		ldy 	#0
_Loop1:	dey
		bne 	_Loop1
		dex
		bne 	_Loop1
		dec 	a
		bne 	_Loop1		
		pla
		inc 	a
		and 	#7
		adc		#48
		jsr 	$FFF1
		bra 	CheckSpeed
;
;									Temp bodges of various kinds.
;
WarmStart:
		lda 	#$00
		tax
		tay
		.byte 	3
		bra 	WarmStart

Unimplemented:
		lda 	#$FF
ErrorHandler:
		ldx 	#$EE
		ldy 	#$EE
		.byte 	3
_EHLoop:
		bra 	_EHLoop		

		.align 	256
Program:
		.binary "build/tokenised.dat"

		.send code


; ************************************************************************************************
;
;									Changes and Updates
;
; ************************************************************************************************
;
;		Date			Notes
;		==== 			=====
;
; ************************************************************************************************

