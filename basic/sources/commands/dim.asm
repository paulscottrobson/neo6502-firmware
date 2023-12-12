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
		phx
		phy
		lda 	XSNumber0+1 				; first dimension
		inc 	a 							; need one more entry than the maximum dimension
		ldy 	XSNumber0+2 				; second dimension, zero if only one.
		beq 	_DCASingle 					
		iny 								; one more than maximum dimension
		jsr 	MultiplyYA 					; YA = Y x A
_DCASingle:
		jsr 	MultiplyYABy5 				; Multiply YA (the number of elements) by 5. 		
		cpy 	#51 						; always going to be too big (256/5)
		bcs 	_DCAError
		jsr 	MemoryAllocateYA 			; allocate memory for it.
		;
		ldx 	XSNumber0 					; copy target address in variable table to zTemp0
		stx 	zTemp0
		ldx 	XSNumber1
		stx 	zTemp0+1
		;
		sta 	(zTemp0) 					; copy out the address of the array memory block.
		tya
		ldy 	#1
		sta 	(zTemp0),y
		;
		iny 								; copy out the two dimensions, if one dimension 
		lda 	XSNumber0+1 				; the second value is zero.
		sta 	(zTemp0),y
		iny
		lda 	XSNumber0+2
		sta 	(zTemp0),y
		;
		ply
		plx
		rts
_DCAError:
		.error_range

; ************************************************************************************************
;
;									Multiply Y by A => YA
;
; ************************************************************************************************

MultiplyYA:
		sta 	zTemp0 						; zTemp0 : adder
		stz 	zTemp0+1
		stz 	zTemp1						; zTemp1 : result
		stz 	zTemp1+1
_MYALoop:
		tya 								; complete ?
		beq 	_MYAExit
		lsr 	a 							; right shift into carry
		tay
		bcc 	_MYANoAdd
		;
		clc 								; add if LSB set
		lda 	zTemp0
		adc 	zTemp1
		sta 	zTemp1
		lda 	zTemp0+1
		adc 	zTemp1+1
		sta 	zTemp1+1
_MYANoAdd:
		asl 	zTemp0
		rol 	zTemp0+1
		bra 	_MYALoop		
_MYAExit:
		lda 	zTemp1 						; return result.
		ldy 	zTemp1+1				
		rts
		
; ************************************************************************************************
;
;									    Multiply YA by 5
;
; ************************************************************************************************

MultiplyYABy5:
		sta 	zTemp0 						; save it.
		sty 	zTemp0+1
		asl 	zTemp0 						; x 4
		rol 	zTemp0+1
		asl 	zTemp0
		rol 	zTemp0+1
		;
		clc 								; add to original.
		adc 	zTemp0
		pha
		tya
		adc 	zTemp0+1
		tay
		pla
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
