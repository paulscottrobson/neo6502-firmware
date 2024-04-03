; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		iread.asm
;		Purpose:	Read I2C Device
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

UnaryIRead: ;; [iread(]	
		jsr 	EXPEvalInteger8				; get device to read
		pha
		jsr 	ERRCheckComma
		jsr 	EXPEvalInteger8 			; get register to read
		pha
		jsr 	ERRCheckRParen 				

		pla 								; set up for read
		sta 	ControlParameters+1
		pla 	
		sta 	ControlParameters

		.DoSendMessage 						; read it.
		.byte 	10,6
		.DoWaitMessage
		lda 	ControlError
		bne 	_URError
		lda 	ControlParameters  			; return 0 / 1
		jmp 	EXPUnaryReturnA

_URError
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
;		22-02-24 		PIN() returns 0 or 1.
;
; ************************************************************************************************
