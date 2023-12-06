; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		let.asm
;		Purpose:	Assignment statement
;		Created:	6th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;								LET <reference> = <value>
;
; ************************************************************************************************

		.section code

Command_Let: ;; [let]
		ldx 	#0 							; slot 0, same precedence as ! operator -1
		lda 	KWD_PLING-$20+BinaryPrecedence		
		dec 	a
		jsr 	EXPEvaluateExpressionPrecedenceA
		lda 	#KWD_EQUAL 					; check '=' operator.
		jsr 	ERRCheckA
		inx
		jsr 	EXPEvalNumber 				; rhs in slot 1.
		dex
		jsr 	AssignValueToReference
		rts
		
; ************************************************************************************************
;
;									Assign value X+1 to reference X
;
; ************************************************************************************************

AssignValueToReference:
		.byte 	3
		lda 	XSControl,x 				; check types match
		eor 	XSControl+1,x
		bmi 	_AVTType 					; they don't.

		lda 	XSControl,x 				; check it's a reference.
		and 	#XS_ISREFERENCE 			
		beq 	_AVTRReference

		lda 	XSControl,x 				; check type.
		bmi 	_AVTRString
		;
		;		Number assignment. Different code for indirection and variable access.
		;



_AVTType:
		.error_type
_AVTRReference:
		.error_reference

_AVTRString:

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

