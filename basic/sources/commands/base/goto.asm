; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		goto.asm
;		Purpose:	GOTO line number
;		Created:	7th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;										GOTO Command
;
; ************************************************************************************************

		.section code

Command_GOTO:	;; [goto]
		ldx 	#0
		jsr 	EXPEvalInteger16 			; get line number to level 0.
GotoR0:		
		lda 	Program 					; back to the program start
		clc
		adc 	#(Program >> 8)
		sta 	codePtr+1
		stz 	codePtr
		;
_GOSearch:		
		lda 	(codePtr) 					; end of program.
		beq 	_GOError
		ldy 	#1 							; found line #
		lda 	(codePtr),y
		cmp 	XSNumber0
		bne 	_GONext
		iny
		lda 	(codePtr),y
		cmp 	XSNumber1
		bne 	_GONext
		jmp 	RUNNewLine

_GONext:		
		clc 								; advance to next line.
		lda 	(codePtr)
		adc 	codePtr
		sta 	codePtr
		bcc 	_GOSearch
		inc 	codePtr+1
		bra 	_GOSearch
		
_GOError:		
		.error_line

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

