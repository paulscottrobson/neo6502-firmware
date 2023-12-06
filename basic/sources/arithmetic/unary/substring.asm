; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		substring.asm
;		Purpose:	Left$/Mid$/Right$ - put here as Left/Right are same code.
;		Created:	6th December 2023
;		Reviewed: 	No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section code

; ************************************************************************************************
;
;								Left$(<string>,<characters>)
;
; ************************************************************************************************

Unary_Left: 	;; [left$(]
		jsr 	SubstringInitial 			; set up.
		lda 	XSNumber0+1,x 				; Param #1 is the length
		sta 	XSNumber0+2,x 				
		stz 	XSNumber0+1,x 				; Start is zero.
		bra 	SubstringMain 				

; ************************************************************************************************
;
;								Left$(<string>,<characters>)
;
; ************************************************************************************************

Unary_Right: 	;; [right$(]
		jsr 	SubstringInitial 			; set up.
		lda 	XSNumber0+1,x 				; Param #1 is the length
		sta 	XSNumber0+2,x 				

		sec
		lda 	XSNumber3,x 				; total length
		sbc 	XSNumber0+1,x 				; length - required count.
		bcs 	_URNotUnderflow		
		lda 	#0 							; start from the beginning, as something like right$("AB",3)
_URNotUnderflow:
		sta 	XSNumber0+1,x 				; this is the start position		
		bra 	SubstringMain

; ************************************************************************************************
;
;							    Mid$(<string>,<start>[,<characters>])
;
; ************************************************************************************************

Unary_Mid: 	;; [mid$(]
		jsr 	SubstringInitial 			; set up.
		;
		lda 	XSNumber0+1,x 				; first parameter is zero ?
		beq 	_UMError
		dec 	XSNumber0+1,x				; reduce initial offset by 1 as MID$(a$,1..) is actually the first character
		lda 	#$FF 						; default length.
		sta 	XSNumber0+2,x
		lda 	(CodePtr),y 				; followed by a comma
		cmp 	#KWD_COMMA 					; no, done.
		bne 	_UMComplete
		iny 								; consume comma
		phx
		inx 								; get length of the substring into +2
		inx
		jsr 	EXPEvalInteger8		
		plx
_UMComplete:
		bra 	SubstringMain
_UMError:		
		.error_argument

; ************************************************************************************************
;
;							X 0,1,2 is string, offset, length
;
; ************************************************************************************************

SubstringMain:		
		jsr 	ERRCheckRParen 				; check closing bracket
		lda 	XSNumber0+1,x 				; is the initial offset >= the length	
		cmp 	XSNumber3,x 	
		bcs 	_SSMNull 					; if so, return an empty string.
		;
		sec
		lda 	XSNumber3,x 				; max # to copy = length - start
		sbc 	XSNumber0+1,x
		sta 	zTemp0
		;
		lda 	XSNumber0+2,x 				; max the count to copy to this.
		cmp 	zTemp0
		bcc 	_SSNotAtLimit
		lda 	zTemp0
_SSNotAtLimit:								
		sta 	XSNumber0+2,x 				; number to copy.
		cmp 	#0
		beq 	_SSMNull 					; return empty string if copy zero.

		lda 	XSNumber0,x 				; save string address on stack.
		pha
		lda 	XSNumber1,x
		pha

		lda 	XSNumber0+2,x 				; allocate string memory for the result.
		jsr 	StringTempAllocate

		pla 								; string address in zTemp0.
		sta 	zTemp0+1
		pla
		sta 	zTemp0

		phy 
		lda 	XSNumber0+1,x 				; set up first character to copy
		tay
_SSCopySubString:
		iny 								; pre-increment because of the length byte.				
		lda 	(zTemp0),y
		jsr 	StringTempWrite 			; write character out
		dec 	XSNumber0+2,x 				; do correct # of characters
		bne 	_SSCopySubString
		ply

		rts				

_SSMNull:
		lda 	#0
		jsr 	StringTempAllocate		
		rts

; ************************************************************************************************
;
;		Common initial code. Read 1st String, 2nd byte and if CS set, 3rd byte on to stack
;		levels X,X+1 and X+2
;
; ************************************************************************************************

SubstringInitial:
		jsr 	EXPEvalString 				; get the string
		lda 	(zTemp0) 					; length of string
		sta 	XSNumber3,x 				; save here as a temporary, need it later.

		inx
		jsr 	ERRCheckComma 				; comma next
		jsr 	EXPEvalInteger8 			; get next parameter
		dex
		rts 								; exit

		.send 	code
		
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
