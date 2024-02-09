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

		jsr 	EXPEvaluateExpressionAtX 	; evaluate the parameter value, keep as referenced.
		lda 	(codePtr),y 				; get next
		cmp 	#KWD_RPAREN 				; ) ?
		beq 	_CCEndCall
		iny 								; consume
		cmp 	#KWD_COMMA 					; get next if comma
		beq 	_CCParamLoop
_CCSyntax:
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

		lda 	(codePtr),y 				; save next byte on stack
		pha
		cmp 	#KWD_REF  					; consume if it is REF
		bne 	_CCNotReference1
		iny
_CCNotReference1:

		jsr 	EvaluateTerm 				; evaluate the term (the target variable)
		lda 	XSControl,x 				; check it is a reference
		and 	#XS_ISVARIABLE
		beq 	_CCSyntax

		jsr 	LocaliseTermX 				; localise that term.

		pla    								; get REF perhaps
		cmp 	#KWD_REF
		bne 	_CCNotReference2
		jsr 	CCCreateRestore 			; create code to copy the result.
_CCNotReference2:		

		inx  								; dereference the parameter value 
		jsr 	DereferenceTOS 				
		dex
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

_CCUnknown:
		.error_unknown

; ************************************************************************************************
;
;		Create a result copy. On entry Stack,X+1 is the *target* variable - this is where the 
;		value that is copied back goes. Stack,X is the *source* variable - this is the value
;		that is copied back - what is currently in the parameter
;
; ************************************************************************************************

CCCreateRestore:
		lda 	XSControl+1,x 				; check the parameter value, is actually a reference
		and 	#XS_ISVARIABLE
		beq 	_CCCSyntax
		;
		lda 	XSControl,x 				; push the type byte
		jsr 	StackPushByte
		lda 	XSNumber0,x  				; push the source on first
		jsr 	StackPushByte
		lda 	XSNumber1,x
		jsr 	StackPushByte
		lda 	XSNumber0+1,x  				; push the target first
		jsr 	StackPushByte
		lda 	XSNumber1+1,x
		jsr 	StackPushByte
		lda 	#STK_REFPARAM 				; push the reference marker.
		jsr 	StackPushByte
		rts
_CCCSyntax:
		.error_syntax

; ************************************************************************************************
;
;									 Copy reference back
;
; ************************************************************************************************

CCCopyReferenceBack:
		phy
		jsr 	StackPopByte 				; throw reference marker.
		;
		jsr 	StackPopByte 				; pop the target
		sta 	zTemp0+1
		jsr 	StackPopByte
		sta 	zTemp0
		;
		jsr 	StackPopByte 				; pop the value to copy
		sta 	zTemp1+1
		jsr 	StackPopByte
		sta 	zTemp1
		;
		jsr 	StackPopByte 				; this is the type byte.
		and 	#XS_ISSTRING
		bne 	_CCCStringReference
		;
		;		Copy number reference back
		;
		ldy 	#3 							; copy it back
_CCCopyBack:
		lda 	(zTemp1),y
		sta 	(zTemp0),y
		dey
		bpl 	_CCCopyBack
		ply
		rts
		;
		;		Copy string reference back
		;
_CCCStringReference:
		phx
		lda 	(zTemp1)
		sta 	zsTemp
		ldy 	#1
		lda 	(zTemp1),y
		sta 	zsTemp+1

		ldy 	zTemp0+1
		lda 	zTemp0
		jsr 	StringConcrete
		plx
		ply	
		rts
		.error_type

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
;		08-02-24 		Add reference parameter code
;		09-02-24 		Add string reference code.
;
; ************************************************************************************************
