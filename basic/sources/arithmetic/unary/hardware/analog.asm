; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		analog.asm
;		Purpose:	Read Analog device
;		Created:	24th February 2024
;		Reviewed: 	No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section code

; ************************************************************************************************
;
; 										IREAD() function
;
; ************************************************************************************************

UnaryAnalog: ;; [analog(]	
		jsr 	EXPEvalInteger8				; get pin to check
		pha
		jsr 	ERRCheckRParen 				
		pla
		sta 	ControlParameters
		.DoSendMessage 						; read it.
		.byte 	10,7
		.DoWaitMessage
		lda 	ControlError
		bne 	_UPRange
		lda 	ControlParameters  			; return result.
		sta 	XSNumber0,x
		lda 	ControlParameters+1
		sta 	XSNumber1,x
		stz 	XSNumber2,x
		stz 	XSNumber3,x
		stz 	XSControl,x
		rts
_UPRange:
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
;		22-02-24 		PIN() returns 0 or 1.
;
; ************************************************************************************************
