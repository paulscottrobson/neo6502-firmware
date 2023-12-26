; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		event.asm
;		Purpose:	event function
;		Created:	26th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;								Event(<integer variable>,<rate>)
;
; ************************************************************************************************

		.section code	

EXPUnaryEvent: ;; [event(]
		jsr 	EvaluateTerm 					; term into current slot
		;
		lda 	XSControl,x 					; check type.
		cmp 	#XS_ISVARIABLE 					; integer reference ?
		bne 	_EUEType
		jsr 	ERRCheckComma
		inx
		jsr 	EXPEvalInteger16 				; get timer rate in +1
		dex
		jsr 	ERRCheckRParen 					; )
		;
		phy 									; save Y

		.DoSendMessage 							; get time.
		.byte 	1,1
		.DoWaitMessage

		lda 	XSNumber0,x 					; copy address of variable to zTemp0
		sta 	zTemp0
		lda 	XSNumber1,x
		sta 	zTemp0+1

		ldy 	#1  							; calc timer() - value in variable
		lda 	ControlParameters+0
		cmp 	(zTemp0)
		lda 	ControlParameters+1
		sbc 	(zTemp0),y
		iny
		lda 	ControlParameters+2
		sbc 	(zTemp0),y
		iny
		lda 	ControlParameters+3
		sbc 	(zTemp0),y
		bcs 	_EUETrigger 					; if timer() >= value then fire the event.
		ply 									; restore Y
		lda 	#0 								; and return 0.
		jsr 	EXPUnaryReturnA
		rts

_EUETrigger:
		clc 									; add timer rate to time() to give next fire time.
		lda 	ControlParameters+0
		adc 	XSNumber0+1,x
		sta 	(zTemp0)
		ldy 	#1
		lda 	ControlParameters+1
		adc 	XSNumber1+1,x
		sta 	(zTemp0),y
		iny
		lda 	(zTemp0),y
		adc 	#0
		sta 	(zTemp0),y
		iny
		lda 	(zTemp0),y
		adc 	#0
		sta 	(zTemp0),y

		ply 									; restore Y and return -1
		lda 	#$FF
		jsr 	EXPUnaryReturnA
		rts
_EUEType:
		.error_type

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

