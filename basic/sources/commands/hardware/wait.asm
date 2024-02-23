; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		wait.asm
;		Purpose:	Wait for n centiseconds
;		Created:	23rd February 2024
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;										WAIT Command
;
; ************************************************************************************************

		.section code

Command_Wait:	;; [wait]
		ldx 	#0
		jsr 	EXPEvalInteger16 			; count of centiseconds (max 653 seconds)
		jsr 	_CWGetTimer 				; read timer
		;
		clc
		lda 	XSNumber0 					; work out end time.
		adc 	ControlParameters+0
		sta 	zTemp0
		lda 	XSNumber1
		adc 	ControlParameters+1
		sta 	zTemp0+1
_CWLoop:
		jsr 	_CWGetTimer 				; read timer
		lda 	ControlParameters+0 		; if < keep trying
		cmp 	zTemp0
		lda 	ControlParameters+1
		sbc 	zTemp0+1
		bmi 	_CWLoop
		rts

_CWGetTimer:
		.DoSendMessage 						; get time to CP[0],CP[1]
		.byte 	1,1
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
;
; ************************************************************************************************

