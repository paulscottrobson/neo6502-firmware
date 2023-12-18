; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		cat.asm
;		Purpose:	Display directory
;		Created:	18th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;										CAT Command
;
; ************************************************************************************************

		.section code

Command_CAT:	;; [cat]
		DoSendMessage
		.byte 	3,1
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
