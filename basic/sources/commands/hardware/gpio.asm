; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		gpio.asm
;		Purpose:	PIN command
;		Created:	20th February 2024
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;										PIN Command
;
; ************************************************************************************************

		.section code

Command_PIN:	;; [pin]

		ldx 	#0
		jsr 	EXPEvalInteger8 			; pin number
		pha		
		jsr 	ERRCheckComma
		jsr 	EXPEvalInteger 				; pin value
		lda 	XSNumber0,x
		ora 	XSNumber1,x
		ora 	XSNumber2,x
		ora 	XSNumber3,x
		sta 	ControlParameters+1
		pla
		sta 	ControlParameters

		.DoSendMessage 						; send message 10,2
		.byte 10,2
		.DoWaitMessage

		lda 	ControlError  				; check it worked.		
		bne 	_CFError
		rts
_CFError:
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

