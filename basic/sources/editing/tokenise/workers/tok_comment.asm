; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		tok_comment.asm
;		Purpose:	Tokenise a comment
;		Created:	17th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section code

; ************************************************************************************************
;
;								Tokenise a comment
;
; ************************************************************************************************

TokeniseComment:
		lda 	#KWD_SQUOTE 				; write out the ', which is a comment marker
		jsr 	TOKWriteA
		jsr 	TOKGetNext 					; consume it.
_TOKSkipSpaces:		
		jsr 	TOKGet 						; what follows ?
		beq 	_TCExit
		cmp 	#' '+1 						; non space ?
		bcs 	_TCCheck
		jsr 	TOKGetNext
		bra 	_TOKSkipSpaces
_TCCheck:
		cmp 	#'"'						; quote follows, exit.
		beq 	_TCExit
		jsr 	TOKCreateString 			; create a comment from the rest of the line.
_TCExit:
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

