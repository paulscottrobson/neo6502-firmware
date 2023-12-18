; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		load.asm
;		Purpose:	Load file
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

Command_Load:	;; [load]
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
		DoSendMessage
		.byte 	3,2
		DoWaitMessage
		lda 	ControlError
		beq 	_CLExit
		.error_file
_CLExit:
		jsr 	ClearCode
		jmp 	WarmStart


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
