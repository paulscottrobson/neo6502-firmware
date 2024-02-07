; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		fkey.asm
;		Purpose:	Define a function key
;		Created:	30th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;										FKEY Command
;
; ************************************************************************************************

		.section code

Command_FKEY:	;; [fkey]

		lda 	(codePtr),y 				; check for FKEY on its own
		cmp 	#KWD_COLON
		beq 	_CFList
		cmp 	#KWD_SYS_END
		beq 	_CFList

		ldx 	#0
		jsr 	EXPEvalInteger8 			; key number
		jsr 	ERRCheckComma
		inx
		jsr 	EXPEvalString 				; key text

		lda 	XSNumber0 					; set up to call fkey handler
		sta 	ControlParameters
		lda 	XSNumber0+1
		sta 	ControlParameters+2
		lda 	XSNumber1+1
		sta 	ControlParameters+3

		.DoSendMessage 						; send message 2,4 define fkey
		.byte 2,4
		.DoWaitMessage

		lda 	ControlError  				; check it worked.		
		bne 	_CFError
		rts
_CFError:
		.error_range		

_CFList:
		.DoSendMessage 						; send message 2,4 define fkey
		.byte 2,8
		.DoWaitMessage
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
;		07-02-24 		Added option to list with just FKEY
;
; ************************************************************************************************

