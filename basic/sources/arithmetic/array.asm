; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		array.asm
;		Purpose:	Convert an array term to an array element
;		Created:	13th December 2023
;		Reviewed:	No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section 	code

; ************************************************************************************************
;
;					Stack,X is the array term. Make it an array reference.
;
;					   On exit, both XSNumber0 should point to the term
;
; ************************************************************************************************

ArrayAccess:
		inx
		;
		;		Evaluate the first/second dimensions of the array
		;
		jsr 	EXPEvalInteger8 			; get the first dimension 0-255
		stz 	XSNumber0+1,x 				; the second is zero by default.
		lda 	(codePtr),y 				; 2nd element ?
		cmp 	#KWD_COMMA 		
		bne 	_AAEndEval
		iny 								; consume comma
		inx 								; get second element.
		jsr 	EXPEvalInteger8
		dex
_AAEndEval:
		dex 								; now points to term
		jsr 	ERRCheckRParen 				; check closing )
		phy
		;
		;		Validate the array and those dimensions.
		;
		lda 	XSNumber0,x 				; copy array descriptor address to zTemp0
		sta 	zTemp0
		lda 	XSNumber1,x 				
		sta 	zTemp0+1
		;
		ldy 	#1  						; check memory allocated
		lda 	(zTemp0)
		ora 	(zTemp0),y
		beq 	_AANoArray
		;
		lda 	(zTemp0) 					; copy data address into XSNumber,x
		sta 	XSNumber0,x
		ldy 	#1
		lda 	(zTemp0),y
		sta 	XSNumber1,x
		;
		ldy 	#2
		lda 	(zTemp0),y 					; compare max dimension to request
		cmp 	XSNumber0+1,x
		bcc 	_AARange
		iny 								; same for second dimension, if zero doesn't matter.
		lda 	(zTemp0),y
		cmp 	XSNumber0+2,x
		bcc 	_AARange		
		;
		ldy 	#4 							; save type.
		lda 	(zTemp0),y
		pha
		;
		tay 								; put second max dimension in Y
		lda 	XSNumber0+1,x 				
		cpy 	#0 							; if Y = 0 e.g. no second dimension we have YA as index
		beq 	_AAHaveIndex
		ldy 	#2 							; get first dim size + 1 => Y
		lda 	(zTemp0),y
		tay
		iny
		lda 	XSNumber0+2,x 				; multiply by the second value.
		jsr 	MultiplyYA 					
		clc 								; add the first index.
		adc 	XSNumber0+1,x 		
		bcc 	_AAHaveIndex
		iny
_AAHaveIndex:		
		jsr 	MultiplyYABy5 				; YA is now an offset into the array.		

		clc 								; add offset to XSNumber01/x
		adc 	XSNumber0,x
		sta 	XSNumber0,x
		sta 	zTemp0
		tya
		adc 	XSNumber1,x
		sta 	XSNumber1,x
		sta 	zTemp0+1

		pla 								; get array type
		bpl 	_AANotString 				; if string, set the type bit in the element.
		ldy 	#4
		lda 	(zTemp0),y
		ora 	#XS_ISSTRING
		sta 	(zTemp0),y
_AANotString:		
		ply
		rts

_AARange:
		.error_range
_AANoArray:
		.error_array

		.send 		code

; ************************************************************************************************
;
;									Changes and Updates
;
; ************************************************************************************************
;
;		Date			Notes
;		==== 			=====
; 		07-02-24 		Check to see array initialised.
;
; ************************************************************************************************

