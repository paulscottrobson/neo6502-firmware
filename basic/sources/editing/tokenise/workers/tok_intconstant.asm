; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		tok_intconstant.asm
;		Purpose:	Tokenise an integer constant
;		Created:	15th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section code


; ************************************************************************************************
;
;					Tokenise an integer constant. May be a line number if first thing.
;
; ************************************************************************************************

TokeniseIntConst:
		jsr 	TOKExtractDigits 			; extract an identifier into the tokElement Buffer
		lda 	#tokElement & $FF	 		; set up to convert it.
		sta 	ControlParameters+4
		lda 	#tokElement >> 8
		sta 	ControlParameters+5
		ldx 	#0 							; convert it.
		lda 	#33
		jsr 	DoMathCommand
		lda 	ControlError 				; error occurred
		bne 	_TICSyntax

		lda 	tokIsFirstElement 			; if first element, it's the line number
		bne 	_TICLineNumber
		jsr 	TOKTokeniseConstant			; write out in 6 bit constant mode.
		rts

_TICLineNumber:
		lda 	XSNumber2,x 				; check 0-65535
		ora		XSNumber3,x
		bne 	_TICSyntax
		;
		lda 	XSNumber0,x 				; copy number -> line # slot
		sta 	tokLineNumber
		lda 	XSNumber1,x
		sta 	tokLineNumber+1
		rts
_TICSyntax:
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

