; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		hit.asm
;		Purpose:	Sprite collision
;		Created:	9th January 2024
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;									Check sprite collision
;
; ************************************************************************************************

		.section code	

EXPUnaryHit: ;; [hit(]
		jsr 	EXPEvalInteger8 			; sprite 1
		pha
		jsr 	ERRCheckComma

		jsr 	EXPEvalInteger8 			; sprite 2
		pha
		jsr 	ERRCheckComma

		jsr 	EXPEvalInteger8 			; closing distance
		pha
		jsr 	ERRCheckRParen

		pla
		sta 	ControlParameters+2
		pla
		sta 	ControlParameters+1
		pla
		sta 	ControlParameters+0

		.DoSendMessage 						; collision test command.
		.byte 	6,4
		.DoWaitMessage

		lda 	ControlError 				; failed ?
		bne 	_EUHError

		lda 	ControlParameters+0 		; true/false option.
		jmp 	ReturnBoolean

_EUHError:
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

