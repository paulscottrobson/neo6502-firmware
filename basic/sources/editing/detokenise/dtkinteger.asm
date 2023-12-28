; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		dtkinteger.asm
;		Purpose:	Detokenise integer
;		Created:	17th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************
		
		.section code

; ************************************************************************************************
;
;									Detokenise integer
;
; ************************************************************************************************

TOKDInteger:
		ldy 	TOKOffset 					; get start of the number into Y 
		dey
		ldx 	#0 							; use the term evaluator.
		jsr 	ExtractTokenisedInteger
		sty 	TOKOffset
		jsr 	CPNumberToString 			; convert to string at YA 
		sta 	zTemp0
		sty 	zTemp0+1
		lda 	(zTemp0) 					; length in X
		tax
		ldy 	#1 							; check seperation
		lda 	(zTemp0),y		
		jsr 	TOKDSpacing
		lda 	#3
		jsr 	DTKColour	
_TOKDPrint:									; print the number
		lda 	(zTemp0),y
		iny
		jsr 	TOKDOutput
		dex
		bne 	_TOKDPrint
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

