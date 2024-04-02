; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		edit.asm
;		Purpose:	Editor
;		Created:	29th March 2024
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;											Run Editor
;
; ************************************************************************************************

		.section code

Command_Edit: ;; [edit]
		phy
		DoSendMessage 						; call initialise editor
		.byte 	13,1
		DoWaitMessage

_CELoop:		
		lda 	ControlParameters 			; was EXIT returned (code 0)
		beq 	_CEExit 					; if so, we are done.

		asl 	a 							; index into jump table.
		tax 								; get the function code.
		jsr  	_CECallback 				; call whatever we want.

		DoSendMessage 						; call next editor functionality
		.byte 	13,2
		DoWaitMessage
		bra 	_CELoop

_CEExit:
		ply
_CEReturn		
		rts
		
_CECallback:
		jmp 	(_CECallbackVectors,x)
_CECallbackVectors:		
		.word	_CEReturn 					; function 0 shouldn't be called
		.word 	EDInitialise 				; function 1 initialises.
		.word 	_CEReturn 					; function 2 get line.
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

