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
;									   GLOAD/LOAD Commands
;
; ************************************************************************************************

		.section code

Command_Load:	;; [load]
		ldx 	#0  						; file name
		jsr 	EXPEvalString
		;
		lda 	(codePtr),y 				; does a , follow
		cmp 	#KWD_COMMA
		beq		CLLoadMemory 				; if so, load into memory.
		;
		stz 	ControlParameters+2 		; load into program space
		lda 	#Program >> 8
		sta 	ControlParameters+3
		jsr 	CLLoad 						; Load BASIC program
		jsr 	ClearCode
		jmp 	WarmStart

Command_GLoad: ;; [gload]
		ldx 	#0  						; file name
		jsr 	EXPEvalString
		lda 	#$FF
		sta 	ControlParameters+2
		sta 	ControlParameters+3
		jsr 	CLLoad
		rts

CLLoadMemory:
		iny 								; skip comma
		ldx 	#1 							; load here
		jsr 	EXPEvalInteger16
		lda 	XSNumber0+1 
		sta 	ControlParameters+2
		lda 	XSNumber1+1 
		sta 	ControlParameters+3
		jsr 	CLLoad
		rts

;
;		Load named file (at stack 0) into address set at param2/3
;		
CLLoad:
		lda 	XSNumber0  					; set file name in parameters
		sta 	ControlParameters+0
		lda 	XSNumber1
		sta 	ControlParameters+1

		jsr 	LoadExtended 				; call the extended load code
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

