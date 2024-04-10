; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		sweet16.asm
;		Purpose:	Execute Sweet 16 code.
;		Created:	10th April 2024
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;										SWEET Command
;
; ************************************************************************************************

		.section code

Command_SWEET:	;; [sweet]
		ldx 	#0 							; register address.
		jsr 	EXPEvalInteger16
		lda 	XSNumber0 					; copy register address to XA
		ldx 	XSNumber1
		
_CSWExecute:	
		sta 	ControlParameters+0 		; set reg address parameters
		stx 	ControlParameters+1
		pha 								; save on stack
		phx
		DoSendMessage 						; execute that code.
		.byte 	1,9
		DoWaitMessage		

		lda 	ControlError 				; reentrancy check ?
		beq 	_CSWEnter

		plx  								; throw register address
		pla
		rts

_CSWEnter:
		.error_unimp

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

