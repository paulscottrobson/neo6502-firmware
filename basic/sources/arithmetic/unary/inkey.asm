; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		inkey.asm
;		Purpose:	Get keystroke if any.
;		Created:	12th December 2003
;		Reviewed: 	No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section code

; ************************************************************************************************
;
; 										INKEY$() function
;
; ************************************************************************************************

UnaryInkey: ;; [inkey$(]	
		jsr 	ERRCheckRParen 				; no parameter
		lda 	#1 							; allocate space for one char
		jsr 	StringTempAllocate

		.DoSendMessage 						; send command 2,1 read keyboard
		.byte 	2,1
		.DoWaitMessage
		lda 	ControlParameters+0
		beq 	_UIExit 					; "" if empty
		jsr 	StringTempWrite
_UIExit:		
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
