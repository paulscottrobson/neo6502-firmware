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
;							Evaluate a term onto the stack, current level.
;
; ************************************************************************************************

EvaluateTerm:	
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
		;
		;		Unary function.
		;
_ETKeyword:		
		cmp 	#$B0 						; is it a unary function ?
		bcs 	_ETSyntax
		phx
		asl 	a 							; access address to jump
		tax
		lda 	StandardVectorTable,x
		sta 	_ETVector+1
		lda 	StandardVectorTable+1,x
		sta 	_ETVector+2
		plx 								; restore stack pos and go there.
		iny 								; consume
_ETVector:
		jmp 	$FFFF		
		;
		;		Here token was 00-3F. A contains token x 2.
		;		
_ETIdentifierOrSpecial:
	 	asl 	a 							; bit 5 now in sign slot
	 	bpl 	_ETIsIdentifier  			; 0 it is 00-1F e.g. identifier
	 	;
	 	;		Here it's a special case of -, a binary token with unary functionality
	 	;
	 	lda 	(codePtr),y 				; get and consume. clearer than reuse
	 	iny
	 	cmp 	#KWD_MINUS 					; negation.
	 	bne 	_ETSyntax
	 	;
	 	jsr 	EvaluateTerm 				; it's minus *something*.
	 	jsr 	DereferenceTOS 				; dereference it.
	 	lda 	XSControl,x 				; numeric term ?
	 	bmi 	_ETBadType
	 	lda 	#16 						; negation function - needs optimising for ints 
	 	jsr 	DoMathCommand 				; work it out
	 	rts 	
_ETSyntax:		
		.error_syntax
_ETBadType:
		.error_type		
		;
		;		Identifier found.
		;
_ETIsIdentifier:
		lda 	(codePtr),y 				; get the MSB of the identifier address.
		iny
		clc
		adc 	#Program >> 8 				; convert to a real address.
		sta 	XSNumber1,x
		sta 	zTemp0+1
		lda 	(codePtr),y 				; LSB - on a page boundary
		iny
		sta 	XSNumber0,x
		sta 	zTemp0
		stz 	XSNumber2,x 				; neatness
		stz 	XSNumber3,x
		;
		phy 								; read and update the type/control
		ldy 	#4
		lda 	(zTemp0),y
		and 	#XS_TYPEMASK 				; type info
		ora 	#XS_ISVARIABLE 				; set variable reference bit (is word)
		ply
		sta 	XSControl,x
		rts

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
		tya 								; get address of decimal data
		inc 	a
		sec
		adc 	CodePtr
		sta 	ControlPort+8 				; param slot 2 = address of decimal data.
		lda 	CodePtr+1
		adc 	#0
		sta 	ControlPort+9
		lda 	#32 						; decimal expand code.
		jsr 	DoMathCommand
		;
		iny 								; get length and add it +1 for the length
		tya
		sec
		adc 	(CodePtr),y 				
		tay
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
		sta 	ControlFunction

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
		sta 	ControlCommand 				; do command
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

