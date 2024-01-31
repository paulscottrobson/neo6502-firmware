; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		cursor.asm
;		Purpose:	Set cursor position
;		Created:	31st January 2024
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;										CURSOR Command
;
; ************************************************************************************************

		.section code

Command_Cursor:	;; [cursor]
		ldx 	#0
		jsr 	EXPEvalInteger8			
		pha
		jsr 	ERRCheckComma
		jsr 	EXPEvalInteger8			
		sta 	ControlParameters+1
		pla
		sta 	ControlParameters+0
		.DoSendMessage
		.byte 	2,7
		.DoWaitMessage
		lda 	ControlError
		bne 	_CCRange
		rts
_CCRange:
		.error_range		


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

