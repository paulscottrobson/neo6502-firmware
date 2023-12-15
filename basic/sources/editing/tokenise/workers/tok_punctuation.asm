; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		tok_punctuation.asm
;		Purpose:	Tokenise punctuation
;		Created:	15th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section code


; ************************************************************************************************
;
;										Tokenise punctuation
;
; ************************************************************************************************

TokenisePunctuation:
		jsr 	TOKGetNext 					; get the first punctuation character
		sta 	tokElement+1 				; set it up
		jsr 	TOKGet 						; look at following character
		beq 	_TPOne 						; skip if end or space
		cmp 	#' '+1
		bcc 	_TPOne

		sta 	tokElement+2 				; make it a 2 character entry
		lda 	#2
		sta 	tokElement 
		jsr 	_TPTryPunctuation 			; try it.
		bcc 	_TPOne 						; failed, do one.
		jsr 	TOKGetNext 					; consume second character
_TPExit:		
		rts

_TPOne:
		lda 	#1 							; 1 character entry
		sta 	tokElement		
		jsr 	_TPTryPunctuation 			; try it.
		bcs 	_TPExit 					; matched, generated, exit
		.error_syntax 						; otherwise error

_TPTryPunctuation:		
		jsr 	TOKFindToken 				; try to find token.
		bcc 	_TPTPExit 					; not found.
		jsr 	TOKWriteXA 					; output the tokens.
		sec 								; was done.
_TPTPExit:
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

