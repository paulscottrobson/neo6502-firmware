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
		; 		TODO: Evaluate parameters on stack.
		;
		jsr 	ERRCheckRParen 				; closing )
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
		;
		;		TODO: Evaluate terms and save parameters there.
		;
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
		; 		
		;		TODO: Unwind locals and parameters
		;
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
