; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		dtkidentifier.asm
;		Purpose:	Detokenise identifier
;		Created:	28th May 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************
		
		.section code

; ************************************************************************************************
;
;										Detokenise identifier
;
; ************************************************************************************************

TOKDIdentifier:
		clc 								; MSB of record	
		adc 	#Program >> 8
		sta 	zTemp0+1
		jsr 	TOKDGet 					; LSB
		sta 	zTemp0
		ldy 	#5 							; start of name
		lda 	(zTemp0),y 					; check we need spacing.
		and 	#$7F
		jsr 	TOKDSpacing 				; do we need space before this.

_TOKDIOut:
		lda 	(zTemp0),y 					; get character
		iny
		pha 								; save it
		and 	#$7F 						; strip MSB
		jsr 	TOKToLower 					; make lower case.
		jsr 	TOKDOutput 					; output it.
		pla
		bpl 	_TOKDIOut 					; output the whole character.
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

