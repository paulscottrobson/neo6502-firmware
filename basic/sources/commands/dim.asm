; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		dim.asm
;		Purpose:	DIM command
;		Created:	11th December 2023
;		Reviewed: 	None
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section code

DimCommand: ;; [dim]
		ldx 	#0 							; get the term, e.g. the variable to dimension
		jsr 	EvaluateTerm
		lda 	XSControl+0 				; check it is a reference.
		and 	#XS_ISVARIABLE
		beq 	_DCSyntax
		inx
		jsr 	EXPEvalInteger8 			; get the first dimension 0-255
		lda 	XSNumber0+1 				; max 254 dimensions
		cmp 	#255
		beq 	_DCRange
		stz 	XSNumber0+2 				; zero the second dimension.
		lda 	(codePtr),y 				; followed by comma
		cmp 	#KWD_COMMA 	 				; no, size is one dimension
		bne 	_DCHaveSize
		inx
		iny 								; consume comma.
		jsr 	EXPEvalInteger8 			; second dimension
		lda 	XSNumber0+2 				; max 254 dimensions
		cmp 	#255
		beq 	_DCRange		
_DCHaveSize:
		jsr 	ERRCheckRParen 				; closing )
		jsr 	DimCreateArray 				; create an array
		lda 	(codePtr),y 				; multiple dims
		iny
		cmp 	#KWD_COMMA
		beq 	DimCommand
		dey
		rts

_DCSyntax:
		.error_syntax		
_DCRange:
		.error_range

; ************************************************************************************************
;
;							Create an array according to slots 0,1,2
;
; ************************************************************************************************

DimCreateArray:
		.byte 	3
		;		Work out total size
		; 		multiply x+1.y+1 if 2 dims
		; 		multiply by 5
		; 		allocate memory
		; 		store memory pointers and dimensions
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
