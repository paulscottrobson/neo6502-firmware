; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		defchr.asm
;		Purpose:	Define character
;		Created:	11th January 2024
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;									Define character
;
; ************************************************************************************************

		.section code

Command_DEFCHR:	;; [defchr]
		ldx 	#0 							; character to define
		jsr 	EXPEvalInteger8
		pha
_CDGet:	jsr 	ERRCheckComma 				; get the definition.
		jsr 	EXPEvalInteger8
		inx
		cpx 	#7
		bne 	_CDGet		
		pla  								; save defined character
		sta 	ControlParameters 
		ldx 	#0  						; copy definition
_CDCopy:lda 	XSNumber0,x
		asl 	a
		asl 	a
		sta 	ControlParameters+1,x
		inx
		cpx 	#7
		bne 	_CDCopy
		.DoSendMessage
		.byte 	2,5
		.DoWaitMessage
		lda 	ControlError
		bne 	_CDRange
		rts
_CDRange:
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

