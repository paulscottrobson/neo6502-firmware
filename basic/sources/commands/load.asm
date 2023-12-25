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
		ldx 	#0  						; file name
		jsr 	EXPEvalString
		;
		lda 	(codePtr),y 				; does a , follow
		cmp 	#KWD_COMMA
		beq		_CLLoadMemory 				; if so, load into memory.
		;
		stz 	ControlParameters+2 		; load into program space
		lda 	#Program >> 8
		sta 	ControlParameters+3
		jsr 	_CLLoad 					; Load BASIC program
		jsr 	ClearCode
		jmp 	WarmStart

_CLLoadMemory:
		iny 								; skip comma
		ldx 	#1 							; load here
		jsr 	EXPEvalInteger16
		lda 	XSNumber0+1 
		sta 	ControlParameters+2
		lda 	XSNumber1+1 
		sta 	ControlParameters+3
		jsr 	_CLLoad
		rts

;
;		Load named file (at stack 0) into address set at param2/3
;		
_CLLoad:
		lda 	XSNumber0  					; set file name in parameters
		sta 	ControlParameters+0
		lda 	XSNumber1
		sta 	ControlParameters+1

		DoSendMessage 						; do the load
		.byte 	3,2
		DoWaitMessage
		lda 	ControlError  				; error check
		beq 	_CLExit
		.error_file
_CLExit:
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

