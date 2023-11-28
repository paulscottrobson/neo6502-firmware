; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		term.asm
;		Purpose:	Evaluate a term
;		Created:	25th November 2023
;		Reviewed:	No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section 	code

; ************************************************************************************************
;
;								Evaluate a term onto the stack.
;
; ************************************************************************************************

EvaluateTerm:	
		inx 								; new slot on stack
		lda 	(CodePtr),y 				; get next token
		bmi 	_ETKeyword 					; is it a keyword (80-FF) ?
		asl		a							; is it a number (40-7F)
		bpl		_ETIdentifierOrSpecial 		; if 00-3F then it is an identifier, or a multi purpose unary ! ? -
		;
		;		40-7F which is an encoded integer constant.
		;
		jsr 	ExtractTokenisedInteger
		jsr 	CheckFollowingDecimal
		rts


_ETKeyword:		
		.byte 	3

_ETIdentifierOrSpecial:
		.byte 	3
		
; ************************************************************************************************
;
;				Found a integer token 40-7F, decode into current stack position.
;
; ************************************************************************************************

ExtractTokenisedInteger:
		lda 	(codePtr),y 				; get first token, known to be 40-7F
		and 	#$3F 						; mask off upper bits
		sta 	XSNumber0,x 				; initial value.
		stz 	XSNumber1,x
		stz 	XSNumber2,x
		stz 	XSNumber3,x
		stz 	XSControl,x 				; integer constant
_ETConstant:
		iny 								; look at next
		lda 	(CodePtr),y
		and 	#$C0 						; is it $40..$7F
		cmp 	#$40
		bne 	_ETExit

		lda 	XSNumber3,x 				; save the upper byte.
		pha

		lda 	XSNumber2,x 				; multiply old by 256
		sta 	XSNumber3,x
		lda 	XSNumber1,x
		sta 	XSNumber2,x
		lda 	XSNumber0,x 				
		sta 	XSNumber1,x
		stz 	XSNumber0,x

		pla 								; old MS Byte

		lsr 	a 							; shift right twice
		ror 	XSNumber3,x
		ror 	XSNumber2,x
		ror 	XSNumber1,x
		ror 	XSNumber0,x

		lsr 	a
		ror 	XSNumber3,x
		ror 	XSNumber2,x
		ror 	XSNumber1,x
		ror 	XSNumber0,x

		lda 	(codePtr),y 				; or the lower 6 bits in.
		and 	#$3F
		ora 	XSNumber0,x
		sta 	XSNumber0,x
		bra 	_ETConstant
_ETExit:
		rts

; ************************************************************************************************
;
;								Check following is decimal
;
; ************************************************************************************************

CheckFollowingDecimal:
		lda 	(CodePtr),y 				; what follows ?
		cmp 	#KWD_SYS_DEC 				; decimal ?
		bne 	_CFDExit
		;
		tya
		inc 	a
		sec
		adc 	CodePtr
		sta 	ControlPort+8 				; param slot 2 = address of decimal data.
		lda 	CodePtr+1
		adc 	#0
		sta 	ControlPort+9
		lda 	#32 						; decimal expand code.
		jsr 	DoMathCommand
_CFDExit:		
		rts

; ************************************************************************************************
;
;					Send command A to math co processor, X is stack position
;
; ************************************************************************************************

DoMathCommand:
		pha
		DoWaitMessage 						; wait till hardware free
		sta 	ControlPort+1

		txa
		clc 
		adc 	#XSStack & $FF
		sta 	ControlPort+4
		lda 	#XSStack >> 8
		adc 	#0
		sta 	ControlPort+5
		lda 	#8
		sta 	ControlPort+6
		lda 	#4
		sta 	ControlPort 				; do command
		DoWaitMessage 		
		pla				; wait for result.
		rts

		.send 		code

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

