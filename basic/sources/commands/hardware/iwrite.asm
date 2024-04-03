; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		iwrite.asm
;		Purpose:	Write to i2C device
;		Created:	24th February 2024
;		Reviewed: 	No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section code

; ************************************************************************************************
;
;									IWRITE <dev>,<addr>,<data>
;
; ************************************************************************************************

IWriteCommand: ;; [iwrite]
		ldx 	#0
		jsr 	EXPEvalInteger16			; device address
		jsr 	ERRCheckComma
		inx
		jsr 	EXPEvalInteger8				; register address
		jsr 	ERRCheckComma

		ldx 	#2 							; register data
		jsr 	EXPEvalInteger8

		lda 	XSNumber0 					; send data out.
		sta 	ControlParameters+0
		lda 	XSNumber0+1
		sta 	ControlParameters+1
		lda 	XSNumber0+2
		sta 	ControlParameters+2

		.DoSendMessage 					
		.byte 	10,5
		.DoWaitMessage

		lda 	ControlError
		bne 	_IWFail
		rts

_IWFail:
		.error_hardware		

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
