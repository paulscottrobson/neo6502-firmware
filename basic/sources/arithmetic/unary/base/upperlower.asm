; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		upperlower.asm
;		Purpose:	Convert string to upper/lower case
;		Created:	15th April 2024
;		Reviewed: 	No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section code

; ************************************************************************************************
;
; 									UPPER$() and LOWER$() function
;
; ************************************************************************************************

UnaryUpper: ;; [upper$(]	
		clc
		bra 	CaseConversion
UnaryLower: ;; [lower$(]	
		sec
CaseConversion:
		php 								; carry flag set lower, clear upper.
		jsr 	EXPEvalString 				; string to zTemp0		
		jsr 	ERRCheckRParen

		lda 	(zTemp0) 					; allocate space for whole string
		jsr 	StringTempAllocate

		plp 								; save position, put the conversion flag on TOS though
		phy 
		php
		ldy 	#0 							; start of string pre decrement
_CCLoop:
		cmp 	(zTemp0)	 				; exit if reached length.
		beq 	_CCDone
		iny 								; increment as length prefixed.

		lda 	(zTemp0),y 					; first make it upper case anyway.
		cmp 	#"a"
		bcc 	_CCNotLower
		cmp 	#"z"+1
		bcs 	_CCNotLower
		eor 	#$20
_CCNotLower:
		plp 								; test flag.
		php
		bcc 	_CCOutput 					; if going to upper, already done.

		cmp 	#"A" 						; test if upper case
		bcc 	_CCOutput
		cmp 	#"Z"+1
		bcs 	_CCOutput
		eor 	#$20 						; if so make lower case.

_CCOutput:
		jsr 	StringTempWrite 			; write to string.
		bra 	_CCLoop

_CCDone:
		ply 								; throw conversion flag.
		ply 								; restore Y and exit
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
