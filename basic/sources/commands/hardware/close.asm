; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		close.asm
;		Purpose:	Close file / all files
;		Created:	20th June 2024
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;										CLOSE Command
;
; ************************************************************************************************

		.section code

Command_Close:	;; [close]
		lda 	(codePtr),y 				; is there a channel
		cmp 	#KWD_COLON 					; no, close all files.
		beq 	CloseAllOpenFiles
		cmp 	#KWD_SYS_END
		beq 	CloseAllOpenFiles

		ldx 	#0
		jsr 	EXPEvalInteger8 			; channel #

CloseFileA:
		sta 	ControlParameters+0 		; handle to clsoe
		.DoSendMessage 						; close files
		.byte 	3,5
		rts

CloseAllOpenFiles:
		lda 	#$FF 						; this closes all files.
		bra 	CloseFileA

		.send code

; ************************************************************************************************
;
;									Changes and Updates
;
; ************************************************************************************************
;
;		Date			Notes
;		==== 			=====
; 		21-01-24 		SFX always plays immediately.
;
; ************************************************************************************************

