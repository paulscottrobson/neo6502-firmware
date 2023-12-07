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
		jsr 	EvaluateTerm 				; term, which is the variable to assign to in slot 0.
		lda 	#KWD_EQUAL 					; check '=' operator.
		jsr 	ERRCheckA
		inx
		jsr 	EXPEvaluateExpressionAtX	; get a value
		jsr 	DereferenceTOS				; dereference it
		dex
		jsr 	AssignValueToReference
		rts
		
; ************************************************************************************************
;
;									Assign value X+1 to reference X
;
; ************************************************************************************************

AssignValueToReference:
		lda 	XSControl,x 				; do the types match ?
		eor 	XSControl+1,x
		bmi 	_AVTRType 					; no, error.
		lda 	XSControl,x
		bmi 	_AVTRString
		;
		;		Number assign.
		;
		lda 	XSNumber0,x 				; copy address to zTemp0
		sta 	zTemp0
		lda 	XSNumber1,x
		sta 	zTemp0+1
		;
		;		Copy data and type out.
		;
		phy
		lda 	XSNumber0+1,x 				; copy the value into the slot.
		sta 	(zTemp0)
		ldy 	#1 				
		lda 	XSNumber1+1,x
		sta 	(zTemp0),y
		iny
		lda 	XSNumber2+1,x
		sta 	(zTemp0),y
		iny
		lda 	XSNumber3+1,x
		sta 	(zTemp0),y
		iny
		lda 	XSControl+1,x 				; copy the type
		and 	#$FF-XS_ISVARIABLE 			; without the variable bit set
		sta 	(zTemp0),y
		ply
		rts

_AVTRType:
		.error_type		
		;
		;		Concrete string into variable address.
		;
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

