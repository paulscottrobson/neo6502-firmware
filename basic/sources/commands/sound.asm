; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		sound.asm
;		Purpose:	Sound Command
;		Created:	10th January 2024
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;										SOUND Command
;
; ************************************************************************************************

		.section code

Command_SOUND:	;; [sound]
		lda 	#$FF  						; this is to detect SOUND CLEAR vs SOUND n CLEAR
		sta 	XSNumber0 		
		lda 	(codePtr),y 				; is first Shift then check for SOUND CLEAR
		cmp 	#KWD_SYS_SH1
		beq 	_CSCheckClear
		;
		ldx 	#0
		jsr 	EXPEvalInteger8 			; channel #

		lda 	(codePtr),y 				; check for SOUND n CLEAR
		cmp 	#KWD_SYS_SH1
		beq 	_CSCheckClear

		inx
		jsr 	ERRCheckComma 				; frequency
		jsr 	EXPEvalInteger16
		inx
		jsr 	ERRCheckComma  				; duration in ms.
		jsr 	EXPEvalInteger16
		;
		lda 	XSNumber0  					; set up to queue.
		sta 	ControlParameters
		lda 	XSNumber0+1
		sta 	ControlParameters+1
		lda 	XSNumber1+1
		sta 	ControlParameters+2
		lda 	XSNumber0+2
		sta 	ControlParameters+3
		lda 	XSNumber1+2
		sta 	ControlParameters+4
		stz 	ControlParameters+5
		.DoSendMessage 						; play that sound
		.byte 	8,4		
		.DoWaitMessage
_CSCheckError:
		lda 	ControlError
		bne 	_CSRange		
		rts
_CSRange:
		.error_range
_CSSyntax:
		.error_syntax				
		;		
		;		Check for CLEAR. 0 is $FF if just SPRITE CLEAR or n for SPRITE n CLEAR
		;
_CSCheckClear:
		iny 								; check CLEAR follows
		lda 	(codePtr),y
		iny
		cmp 	#(KWD_CLEAR & $FF)
		bne 	_CSSyntax
		;
		lda 	XSNumber0 					; was it SOUND CLEAR
		bmi 	_CSResetAll
		sta 	ControlParameters
		.DoSendMessage 						; stop that sound
		.byte 	8,2
		.DoWaitMessage
		bra 	_CSCheckError
_CSResetAll:
		.DoSendMessage 						; reset all sound
		.byte 	8,1
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

