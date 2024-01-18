; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		val.asm
;		Purpose:	String to Decimal
;		Created:	2nd December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;											VAL/ISVAL
;
; ************************************************************************************************

		.section code	

EXPUnaryVal: ;; [val(]
		jsr 	EXPValBody 						; do the main bit.
		bne 	_EXPUVExit 						; check error.
		rts
_EXPUVExit:
		.error_range		

EXPUnaryIsVal: ;; [isval(]
		jsr 	EXPValBody 						; do the main bit.
		beq 	EXPUnaryReturnMinus1 			; error code.

EXPUnaryReturnZero:		
	 	lda 	#0 								; returns zerozero no error
	 	jmp 	EXPUnaryReturnA

EXPUnaryReturnMinus1:	 	
		lda 	#$FF 							; returns -1
		sta 	XSNumber0,x
		sta 	XSNumber1,x
		sta 	XSNumber2,x
		sta 	XSNumber3,x
		stz 	XSControl,x
		rts

; ************************************************************************************************
;
;											Common code
;
; ************************************************************************************************

EXPValBody:
		jsr 	EXPEvalString 					; string to R0, zTemp0		
		jsr 	ERRCheckRParen 					; )

		lda 	zTemp0 							; copy address to offset 8/9
		sta 	ControlParameters+4
		lda 	zTemp0+1
		sta 	ControlParameters+5

		lda 	#33
		jsr 	DoMathCommand
		lda 	ControlError 					; error occurred
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
;
; ************************************************************************************************

