; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		aswrite.asm
;		Purpose:	Write to memory / display maybe
;		Created:	27th December 2023
;		Reviewed: 	No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

	.section 	code

; ************************************************************************************************
;
;							Write a byte to the current assembler address
;
; ************************************************************************************************

AssemblerWriteByte:		
	pha
	lda 	AssemblerControl 				; check printing bytes ?
	and 	#2
	beq 	_AWBNoPrint
	;
	lda		AssemblerAddress+1 				; print address
	jsr 	PrintHex
	lda		AssemblerAddress
	jsr 	PrintHex
	lda 	#' '
	jsr 	EXTPrintCharacter
	pla 									; print byte
	pha
	jsr 	PrintHex
	lda 	#13
	jsr 	EXTPrintCharacter
_AWBNoPrint:	
	lda		AssemblerAddress				; copy address to zTemp0
	sta 	zTemp0
	lda		AssemblerAddress+1
	beq 	_AWBRange
	sta 	zTemp0+1
	pla 									; write byte out
	sta 	(zTemp0)
	inc 	AssemblerAddress 				; bump address
	bne 	_AWBNoCarry
	inc 	AssemblerAddress+1
_AWBNoCarry:	
	rts
	
_AWBRange:
	jmp 	RangeError

; ************************************************************************************************
;
;										Print A in Hex
;
; ************************************************************************************************

PrintHex:
		pha
		lsr 	a
		lsr 	a
		lsr 	a
		lsr 	a
		jsr 	_PrintNibble
		pla
_PrintNibble:
		and 	#15
		cmp 	#10
		bcc 	_NoShift
		adc 	#6
_NoShift:
		adc 	#48
		jmp 	EXTPrintCharacter

	.send 		code


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
