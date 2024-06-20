; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		exists.asm
;		Purpose:	Check file exists
;		Created:	20th June 2024
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;										Check file exists
;
; ************************************************************************************************

		.section code	

EXPUnaryExists: ;; [exists(]
		jsr 	EXPEvalString 					; string to R0, zTemp0		
		jsr 	ERRCheckRParen 					; )

		lda 	zTemp0 							; copy name to Parameters 0,1
		sta 	ControlParameters+0
		lda 	zTemp0+1
		sta 	ControlParameters+1

		.DoSendMessage 							; do file stat
		.byte 	3,16
		.DoWaitMessage				

		lda 	ControlError 					; error ?
		beq 	_EUENotFlip 					; return !error
		lda 	#$FF
_EUENotFlip:
		eor 	#$FF		
		jmp 	ReturnBoolean

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

