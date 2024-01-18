; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		point.asm
;		Purpose:	Read pixel from screen
;		Created:	18th January 2024
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;									Read Pixel from screen
;
; ************************************************************************************************

		.section code	

EXPUnaryPoint: ;; [point(]
		jsr 	EXPEvalInteger16 			; x
		inx
		jsr 	ERRCheckComma
		jsr 	EXPEvalInteger16 			; y
		dex
		jsr 	ERRCheckRParen

		lda 	XSNumber0,x 				; copy into parameter block.
		sta 	ControlParameters+0
		lda 	XSNumber1,x
		sta 	ControlParameters+1
		lda 	XSNumber0+1,x
		sta 	ControlParameters+2
		lda 	XSNumber1+1,x
		sta 	ControlParameters+3

		.DoSendMessage 						; read pixel
		.byte 	5,33
		.DoWaitMessage

		lda 	ControlError 				; failed ?
		bne 	_EUPError

		lda 	ControlParameters+0 		; true/false option.
		jmp 	EXPUnaryReturnA

_EUPError:
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

