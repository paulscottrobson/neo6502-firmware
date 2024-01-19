; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		palette.asm
;		Purpose:	Set colour palette
;		Created:	31st December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;										PALETTE Command
;
; ************************************************************************************************

		.section code

Command_Palette:	;; [palette]
		lda 	(codePtr),y 				; is it PALETTE CLEAR
		cmp 	#KWD_SYS_SH1
		beq 	_CPCheckClear

		ldx 	#0
		jsr 	EXPEvalInteger8			
		pha
		jsr 	ERRCheckComma
		jsr 	EXPEvalInteger8			
		pha
		jsr 	ERRCheckComma
		jsr 	EXPEvalInteger8			
		pha
		jsr 	ERRCheckComma
		jsr 	EXPEvalInteger8			
		sta 	ControlParameters+3
		pla
		sta 	ControlParameters+2
		pla
		sta 	ControlParameters+1
		pla
		sta 	ControlParameters+0
		.DoSendMessage
		.byte 	5,32
		.DoWaitMessage
		rts

_CPCheckClear:
		iny
		lda 	(codePtr),y
		iny
		cmp 	#KWD_CLEAR-$100
		beq 	_CPResetPalette
		.error_syntax
		
_CPResetPalette:		
		.DoSendMessage
		.byte 	5,34
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

