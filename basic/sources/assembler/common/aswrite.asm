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
		lda 	VariableO 					; check printing bytes ?
		and 	#2
		beq 	_AWBNoPrint
		;
		lda		VariableP+1 				; print address
		jsr 	PrintHex
		lda		VariableP
		jsr 	PrintHex
		lda 	#' '
		jsr 	WriteCharacter
		pla 								; print byte
		pha
		jsr 	PrintHex
		lda 	#13
		jsr 	WriteCharacter
_AWBNoPrint:	
		lda		VariableP					; copy address to zTemp0
		sta 	zTemp0
		lda		VariableP+1
		beq 	_AWBRange
		sta 	zTemp0+1
		pla 								; write byte out
		sta 	(zTemp0)
		inc 	VariableP 					; bump address
		bne 	_AWBNoCarry
		inc 	VariableP+1
_AWBNoCarry:	
		rts
	
_AWBRange:
		.error_range

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
		jmp 	WriteCharacter

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
