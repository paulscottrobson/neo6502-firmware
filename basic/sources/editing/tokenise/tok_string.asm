; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		tok_string.asm
;		Purpose:	Tokenise a string constant
;		Created:	15th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section code

; ************************************************************************************************
;
;								Tokenise a string constant.
;
; ************************************************************************************************

TokeniseString:
		jsr 	TOKGetNext 					; consume the '"'
		jsr 	TOKCreateString 
		rts

; ************************************************************************************************
;
;		Create a string structure, from the rest of the line to EOL or " (also used by comment)
;
; ************************************************************************************************

TOKCreateString:
		lda 	#KWD_SYS_STR 				; write out the string unary function.
		jsr 	TOKWriteA

		lda 	tokLineSize 				; save the position of the length so we can update it.
		sta 	tokLengthPos
		lda 	#0 							; write initial length zero.
		jsr 	TOKWriteA
_TCSLoop:
		jsr 	TOKGetNext 					; get and consume next.
		beq 	_TCSEndString 				; end of line
		cmp 	#'"' 						; closing quote mark
		beq 	_TCSEndString
		;
		jsr 	TOKWriteA 					; output it
		ldx 	tokLengthPos 				; increment the length
		inc 	tokLineSize,x
		bra 	_TCSLoop
_TCSEndString:
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

