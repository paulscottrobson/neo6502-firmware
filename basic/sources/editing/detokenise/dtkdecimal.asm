; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		dtkdecimal.asm
;		Purpose:	Detokenise decimal
;		Created:	17th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************
		
		.section code

; ************************************************************************************************
;
;										Detokenise decimal
;
; ************************************************************************************************

TOKDDecimal:
		lda 	#'.' 						; decimal point.
		jsr 	TOKDOutput
		jsr 	TOKDGet 					; string length
		tax
		beq 	_TOKDDExit
_TOKDDLoop: 								; output string.
		jsr 	TOKDGet
		pha
		lsr 	a
		lsr 	a
		lsr 	a
		lsr 	a
		jsr 	_TOKDDigit
		pla
		jsr 	_TOKDDigit
		dex
		bne 	_TOKDDLoop
_TOKDDExit:		
		rts				

_TOKDDigit:
		and 	#15
		cmp 	#15
		beq 	_TOKDDExit
		ora 	#"0"
		jsr 	TOKDOutput
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
;	 	30/06/23 		Test was using bmi for complete so didn't work for > 127.
;
; ************************************************************************************************

