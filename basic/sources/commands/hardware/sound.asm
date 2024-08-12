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

Command_NOISE:  ;; [noise]
		lda 	#1
		sta 	soundType
		bra 	SoundCommon
		
Command_SOUND:	;; [sound]
		stz 	soundType

SoundCommon:		
		lda 	#$FF  						; this is to detect SOUND CLEAR vs SOUND n CLEAR
		sta 	XSNumber0 		
		lda 	(codePtr),y 				; is first Shift then check for SOUND CLEAR
		cmp 	#KWD_SYS_SH1
		beq 	_CSCheckClear
		;
		ldx 	#0
		stx 	XSNumber0+3  				; default slide.
		stx 	XSNumber1+3
		jsr 	EXPEvalInteger8 			; channel #

		lda 	(codePtr),y 				; check for SOUND n CLEAR
		cmp 	#KWD_SYS_SH1
		beq 	_CSCheckClear
		jsr 	ERRCheckComma 				

_CSMakeSound:		
		inx 								; frequency
		jsr 	EXPEvalInteger16
		inx
		jsr 	ERRCheckComma  				; duration in ms.
		jsr 	EXPEvalInteger16
		;
		lda 	(codePtr),y 				; check for slide.
		cmp 	#KWD_COMMA
		bne 	_CSNoSlide
		;
		iny
		inx
		jsr 	EXPEvalInteger

_CSNoSlide:		
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
		lda 	XSNumber0+3
		sta 	ControlParameters+5
		lda 	XSNumber1+3
		sta 	ControlParameters+6
		lda 	soundType
		sta 	ControlParameters+7
		lda 	#100
		sta 	ControlParameters+8

		.DoSendMessage 						; play that sound (extended)
		.byte 	8,7		
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
		;		Check for CLEAR. 0 is $FF if just SOUND CLEAR or n for SOUND n CLEAR
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
		;
		;		Now check if a sound follows.
		;
		lda 	(codePtr),y 				; EOL or :
		cmp 	#KWD_SYS_END
		beq 	_CSExit
		cmp 	#KWD_COLON
		bne 	_CSMakeSound		
_CSExit:		
		rts

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
;		15/01/24 		Added optional fourth parameter for sliding.
; 		21/01/24 		Allowed SOUND 0 CLEAR 1,440,1 type syntax.
;
; ************************************************************************************************

	