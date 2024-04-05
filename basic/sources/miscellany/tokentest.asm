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



TIPtr = $FE

TestTokenising:	
		lda 	#TestInput & $FF
		sta 	TIPtr
		lda 	#TestInput >> 8
		sta 	TIPtr+1
_TTLoop:
		jsr 	TTGet
		tax
		beq 	_TTExit
		stz 	inputBuffer
_TTGetLine:
		jsr 	TTGet
		inc 	inputBuffer
		ldy 	inputBuffer
		sta 	inputBuffer,y
		dex
		bne 	_TTGetLine		
		jsr 	TOKTokenise
		jsr 	PGMDeleteLine
		lda 	tokLineSize 
		cmp 	#4
		beq 	_TTLoop
		jsr 	PGMInsertLine
		bra 	_TTLoop		
_TTExit:
		lda 	#Program >> 8
		sta 	$00
		jmp 	$FFFF		

TTGet:	lda 	(TIPtr)
		inc 	TIPtr
		bne 	_TTGExit
		inc 	TIPtr+1
_TTGExit:
		rts

TestInput:
		.include "../generated/inputtoken.dat"				

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

