; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		save.asm
;		Purpose:	Save file
;		Created:	18th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;										SAVE Command
;
; ************************************************************************************************

		.section code

Command_Save:	;; [save]
		ldx 	#0
		jsr 	EXPEvalString
		lda 	XSNumber0
		sta 	ControlParameters+0
		lda 	XSNumber1
		sta 	ControlParameters+1
		;
		stz 	ControlParameters+2
		lda 	#Program >> 8
		sta 	ControlParameters+3
		;
		jsr 	ClearResetFreeMemory
		sec
		lda 	zTemp0
		sbc 	ControlParameters+2
		sta 	ControlParameters+4
		lda 	zTemp0+1
		sbc 	ControlParameters+3
		sta 	ControlParameters+5
		inc 	ControlParameters+4
		bne 	_CSNoCarry
		inc 	ControlParameters+5
_CSNoCarry:		
		DoSendMessage
		.byte 	3,3
		DoWaitMessage
		lda 	ControlError
		beq 	_CSExit
		.error_file
_CSExit		
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

