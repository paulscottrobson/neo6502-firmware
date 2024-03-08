; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		save.asm
;		Purpose:	Save file
;		Created:	18th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;										SAVE Command
;
; ************************************************************************************************

		.section code

Command_Save:	;; [save]
		ldx 	#0  						; file name
		jsr 	EXPEvalString
		lda 	(codePtr),y 				; , found
		cmp 	#KWD_COMMA
		beq 	_CSSaveRange
		;
		stz 	ControlParameters+2 		; start of program
		lda 	#Program >> 8
		sta 	ControlParameters+3
		;
		jsr 	ClearResetFreeMemory 		; get end of program
		sec  								; work out length as end-start+1
		lda 	zTemp0
		sbc 	ControlParameters+2
		sta 	ControlParameters+4
		lda 	zTemp0+1
		sbc 	ControlParameters+3
		sta 	ControlParameters+5
		inc 	ControlParameters+4
		bne 	_CSNoCarry
		inc 	ControlParameters+5
_CSNoCarry:		
		jsr 	_CSSave 					; and do the actual write
		rts
;
;		Save "<name>",<start>,<size>
;
_CSSaveRange:
		inx 								; get from
		jsr 	ERRCheckComma
		jsr 	EXPEvalInteger16
		inx 								; get size
		jsr 	ERRCheckComma
		jsr 	EXPEvalInteger16
		;
		clc 								; add from to size, error on overflow
		lda 	XSNumber0+1 				; copy from at the same time.
		sta 	ControlParameters+2
		adc 	XSNumber0+2
		lda 	XSNumber1+1
		sta 	ControlParameters+3
		adc 	XSNumber1+2
		bcs 	_CSError
		;
		lda 	XSNumber0+2 				; copy size
		sta 	ControlParameters+4
		lda 	XSNumber1+2
		sta 	ControlParameters+5
		ora 	XSNumber0+2 				; check size = 0
		beq 	_CSError
		jsr 	_CSSave
		rts
;
;		Copy name paramters and do save.
;
_CSSave:
		lda 	XSNumber0
		sta 	ControlParameters+0
		lda 	XSNumber1
		sta 	ControlParameters+1
		DoSendMessage
		.byte 	3,3
		DoWaitMessage
		lda 	ControlError
		beq 	_CSExit
_CSError:		
		.error_file
_CSExit		
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

