; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		gpio.asm
;		Purpose:	PIN command
;		Created:	20th February 2024
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;										PIN Command
;
; ************************************************************************************************

		.section code

Command_PIN:	;; [pin]

		ldx 	#0
		jsr 	EXPEvalInteger8 			; pin number
		pha		
		jsr 	ERRCheckComma

		lda 	(codePtr),y 				; pin INPUT check
		cmp 	#KWD_INPUT
		beq 	_CPInput
		cmp 	#KWD_SYS_SH1 				; pin OUTPUT check ?
		beq 	_CPCheckOutput
		;
		;		Set to value
		;
_CPValue:		
		jsr 	EXPEvalInteger 				; pin value
		lda 	XSNumber0,x
		ora 	XSNumber1,x
		ora 	XSNumber2,x
		ora 	XSNumber3,x
		sta 	ControlParameters+1
		pla
		sta 	ControlParameters

		.DoSendMessage 						; send message 10,2
		.byte 10,2
		.DoWaitMessage

_CPCheckResult:
		lda 	ControlError  				; check it worked.		
		bne 	_CPError
		rts
		;
		;		Set to output
		;
_CPCheckOutput:
		iny 								; what follows 	
		lda 	(codePtr),y
		dey
		cmp 	#KWD_OUTPUT-$100
		bne 	_CPValue 					; if not output then value
		lda 	#2							; set as output
		iny
		iny
		bra		_CPSetDirection
		;
		;		Set to input
		;
_CPInput:
		lda 	#1
		iny
_CPSetDirection:
		sta 	ControlParameters+1
		pla
		sta 	ControlParameters
		.DoSendMessage 						; send message 10,4 set direction
		.byte 10,4
		.DoWaitMessage
		bra 	_CPCheckResult 				; check what came back

_CPError:
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

