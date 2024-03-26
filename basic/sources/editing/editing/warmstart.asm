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

		ldy 	#inputBuffer >> 8 			; address of input buffer
		ldx 	#inputBuffer & $FF

		jsr 	ReadLine 					; read using screen editor

;		jsr 	InputLine 					; input string to buffer direct typing (ignores YX)

		lda 	inputBuffer 				; entered something ?
		beq 	_WSNotSerialLinkMos
		lda 	inputBuffer+1 				; if first character is / start the serial link.
		cmp 	#'/'
		beq 	_WSSerialLink		
		cmp 	#'*' 						; if first character is * MOS command
		beq 	_WSMosCommand

_WSNotSerialLinkMos:		
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

_WSMosCommand:
		lda 	#inputBuffer & $FF 			; address of command
		sta 	ControlParameters+0
		lda 	#inputBuffer >> 8
		sta 	ControlParameters+1
		.DoSendMessage 					 	; MOS Command
		.byte 	1,8
		.DoWaitMessage		
		jmp 	WarmStart

_WSSerialLink:
		.DoSendMessage 					 	; and reset it.
		.byte 	1,5
		.DoWaitMessage		
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

