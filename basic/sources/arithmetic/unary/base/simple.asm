; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		simple.asm
;		Purpose:	Simple Unary functions
;		Created:	1st December 2023
;		Reviewed:	No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section 	code

; ************************************************************************************************
;
; 								Parenthesis unary function 
;
; ************************************************************************************************

UnaryParenthesis: ;; [(]
		jsr 	EXPEvaluateExpressionAtX	; evaluate at stack level X
		jsr 	DereferenceTOS 				; dereference it.
		jmp 	ERRCheckRParen	 			; check )

; ************************************************************************************************
;
;								Unary functions, 1 parameter
;
; ************************************************************************************************

single 	.macro
		lda 	#\1 						; do appropriate command
		bra 	DoUnaryFunction
		.endm

DoUnaryFunction:	
		pha	
		jsr 	EXPEvalNumber 				; one operand
		pla
		jsr 	DoMathCommand
		lda 	ControlError
		bne 	DUFError
		jmp 	ERRCheckRParen 				; closing )
DUFError:
		.error_range

UnaryInt: ;; [int(]
		.single 	17

UnarySqr: ;; [sqr(]
		.single 	18

UnarySin: ;; [sin(]
		.single 	19

UnaryCos: ;; [cos(]
		.single 	20

UnaryTan: ;; [tan(]
		.single 	21

UnaryATan: ;; [atan(]
		.single 	22

UnaryExp: ;; [exp(]
		.single 	23

UnaryLog: ;; [log(]
		.single 	24

UnaryRnd: ;; [rnd(]
		.single 	27

UnaryRand: ;; [rand(]
		.single 	28

; ************************************************************************************************
;
;										Optimisable ABS
;
; ************************************************************************************************

UnaryAbs: ;; [abs(]			
		jsr 	EXPEvalNumber 				; one operand
		jsr 	ERRCheckRParen 				; closing )
		lda 	XSControl,x
		and 	#XS_TYPEMASK
		beq 	_UAAbsInteger
		lda 	#25
		jsr 	DoMathCommand
		lda 	ControlError
		bne 	DUFError
_UAExit:
		rts

_UAAbsInteger: 								; integer optimised.
		lda 	XSNumber3,x
		bpl 	_UAExit
		jmp 	UnaryNegateInteger

; ************************************************************************************************
;
;										Optimisable SGN
;
; ************************************************************************************************

UnarySgn: ;; [sgn(]
		jsr 	EXPEvalNumber 				; one operand
		jsr 	ERRCheckRParen 				; closing )
		lda 	XSControl,x
		and 	#XS_TYPEMASK
		beq 	_UASgnInteger
		lda 	#26
		jsr 	DoMathCommand
		lda 	ControlError
		bne 	DUFError
_UASGNExit:		
		rts

_UASGNInteger:
		lda 	XSNumber0,x		
		ora 	XSNumber1,x		
		ora 	XSNumber2,x		
		ora 	XSNumber3,x		
		beq 	_UASGNExit
		;
		lda 	XSNumber3,x
		and 	#$80
		beq 	_USIsPositive
		lda 	#$FF
_USIsPositive:
		sta 	XSNumber0,x		
		sta 	XSNumber1,x		
		sta 	XSNumber2,x		
		sta 	XSNumber3,x		
		bne 	_UASGNExit
		inc 	XSNumber0,x
		rts

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

