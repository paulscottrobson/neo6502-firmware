; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		detokenise.asm
;		Purpose:	Detokenise line
;		Created:	17th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************
		
		.section code

decode 	.macro 								; decode a keyword macro.
		ldy 	#(\1) >> 8
		ldx 	#(\1) & $FF
		jsr 	TOKDToken
		.endm

; ************************************************************************************************
;
;							Detokenise code at codePtr (not line numbers)
;
; ************************************************************************************************

TOKDetokenise:
		lda 	#3 							; start position.
		sta 	TOKOffset
		stz		TOKLastCharacter 			; clear last character
		stz 	inputPos 					; write position.
		;
		;		Main detokenising loop
		;
_TOKDLoop:
		jsr 	TOKDGet 					; get next
		cmp 	#KWD_SYS_END				; end of line, exit.
		beq 	_TOKDExit
		cmp 	#$20 						; 00-1F identifier
		bcc 	_TOKDIdentifier
		cmp 	#$40	 					; 20-3F binary operator.
		bcc 	_TOKDBinaryOp
		cmp 	#$00 						; 80-FF token (also shift, hex, string, decimals)
		bmi 	_TOKDToken
		jsr 	TOKDInteger 				; 40-7F integer
		bra 	_TOKDLoop

_TOKDIdentifier:
		jsr 	TOKDIdentifier 				; call the identifier decode code.
		bra 	_TOKDLoop
_TOKDBinaryOp:
		clc 								; make $80 based.
		adc 	#$60
		.decode BinaryTokenText
		bra 	_TOKDLoop

_TOKDToken:
		cmp 	#KWD_SYS_SH1 				; handle shifts.
		beq 	_TOKDShift1
		cmp 	#KWD_SYS_SH2
		beq 	_TOKDShift2
		cmp 	#KWD_SYS_STR 				; strings
		beq 	_TOKDString
		cmp 	#KWD_SYS_DEC 				; decimals
		beq 	_TOKDDecimal
		cmp 	#KWD_DOLLAR 				; hex
		beq 	_TOKDHex
		
		.decode BaseTokenText
		bra 	_TOKDLoop

_TOKDShift1:								; shift-1
		jsr 	TOKDGet
		.decode Shift1TokenText
		bra 	_TOKDLoop

_TOKDShift2:		 						; shift-2
		jsr 	TOKDGet
		.decode Shift2TokenText
		bra 	_TOKDLoop

_TOKDString: 								; string
		jsr 	TOKDString
		bra 	_TOKDLoop

_TOKDDecimal: 								; decimal
		jsr 	TOKDDecimal
		bra 	_TOKDLoop

_TOKDHex:									; hexadecimal
		jsr 	TOKDHexadecimal
		bra 	_TOKDLoop
_TOKDExit:
		clc
		rts

; ************************************************************************************************
;
;										Read Next Character
;
; ************************************************************************************************

TOKDGet:phy
		ldy 	tokOffset
		inc 	tokOffset
		lda 	(codePtr),y
		ply
		rts

; ************************************************************************************************
;
;							Output one character to the output buffer
;
; ************************************************************************************************

TOKDOutput:
		sta 	TOKLastCharacter 			; save last character
		phx
		ldx 	inputPos
		inc 	inputPos
		sta 	inputBuffer,x
		stz 	inputBuffer+1,x
		plx
		rts

; ************************************************************************************************
;
;										Spacing required
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

; ************************************************************************************************
;
;											Colour Scheme
;
; ************************************************************************************************

DTKColour:
		phx
		tax
		lda 	TOK_Colour_Scheme,x
		ora 	#$80
		jsr 	TOKDOutput
		plx
		rts

TOK_Colour_Scheme:
		.byte 	COL_WHITE 					; +0 comment
		.byte 	COL_CYAN 					; +1 keyword
		.byte 	COL_YELLOW 					; +2 identifier
		.byte 	COL_ORANGE 					; +3 constants
		.byte 	COL_WHITE 					; +4 string
		.byte 	COL_GREEN 					; +5 punctuation
		.byte 	COL_RED 					; +6 line number
		
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

