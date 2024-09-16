; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		not.asm
;		Purpose:	Logical Not
;		Created:	16th September 2024
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;								Return logical NOT of parameter.
;
; ************************************************************************************************

		.section code	

EXPUnaryNot: ;; [not]
		jsr 	EvaluateTerm				; not takes one term because of precedence
		bit 	XSControl,x	 				; fail if string
		bmi 	_EXNType
		bvc 	_EXNIsInteger
		lda 	#17 						; if real make it integer
		jsr 	DOMathCommand
_EXNIsInteger:		
		lda 	XSNumber3,x 				; check if zero.
		ora 	XSNumber2,x
		ora 	XSNumber1,x
		ora 	XSNumber0,x
		beq 	_EXNIsZero 					; make it 0 or $FF
		lda 	#$FF
_EXNIsZero:		
		eor 	#$FF 						; now zero will be $FF and anything else $00
		jmp 	ReturnBoolean


_EXNType:
		.error_type

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

