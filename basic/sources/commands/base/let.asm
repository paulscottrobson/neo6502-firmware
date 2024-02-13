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
		ldx 	#0 							; slot 0
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
		lda 	XSControl,x 				; is it a reference ?
		and 	#XS_ISVARIABLE
		beq 	_AVTRSyntax
		;
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
		;
		lda 	XSControl,x 				; check if target is a 16 bit reference.
		and 	#XS_ISWORDREF
		bne 	_AVTR16Bit  				; if so, exit now.
		;
		iny  								; do the rest & type
		lda 	XSNumber2+1,x
		sta 	(zTemp0),y
		iny
		lda 	XSNumber3+1,x
		sta 	(zTemp0),y
		iny
		lda 	XSControl+1,x 				; copy the type
		and 	#$FF-XS_ISVARIABLE 			; without the variable bit set
		sta 	(zTemp0),y
_AVTR16Bit:
		ply
		rts


_AVTRSyntax:
		.error_syntax
_AVTRType:
		.error_type		
		;
		;		Concrete string into variable address.
		;
_AVTRString:
		lda 	XSNumber0+1,x 				; copy target string to zsTemp
		sta 	zsTemp
		lda 	XSNumber1+1,x
		sta 	zsTemp+1
		;
		phy
		ldy 	XSNumber1,x 				; YA is the address of the current concreted string if any
		lda 	XSNumber0,x
		phx
		jsr 	StringConcrete
		plx
		ply
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
;		13-02-24 		Added [] assignment
;
; ************************************************************************************************

