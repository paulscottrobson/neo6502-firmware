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
;										CLS Command
;
; ************************************************************************************************

		.section code

Command_Palette:	;; [palette]
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

