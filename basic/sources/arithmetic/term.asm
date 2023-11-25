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
		.byte 	3
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

