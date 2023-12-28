; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		warmstart.asm
;		Purpose:	Warm Start code
;		Created:	17th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************
		
		.section code

; ************************************************************************************************
;
;											Warm Start
;
; ************************************************************************************************

WarmStart:
		lda 	#COL_GREEN+$80 				; set display colour.
		jsr 	WriteCharacter
		jsr 	InputLine 					; input string to buffer
		stz 	ControlStatus 				; clear break flag.
		jsr 	TOKTokenise 				; tokenise it.
		lda 	tokLineNumber 				; any line number ? if not, execute it.
		ora 	tokLineNumber+1
		beq 	_WSExecute
		jsr 	PGMDeleteLine 				; delete line specified
		jsr 	PGMInsertLine 				; insert it if not blank
		jsr 	ClearCode 					; clear program memory
		bra 	WarmStart 					; get another command

_WSExecute:
		ldx 	tokLineSize 				; put a NULL on the 'next line' to force END at end of line.
		stz 	tokLineSize,x
		;
		lda 	#tokLineSize >> 8 			; set up to 'run code'
		sta 	codePtr+1
		lda 	#tokLineSize & $FF
		sta 	codePtr

		jmp 	RUNNewLine 					; go to run it.

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

