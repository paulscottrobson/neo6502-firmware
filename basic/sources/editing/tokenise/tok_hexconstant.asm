; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		tok_hexconstant.asm
;		Purpose:	Tokenise a hexadecimal constant
;		Created:	15th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section code

; ************************************************************************************************
;
;							Tokenise an integer hexadecimal constant.
;
; ************************************************************************************************

TokeniseHexConst:
		jsr 	TOKGetNext 					; consume the '$'
		lda 	#KWD_DOLLAR 				; write out the '$' unary function.
		jsr 	TOKWriteA
		jsr 	TOKExtractIdentifier 		; extract an identifier into the tokElement Buffer
		lda 	tokElement 					; nothing at all !
		beq 	_THCSyntax

		stz 	XSNumber0 					; zero slot 0
		stz 	XSNumber1
		stz 	XSNumber2
		stz 	XSNumber3

		ldx 	#1 							; index into buffer
_THCLoop:
		ldy 	#4 							; shift slot 0 left x 4
_THCShift:
		asl 	XSNumber0
		rol 	XSNumber1		
		rol 	XSNumber2
		rol 	XSNumber3
		dey
		bne 	_THCShift
		;
		lda 	tokElement,x 				; next character
		jsr 	TOKIsHexadecimal 			; check hex
		bcc 	_THCSyntax 					; it isn't.
		cmp 	#"A" 						; convert to 0-15
		bcc 	_THCNotAlpha		
		sbc 	#7
_THCNotAlpha:	
		and 	#15
		ora 	XSNumber0 					; write in
		sta 	XSNumber0
		inx
		dec 	tokElement 					; do for all characters
		bne 	_THCLoop		
		;
		ldx 	#0 							; write out as a constant.
		jsr 	TOKTokeniseConstant			
		rts

_THCSyntax:
		.error_syntax

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

