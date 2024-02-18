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

		ldy 	#3 								; if $FFxxxxxx then always return zero, halted.
		lda 	(zTemp0),y
		cmp 	#$FF
		bmi 	_EUEFrozen

		lda 	(zTemp0)	 					; reset timer if zero
		ldy 	#1 								
		ora 	(zTemp0),y 						
		iny 	
		ora 	(zTemp0),y
		iny 	
		ora 	(zTemp0),y
		beq 	_EUEInitialise

		ldy 	#1  							; calc timer() - value in variable
		lda 	ControlParameters+0
		cmp 	(zTemp0)
		lda 	ControlParameters+1
		sbc 	(zTemp0),y
		sta 	zTemp1
		iny
		lda 	ControlParameters+2
		sbc 	(zTemp0),y
		sta 	zTemp1+1
		iny
		lda 	ControlParameters+3
		sbc 	(zTemp0),y
		bcs 	_EUETrigger 					; if timer() >= value then fire the event.
		ply 									; restore Y
_EUEFrozen:		
		lda 	#0 								; and return 0.
		jsr 	EXPUnaryReturnA
		rts

_EUEType:
		.error_type

_EUEInitialise:	 								; initialise if zero.
		ldy 	#3
_EUECopy1:		
		lda 	ControlParameters,y
		sta 	(zTemp0),y
		dey 	
		bpl 	_EUECopy1
_EUETrigger:
		clc 									; add timer rate to time variable to give next fire time.
		lda 	(zTemp0)
		adc 	XSNumber0+1,x
		sta 	(zTemp0)
		ldy 	#1
		lda 	(zTemp0),y
		adc 	XSNumber1+1,x
		sta 	(zTemp0),y

		ldy 	#2
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

		.send code


; ************************************************************************************************
;
;									Changes and Updates
;
; ************************************************************************************************
;
;		Date			Notes
;		==== 			=====
;		17-01-24 		Made event additive and reset on zero.
;		18-01-24 		Fixed stupid event() bug ; not initialising properly when zero.
;
; ************************************************************************************************

