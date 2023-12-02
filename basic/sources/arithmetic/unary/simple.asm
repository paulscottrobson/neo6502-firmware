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
		jsr 	EXPEvaluateExpression 		; evaluate at stack level X
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
		jsr 	EXPEvaluateExpression 		; one operand
		pla
		jsr 	DoMathCommand
		lda 	ControlError
		bne 	_DUFError
		jmp 	ERRCheckRParen 				; closing )
_DUFError:
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

UnaryAbs: ;; [abs(]
		.single 	25

UnarySgn: ;; [sgn(]
		.single 	26

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

; rand( rnd( 

; <string> $ asc len val isval 
