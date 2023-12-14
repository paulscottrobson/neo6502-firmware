; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		dtkidentifier.asm
;		Purpose:	Detokenise identifier
;		Created:	28th May 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************
		
		.section code

; ************************************************************************************************
;
;										Detokenise identifier
;
; ************************************************************************************************

TOKDIdentifier:
		ldy 	#$FF 						; flag set on first character only.
		sty 	TOKDIFirstChar
_TOKDLoop:
		tay 								; token in Y, now coner it to ASCII
		lda 	#'_' 						; handle _
		cpy 	#$64
		beq 	_TOKDIOutput
		tya 								; handle a-z
		clc
		adc	 	#$21
		cpy 	#$5A
		bcc 	_TOKDIOutput
		sec 								; handle 0-9
		sbc 	#$4B
		;
		;		ASCII equivalent in A
		;	
_TOKDIOutput:		
		bit 	TOKDIFirstChar 				; check spacing if first character
		bpl 	_TOKDINoSpacing
		pha
		jsr 	TOKDSpacing
		stz 	TOKDIFirstChar
		pla
_TOKDINoSpacing:		
		;
		;		Output
		;
		jsr 	TOKDOutput		
		jsr 	TOKDGet 					; get next token
		cmp 	#$7C 						; 7C..7F are end markers
		bcc 	_TOKDLoop
		;
		;		Output the end $ ( or $(
		;
		beq 	_TOKDIExit 					; it's a number, no tail.
		lsr 	a 							; string ?
		bcc 	_TOKDICheckArray
		pha
		lda 	#"$"
		jsr 	TOKDOutput
		pla
_TOKDICheckArray:
		lsr 	a 							; array ?
		bcc 	_TOKDIExit		
		lda 	#"("
		jsr 	TOKDOutput
_TOKDIExit:
		rts		

		.send code
		
		.section storage
TOKDIFirstChar:
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
; 		01/07/23 		. is no longer part of an identifier, it's a token.
;
; ************************************************************************************************

