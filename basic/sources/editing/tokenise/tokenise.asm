; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		tokenise.asm
;		Purpose:	Tokenise the line from the data source.
;		Created:	14th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section code

; ************************************************************************************************
;
;								Tokenise line in input buffer
;
; ************************************************************************************************

TOKTokenise:
		lda 	#1 							; set first element flag.
		sta 	tokIsFirstElement
		;
		lda 	#3 							; set the line length to three for the 
		sta 	tokLineSize 				; line length itself and the line numbers.
		stz 	tokLineNumber
		stz 	tokLineNumber+1
		;
		stz 	inputPos 					; input position.
		
_TOKMainLoop:
		.byte 	3
		jsr 	TOKTokeniseOne
		bcs 	_TOKMainLoop
		lda 	#KWD_SYS_END
		jsr 	TOKWriteA
		rts

; ************************************************************************************************
;
;						Tokenise one element. Return CS to continue
;
; ************************************************************************************************

TOKTokeniseOne:		
		; 	Skip spaces
		; 	If End, exit with CC.
		; 	If '$', extract identifier convert from hex and write as constant
		; 	If '"', do the string constant code.
		; 	If '.', do the packed BCD Decimal code.
		;	If Decimal extract identifier, convert from decimal and write as constant / line number.
		; 	If Identifier extract identifier, check trailing $( and write as token or identifier.
		; 	Check first 2 characters
		; 	Check 1 character
		clc
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

