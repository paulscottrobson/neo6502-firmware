; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		mouse.asm
;		Purpose:	Control Mouse 
;		Created:	9th April 2024
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;										MOUSE Command
;
; ************************************************************************************************

		.section code

Command_Mouse:	;; [mouse]
		lda 	(codePtr),y 				; check for TO.
		cmp 	#KWD_TO
		beq 	_MOTo
		;
		;		MOUSE SHOW|HIDE
		;
		cmp 	#KWD_SYS_SH1 				; check for CURSOR SHOW and HIDE, shifted
		bne 	_MOError
		iny
		lda 	(codePtr),y 				; check SHOW/HIDE
		iny
		cmp 	#KWD_SHOW-$100
		beq		_MOControl
		cmp 	#KWD_HIDE-$100
		beq		_MOControl
		cmp 	#KWD_CURSOR-$100  			; check cursor
		beq 	_MOCursor
_MOError:
		.error_syntax		 	

_MOCursor:
		ldx 	#0 							; get cursor number
		jsr 	EXPEvalInteger8
		sta 	ControlParameters+0
		.DoSendMessage  					; set mouse cursor
		.byte 	11,5
		.DoWaitMessage
		lda 	ControlError
		bne 	_MORange
		rts
_MORange:
		.error_range		

_MOControl:
		eor 	#KWD_HIDE-$100 				; 0 HIDE <>0 SHOW
		sta 	ControlParameters+0
		.DoSendMessage  					; set mouse visibility
		.byte 	11,2
		.DoWaitMessage
		rts

		;
		;		MOUSE TO x,y
		;
_MOTo:		
		iny  								; get coordinates
		ldx 	#0
		jsr 	EXPEvalInteger16
		inx
		jsr 	ERRCheckComma
		jsr 	EXPEvalInteger16

		lda 	XSNumber0  					; set up for call.
		sta 	ControlParameters+0
		lda 	XSNumber1
		sta 	ControlParameters+1
		lda 	XSNumber0+1
		sta 	ControlParameters+2
		lda 	XSNumber1+2
		sta 	ControlParameters+3

		.DoSendMessage  					; set mouse position.
		.byte 	11,1
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

