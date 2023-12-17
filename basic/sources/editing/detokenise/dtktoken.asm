; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		dtktoken.asm
;		Purpose:	Detokenise token
;		Created:	17th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************
		
		.section code

; ************************************************************************************************
;
;										Detokenise line
;
; ************************************************************************************************

TOKDToken:
		sty 	zTemp0+1 					; save table address/
		stx 	zTemp0
		;
		tax 								; token ID in X.				
_TOKDFind:
		dex 								; reached the token position
		bpl 	_TOKDFound		
		sec 								; go to next entry in token table
		lda 	(zTemp0)
		adc 	zTemp0
		sta 	zTemp0
		bcc 	_TOKDFind
		inc 	zTemp0+1
		bra 	_TOKDFind
		;
		;		zTemp0 now points to the token.
		;
_TOKDFound:		
		lda 	(zTemp0) 					; length to X.
		beq 	_TOKDExit 					; not a token with text.
		tax 	 				
		ldy 	#1 							; output the token.
		lda 	(zTemp0),y 					; check spacing first character
		and 	#$7F
		jsr 	TOKDSpacing 				; do we need space before this.
_TOKDOutput:
		lda 	(zTemp0),y 					; output them in lower case
		and 	#$7F
		jsr 	TOKToLower
		jsr 	TOKDOutput
		iny
		dex
		bne 	_TOKDOutput		
_TOKDExit:
		rts		

; ************************************************************************************************
;
;							Check if spacing required, next character out is A
;
; ************************************************************************************************

		.send code
		
; ************************************************************************************************
;
;									Changes and Updates
;
; ************************************************************************************************
;
;		Date			Notes
;		==== 			=====
;		28/06/23 		Support shifted tokens.
;
; ************************************************************************************************

