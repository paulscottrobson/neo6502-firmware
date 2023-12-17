; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		dtkhex.asm
;		Purpose:	Detokenise hexadecimal
;		Created:	17th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************
		
		.section code

; ************************************************************************************************
;
;										Detokenise hexadecimal
;
; ************************************************************************************************

TOKDHexadecimal:
		lda 	#'A' 						; spacing if identifier because of $ double usage
		jsr 	TOKDSpacing
		lda 	#'$' 						; $
		jsr 	TOKDOutput
		ldy 	TOKOffset 					; get start of the number into Y 
		ldx 	#0 							; use the term evaluator.
		jsr 	ExtractTokenisedInteger
		sty 	TOKOffset
		stz 	zTemp0 						; use to truncate leading zeros.
		lda 	XSNumber3 					; print in hex
		jsr 	_TODHPrint
		lda 	XSNumber2
		jsr 	_TODHPrint
		lda 	XSNumber1
		jsr 	_TODHPrint
		lda 	XSNumber0
		jsr 	_TODHPrint
		bit 	zTemp0 						; was it zero, all suppressed
		bmi 	_TOKDNonZero
		lda 	#"0"
		jsr 	TOKDOutput
_TOKDNonZero:		
		rts

_TODHPrint: 								; print byte A suppress leading zero
		pha
		lsr 	a
		lsr 	a
		lsr 	a
		lsr 	a
		jsr 	_TODHPrintNibble
		pla
_TODHPrintNibble: 							; print nibble A suppress leading zero
		and 	#15
		bit 	zTemp0
		bmi 	_TODHNoSuppress
		cmp 	#0
		beq 	_TODHPNExit
_TODHNoSuppress:
		dec 	zTemp0
		cmp 	#10
		bcc 	_TODHDigit
		adc 	#6
_TODHDigit:
		adc 	#48
		jsr 	TOKDOutput
_TODHPNExit:		
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

