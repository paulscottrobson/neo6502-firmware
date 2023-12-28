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
		pha
		jsr 	TOKDSpacing 				; do we need space before this.
		pla
		cmp 	#"'" 						; is it quote ?
		beq 	_TOKDIsComment
		jsr 	TOKIsAlpha 					; is it an alpha token ?
		bcc 	_TOKDPunctuation
		lda 	#1
		bra 	_TOKDSetColour
_TOKDIsComment:
		lda 	#0		
		bra 	_TOKDSetColour
_TOKDPunctuation:
		lda 	#5
_TOKDSetColour:	
		jsr 	DTKColour	
_TOKDOutput:
		lda 	(zTemp0),y 					; output them in lower case
		and 	#$7F
		jsr 	TOKToLower
		cmp		#"(" 						; change colour on (
		bne 	_TOKDNotBracket
		lda 	#5
		jsr 	DTKColour
		lda 	#"("
_TOKDNotBracket:		
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
;
; ************************************************************************************************

