; ************************************************************************************************
; ************************************************************************************************
;
;       Name:       tokentest.asm
;       Purpose:    Tokeniser testing code
;       Created:    15th December 2023
;       Reviewed:   No
;       Author:     Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section code
		.if BASICBUILD==2

TestTokenising:
		lda 	#_Test1 & $FF
		ldy 	#_Test1 >> 8
		jsr 	LoadTokenCodeYA
		jsr 	TOKTokenise
_h1:	.byte 	3	
		bra 	_h1

_Test1:	.byte 	_Test1End-_Test1-1
		.text 	' "Hello world"'
_Test1End:						
;
;           Temp bodges of various kinds.
;
LoadTokenCodeYA:
		sta 	zTemp0
		sty 	zTemp0+1
		lda 	(zTemp0)
		tax
		inx
		ldy 	#0
_LTCCopy:		
		lda 	(zTemp0),y
		sta 	inputBuffer,y
		iny
		dex
		bne 	_LTCCopy
		rts

		.endif
		.send code


; ************************************************************************************************
;
;                                   Changes and Updates
;
; ************************************************************************************************
;
;       Date            Notes
;       ====            =====
;
; ************************************************************************************************

