; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		tokio.asm
;		Purpose:	Read/Write functions
;		Created:	14th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************
		
		.section code

; ************************************************************************************************
;
;									Output token X/A, X only if non-zero.
;
; ************************************************************************************************

TOKWriteXA:
		pha
		txa
		beq 	_TOKNoShift
		jsr 	TOKWriteA
_TOKNoShift:		
		pla
		jsr 	TOKWriteA
		rts

; ************************************************************************************************
;
;									Output token A
;
; ************************************************************************************************

TOKWriteA:
		phx
		ldx 	tokLineSize
		sta 	tokLineSize,x
		inc 	tokLineSize
		plx
		rts

; ************************************************************************************************
;
;		  Get current character in token buffer. Z flag set at End, with/without advance
;
; ************************************************************************************************				

TOKGet:
		phx
		lda 	#0
		ldx 	inputPos 					; end of buffer, return zero.
		cpx 	inputBuffer
		beq		_TGExit
		lda 	inputBuffer+1,x 			; read next character skipping length prefix.
_TGExit:	
		plx
		cmp 	#0
		rts		

TOKGetNext:
		jsr 	TOKGet
		beq 	_TGNExit
		inc 	inputPos
_TGNExit:
		cmp 	#0
		rts		

; ************************************************************************************************
;
;				Extract sequence of identifier charactes into the tokElement buffer
;
; ************************************************************************************************

TOKExtractIdentifier:
		stz 	tokElement
_TEILoop:
		jsr 	TOKGet 						; get next, check end of token text
		beq 	_TEIEndMainID		
		jsr 	TOKIsIdentifierElement 		; if identifier element
		bcc 	_TEIEndMainID
		jsr 	TOKGetNext 					; get and consume
		jsr 	TOKToUpper 					; capitalise it and add it
		jsr 	TEIAddChar
		bra 	_TEILoop
_TEIEndMainID:
		cmp 	#"$" 						; do we have a string ?
		bne 	_TEINotString
		jsr 	TEIAddChar 					; add it
		jsr 	TOKGetNext 					; consume $
		jsr 	TOKGet 						; get next
_TEINotString:
		cmp 	#"("						; do we have an array ?
		bne 	_TEINotArray
		jsr 	TEIAddChar 					; add (
		jsr 	TOKGetNext 					; consume (
_TEINotArray:		
		rts

; ************************************************************************************************
;
;					Extract sequence of digits into the tokElement buffer
;
; ************************************************************************************************

TOKExtractDigits:
		stz 	tokElement
_TEDLoop:
		jsr 	TOKGet 						; get next, check end of token text
		beq 	_TEDEndDigits  				; end of line
		jsr 	TOKIsDigit 		 			; check digit
		bcc 	_TEDEndDigits
		jsr 	TOKGetNext 					; get and consume and add
		jsr 	TEIAddChar
		bra 	_TEDLoop
_TEDEndDigits:
		rts

TEIAddChar:
		inc 	tokElement 					; add it.
		ldx 	tokElement
		sta 	tokElement,x
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

