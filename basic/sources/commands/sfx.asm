; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		sfx.asm
;		Purpose:	Sound Effect Command
;		Created:	10th January 2024
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;										SFX Command
;
; ************************************************************************************************

		.section code

Command_SFX:	;; [sfx]
		lda 	(codePtr),y 				; push first on stack
		pha
		cmp 	#KWD_SYS_SH1
		bne 	_CSNotClear
		iny  								; check CLEAR
		lda 	(codePtr),y
		iny
		cmp 	#KWD_CLEAR & $FF
		bne 	_CSSyntax
		pla 								; replace with $FF
		lda 	#$FF
		pha
_CSNotClear:		
		ldx 	#0
		jsr 	EXPEvalInteger8 			; channel #
		jsr 	ERRCheckComma
		inx
		jsr 	EXPEvalInteger8 			; f/x number

		pla 	 							; check CLEAR prefix.
		cmp 	#$FF
		bne 	_CSNotClear2
		lda 	XSNumber0
		sta 	ControlParameters+0
		.DoSendMessage 						; clear channel
		.byte 	8,2

_CSNotClear2:
		lda 	XSNumber0+1
		sta 	ControlParameters+1
		lda 	XSNumber0
		sta 	ControlParameters+0
		.DoSendMessage 						; play sound
		.byte 	8,5
		rts		

_CSSyntax:
		.error_syntax		
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

