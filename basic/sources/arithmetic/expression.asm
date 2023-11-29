; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		expression.asm
;		Purpose:	Evaluate expression to R0
;		Created:	29th November 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;				Evaluate an expression into Stack,X from (codePtr),y Precedence A.
;
; ************************************************************************************************

		.section code	

EXPEvaluateExpressionRestart:
		ldx 	#0 							; start from base of stack.
EXPEvaluateExpression:
		lda 	#0 							; current precedence is 0.
EXPEvaluateExpressionPrecedenceA:
		pha		 							; push on stack.
		jsr 	EvaluateTerm 				; evaluate term at this level.
		;
		;		Main loop (precedence on TOS)
		;		
EXPRMainLoop:		
		lda 	(codePtr),y 				; what follows needs to be a binary operator
		and 	#$E0 						; is it $20-$3F, e.g. a binary operator.
		cmp 	#$20
		bne 	_EEExit
		;
		phx 			 					; get binary precedence for current operator -> zTemp0+1
		lda 	(codePtr),y 				; get the operator again
		tax
		lda 	BinaryPrecedence-$20,x 		; get the binary operator precedence.
		sta 	zTemp0+1
		plx
		;
		pla 								; restore current precedence.
		cmp 	zTemp0+1 					; if >= operator precedence then exit
		bcs 	_EEExit2
		;
		pha 								; save current precedence on the 6502 stack.
		lda 	(codePtr),y 				; get the operator back.		
		iny 								; consume operator.
		pha 								; save the operator.

		lda 	zTemp0+1					; get precedence of operator
		inx 								; space for it on the stack and do the RHS.
		jsr 	EXPEvaluateExpressionPrecedenceA
		dex 								; back to original value.

		pla 								; load operator
		and 	#$1F 						; mask out bits
		asl 	a 							; double as look up table, also Clear Carry
		adc 	#(BinaryVectorTable & $FF) 	; work out the address to do the indirect jump.
		sta 	_EXPRIndirectJump+1
		lda 	#(BinaryVectorTable >> 8)
		adc 	#0
		sta 	_EXPRIndirectJump+2

_EXPRIndirectJump:		
		jmp 	($FFFF) 					; jump to the code routine, loops back to EXPRMainLoop

_EEExit:
		pla 								; throw precedence
_EEExit2:		
		rts

; (===================================================================================================)
; Placeholder fix up.

BinaryVectorTable:
		.word 	0
		.word 	BodgeMinus

; Placeholder TODO: Routine now does dereferencing, type testing, and similar, and preserves Y'
		
BodgeMinus: ;; [-]
		sec
		lda 	XSNumber0,x
		sbc 	XSNumber0+1,x
		sta 	XSNumber0,x
		lda 	XSNumber1,x
		sbc 	XSNumber1+1,x
		sta 	XSNumber2,x
		lda 	XSNumber2,x
		sbc 	XSNumber2+1,x
		sta 	XSNumber2,x
		lda 	XSNumber3,x
		sbc 	XSNumber3+1,x
		sta 	XSNumber3,x
		jmp 	EXPRMainLoop

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

