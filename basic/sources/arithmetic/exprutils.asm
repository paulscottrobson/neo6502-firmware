; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		exprutils.asm
;		Purpose:	Evaluate expression helpers.
;		Created:	1st December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section code	

; ************************************************************************************************
;
;								Evaluate a numeric expression
;
; ************************************************************************************************

EXPEvalNumber:
		jsr 	EXPEvaluateExpressionAtX	; get a value
		jsr 	DereferenceTOS				; dereference it
		bit 	XSControl,x	 				; fail if string
		bmi 	EVUType
		rts

EVUType:
		.error_type

; ************************************************************************************************
;
;								Evaluate an integer (various)
;
; ************************************************************************************************

EXPEvalInteger:
		jsr 	EXPEvalNumber 				; get number, coeerce to integer.
		bit 	XSControl,x 				; integer ?
		beq 	_EEIExit
		lda 	#17 						; no, make it integer
		jsr 	DOMathCommand
_EEIExit:		
		rts

EXPEvalInteger16:
		jsr 	EXPEvalInteger 				; get integer
		lda 	XSNumber3,x
		ora 	XSNumber2,x
		bne 	EVURange
		rts

EXPEvalInteger8:
		jsr 	EXPEvalInteger 				; get integer
		lda 	XSNumber1,x 				; range 00-FF
		bne 	EVURange
		lda 	XSNumber0,x
		rts

; ************************************************************************************************
;
;								Evaluate a string expression
;
; ************************************************************************************************

EXPEvalString:
		jsr 	EXPEvaluateExpressionAtX 	; get a value
		jsr 	DereferenceTOS 				; dereference it
		bit 	XSControl,x	 				; fail if not string
		bpl 	EVUType
		lda 	XSNumber0,x
		sta 	zTemp0
		lda 	XSNumber1,x
		sta 	zTemp0+1
		rts

EVURange:
		.error_range
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

