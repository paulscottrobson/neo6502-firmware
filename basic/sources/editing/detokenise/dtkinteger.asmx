; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		dtkinteger.asm
;		Purpose:	Detokenise integer
;		Created:	28th May 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************
		
		.section code

; ************************************************************************************************
;
;							Detokenise integer, first token A, base Y
;
; ************************************************************************************************

TOKDInteger:
		phy 								; save base on stack
		ldx 	#IFR0 						; set first byte of integer into into R0
		jsr 	IFSetByte
_TOKDILoop:
		lda 	(zTemp2) 					; followed by a 00-3F, extract the whole integer out.
		cmp 	#$40
		bcs 	_TOKDIHaveInteger
		ldx 	#IFR0 						; R0 << 6
		jsr 	IFShiftLeft
		jsr 	IFShiftLeft
		jsr 	IFShiftLeft
		jsr 	IFShiftLeft
		jsr 	IFShiftLeft
		jsr 	IFShiftLeft
		jsr 	TOKDGet 					; OR byte in.
		ora 	IFR0+IFMantissa0
		sta 	IFR0+IFMantissa0
		bra 	_TOKDILoop
		
_TOKDIHaveInteger:							; integer in R0, base on TOS.
		ply 								; restore base
		tya 								; base in A
		jsr 	IFIntToString 				; make it a string		
		stx 	zTemp0 						; output that string.
		sty 	zTemp0+1
		lda 	(zTemp0)
		ldy 	#1 							; output buffer.
		lda 	(zTemp0),y
		jsr 	TOKDSpacing 				; check spacing okay.
_TOKDOutput:
		lda 	(zTemp0),y
		jsr 	TOKDOutput
		iny		
		lda 	(zTemp0),y
		bne 	_TOKDOutput
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
;		28/06/23 		Fixed spacing "print 22" became print 22
;
; ************************************************************************************************

