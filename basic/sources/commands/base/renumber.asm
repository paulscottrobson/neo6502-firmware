; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		renumber.asm
;		Purpose:	Renumber the program
;		Created:	7th February 2024
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;										RENUMBER Command
;
; ************************************************************************************************

		.section code

Command_Renumber:	;; [renumber]
		lda 	#1000 & $FF 				; default renumberererererere....
		sta 	XSNumber0
		lda 	#1000 >> 8
		sta 	XSNumber1
		lda 	(codePtr),y 				; any parameter ?
		cmp 	#KWD_COLON
		beq 	_CRMain
		cmp 	#KWD_SYS_END
		beq 	_CRMain 
		ldx 	#0
		jsr 	EXPEvalInteger16 			; get line number for renumber start.
_CRMain:
		sec 								; CS, just does the check
		ldx 	#10 
		jsr 	RenumberProgram
		bcs 	_CRError 					; cannot do it, number too large
		clc  								; now actually do it.
		lda 	#10 
		jsr 	RenumberProgram
		jmp 	WarmStart 					; and warm start
_CRError:
		.error_range

; ************************************************************************************************
;
;					Renumber code. CC actually renumbers. CS overflow error on exit
;										XSNumber0/1 start line, X step
;
; ************************************************************************************************

RenumberProgram:
		php 								; save renumber do flag on stack

		lda 	XSNumber0  					; start line to zTemp0
		sta 	zTemp0
		lda 	XSNumber1
		sta 	zTemp0+1

		lda 	#Program >> 8 				; start of program to codePtr
		clc
		adc 	Program
		sta 	codePtr+1
		stz 	codePtr

_RenumLoop:
		lda 	(codePtr) 					; end of program
		clc
		beq 	_RenumExit 					; if so, return with carry clear as okay.
		;
		plp 								; access overwrite flag.
		php
		bcs 	_RenumSkip 					; if CS don't actually renumber
		;
		lda 	zTemp0 						; update the line
		ldy 	#1
		sta 	(codePtr),y
		lda 	zTemp0+1
		iny
		sta 	(codePtr),y
_RenumSkip:
	
		txa 								; work out new line number
		clc
		adc 	zTemp0
		sta 	zTemp0
		bcc 	_RenumNext
		inc 	zTemp0+1 					; next upper byte
		sec 								; if zero, e.g. wrap around, then exit with CS (failed)
		beq 	_RenumExit
_RenumNext:
		lda 	(codePtr) 					; next program line.
		clc
		adc 	codePtr
		sta 	codePtr		
		bcc 	_RenumLoop
		inc 	codePtr+1
		bra 	_RenumLoop

_RenumExit:
		pla 								; throw the saved renumber do flag
		rts

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

