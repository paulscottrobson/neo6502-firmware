; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		call.asm
;		Purpose:	Call procedure
;		Created:	11th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section code

; ************************************************************************************************
;
;										CALL command
;
; ************************************************************************************************

Command_CALL:	;; [call]
		lda 	#STK_CALL
		jsr 	StackOpen 

		lda 	(codePtr),y 				; check identifier follows.
		cmp 	#$20 
		bcs 	_CCSyntax

		pha 								; save identifier offset address on the stack
		iny
		lda 	(codePtr),y
		pha
		iny
;
;		Evaluate parameters onto stack.
;
		stz 	paramCount
		lda 	(codePtr),y 				; check if parameters
		cmp 	#KWD_RPAREN
		beq 	_CCEndCall

_CCParamLoop		
		inc 	paramCount 					; one more parameter
		ldx 	paramCount 					; put in that slot, e.g. param#1 => slot #1
		jsr 	EXPEvaluateExpressionAtX 	; evaluate it
		jsr 	DereferenceTOS 				; dereference it.
		lda 	(codePtr),y 				; get next
		cmp 	#KWD_RPAREN 				; ) ?
		beq 	_CCEndCall
		iny 								; consume
		cmp 	#KWD_COMMA 					; get next if comma
		beq 	_CCParamLoop
		.error_syntax 						

_CCEndCall:		
		iny 								; consume closing parameters
		jsr 	STKSaveCodePosition	 		; save return address onto stack

		pla 								; get address of line.
		sta 	zTemp0
		pla
		clc
		adc 	#Program >> 8
		sta 	zTemp0+1
		;
		ldy 	#4 							; is it a procedure ?
		lda 	(zTemp0),y
		cmp 	#XS_ISPROC 
		bne 	_CCUnknown 					; nope.
		lda 	(zTemp0) 					; copy proc address
		sta 	codePtr		
		ldy 	#1
		lda 	(zTemp0),y
		sta 	codePtr+1
		ldy 	#6 							; after the identifier
		ldx 	#0  						; now start localising & copying parameters
_CCUpdateParamLoop:		
		cpx 	paramCount 					; done all the parameters ?
		beq 	_CCDoneParameters
		jsr 	LocaliseNextTerm 			; get a term into the slot.
		jsr 	AssignValueToReference	 	; assign the parameter value to the parameter variable
		inx 								; do the next one.
		lda 	(codePtr),y 				; if , follows then consume it
		cmp 	#KWD_COMMA
		bne 	_CCUpdateParamLoop
		iny
		bra 	_CCUpdateParamLoop

_CCDoneParameters:
		jsr 	ERRCheckRParen 				; check right bracket and continue.
		rts

_CCSyntax:
		.error_syntax
_CCUnknown:
		.error_unknown

; ************************************************************************************************
;
;										ENDPROC command
;
; ************************************************************************************************

Command_ENDPROC:	;; [endproc]
		jsr		StackRemoveLocals
_CEContinue:		
		lda 	#STK_CALL
		jsr 	StackCheckFrame
		jsr 	STKLoadCodePosition 		
		jsr 	StackClose		 			
		rts

		.send code

; ************************************************************************************************
;
;		Date			Notes
;		==== 			=====
;
; ************************************************************************************************
