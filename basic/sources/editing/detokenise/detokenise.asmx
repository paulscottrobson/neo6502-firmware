; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		detokenise.asm
;		Purpose:	Detokenise line
;		Created:	28th May 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************
		
		.section code

; ************************************************************************************************
;
;						Detokenise code at YX (not including line numbers)
;
; ************************************************************************************************

TOKDetokenise:
		stx 	zTemp2 						; save tokenised code in zTemp2
		sty 	zTemp2+1
		stz		TOKLastCharacter 			; clear last character
		;
		;		Main detokenising loop
		;
_TOKDLoop:
		jsr 	TOKDGet 					; get next
		cmp 	#PR_LSQLSQENDRSQRSQ			; end of line, exit.
		beq 	_TOKDExit
		;
		cmp 	#PR_LSQLSQSTRINGRSQRSQ		; is it a string or decimal places modifier
		beq 	_TOKDDataItem
		cmp 	#PR_LSQLSQDECIMALRSQRSQ
		beq 	_TOKDDataItem
		;
		cmp 	#PR_AMPERSAND 				; & is a special case.
		beq 	_TOKAmpersand
		;
		cmp 	#0 							; is it a token 80-FF
		bpl 	_TOKDNotToken
		jsr 	TOKDToken 					; token to text.		
		bra 	_TOKDLoop
		;
		;		00-7F which are integers and identifiers.
		;
_TOKDNotToken:
		cmp 	#$40  						; 40-7F Identifier
		bcc 	_TOKDNotIdentifier
		jsr 	TOKDIdentifier
		bra 	_TOKDLoop
		;
_TOKDNotIdentifier: 						; 00-3F Base 10 Integer
		ldy 	#10 						
		jsr 	TOKDInteger
		bra 	_TOKDLoop
		;
		;		&<hex> code, outputs following integer in hex format.
		;
_TOKAmpersand:
		jsr 	TOKDSpaceLastAlpha  		; space if last alpha
		lda 	#"&" 						; output hex marker
		jsr 	TOKDOutput
		jsr 	TOKDGet 					; get first char of integer
		ldy 	#16 						; expand in base 16
		jsr 	TOKDInteger
		bra 	_TOKDLoop
		;
		;		Handle data
		;
_TOKDDataItem:								; [[STRING]] [[DECIMAL]]
		jsr 	TOKDDataItem
		bra 	_TOKDLoop

_TOKDExit:
		clc
		rts

; ************************************************************************************************
;
;										Read Next Character
;
; ************************************************************************************************

TOKDGet:lda 	(zTemp2)
		inc 	zTemp2
		bne 	_TKDGExit
		inc 	zTemp2+1
_TKDGExit:
		rts

; ************************************************************************************************
;
;						Output one character to whatever handler is set up
;
; ************************************************************************************************

TOKDOutput:
		sta 	TOKLastCharacter 			; save last character
		jmp 	(TOKOutputMethod) 			; call output handler

; ************************************************************************************************
;
;									Set the handler.
;
; ************************************************************************************************

TOKSetDetokeniseOutput:	
		stx 	TOKOutputMethod
		sty 	TOKOutputMethod+1
		rts

		.send code
		
		.section storage
TOKOutputMethod:							; routine to handle output characters
		.fill 	2
TOKLastCharacter: 							; last character output.
		.fill 	1		
		.send storage

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

