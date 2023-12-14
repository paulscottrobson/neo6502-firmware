; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		dtktoken.asm
;		Purpose:	Detokenise token
;		Created:	28th May 2023
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
		ldx 	#StandardTokens & $FF 		; standard table
		ldy 	#StandardTokens >> 8
		cmp 	#PR_LSQLSQSHIFTRSQRSQ 		; is it the shift token ?
		bne 	_TOKDSearch
		;
		jsr 	TOKDGet 					; get next
		ldx 	#AlternateTokens & $FF 		; alt table address
		ldy 	#AlternateTokens >> 8
		;
		;		Seach for token A in table YX.
		;
_TOKDSearch:		
		stx 	zTemp0 						; save table in zTemp0
		sty 	zTemp0+1
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
		lda 	(zTemp0),y 					; check spacing first character,
		jsr 	TOKDSpacing 				; do we need space before this.
_TOKDOutput:
		lda 	(zTemp0),y 					; output them in lower case
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

TOKDSpacing:
		jsr 	TOKIsIdentifierElement		; next character alphanumeric
		bcc 	TOKDSExit
TOKDSpaceLastAlpha:		
		lda 	TOKLastCharacter			; and last character also alphanumeric
		jsr 	TOKIsIdentifierElement
		bcc 	TOKDSExit
		lda 	#" " 						; we need a space.
		jsr 	TOKDOutput
TOKDSExit:
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
;		28/06/23 		Support shifted tokens.
;
; ************************************************************************************************

