; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		tok_decimal.asm
;		Purpose:	Tokenise a decimal (dp followed by digits)
;		Created:	15th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section code


; ************************************************************************************************
;
;										Tokenise a decimal
;
; ************************************************************************************************

TokeniseDecimal:
		jsr 	TOKGetNext 					; consume the '.'		
		lda 	#KWD_SYS_DEC 				; write out the decimal marker.
		jsr 	TOKWriteA

		lda 	tokLineSize 				; save the position of the length so we can update it.
		sta 	tokLengthPos
		lda 	#0 							; write initial length zero.
		jsr 	TOKWriteA
		stz 	tokDecCount 				; count the decimal position.
_TDLoop:
		jsr 	TOKGet 						; get next
		beq 	_TDExit 					; end of line
		jsr 	TOKIsDigit  				; check if digit.
		bcc 	_TDExit
		jsr 	_TDWriteNibble 				; output it
		jsr 	TOKGetNext 					; consume it and loop back
		bra 	_TDLoop
_TDExit:
		lda 	#$0F 						; trailing write nibble
		jsr 	_TDWriteNibble
		rts		
;
;		Append Nibble A.
;
_TDWriteNibble:
		ldx 	tokDecCount  				; limit to 6 digits
		cpx 	#6
		bcs 	_TDWNExit
		;
		and 	#$0f 						; mask off digit.
		pha 								; shift bit 0 of count into carry
		lda 	tokDecCount
		lsr 	a
		pla
		inc 	tokDecCount 				; bump count
		bcs 	_TDSecondNibble 			; if set, it's a second nibble.

		asl 	a 							; shift into upper nibble
		asl 	a
		asl 	a
		asl 	a
		ora 	#$0F 						; put 1111 in the lower nibble.
		jsr 	TOKWriteA 					; output it
		ldx 	tokLengthPos 				; increment the length
		inc 	tokLineSize,x
_TDWNExit:		
		rts

_TDSecondNibble:
		ldx 	tokLineSize 				; put into lower nibble
		sta 	zTemp0
		lda 	tokLineSize-1,x		
		and 	#$F0
		ora 	zTemp0
		sta 	tokLineSize-1,x
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

