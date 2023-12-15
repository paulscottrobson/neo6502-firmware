; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		tok_identifier.asm
;		Purpose:	Tokenise identifier
;		Created:	15th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section code


; ************************************************************************************************
;
;										Tokenise identifier
;
; ************************************************************************************************

TokeniseIdentifier:
		jsr 	TOKExtractIdentifier 		; get the identifier.
		jsr 	TOKFindToken 				; try to find token.
		bcc 	_TINoToken  				; not found, it's an identifier.
		jsr 	TOKWriteXA 					; output it.
		rts
		;
		;		Not a token, so it must be an identifier.
		;
_TINoToken:		
		jsr 	TOKFindIdentifier 			; identifier exists ?
		bcs 	_TIHaveToken 				; yes, output it
		jsr 	TOKCreateIdentifier 		; else create it.
_TIHaveToken:
		pha 								; output the token.
		txa
		jsr 	TOKWriteA
		pla		
		jsr 	TOKWriteA
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

