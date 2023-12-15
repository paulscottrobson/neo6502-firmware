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
		jsr 	TOKTokeniseOne 				; tokenise one element in the line
		stz 	tokIsFirstElement 			; reset first element flag
		bcs 	_TOKMainLoop
		lda 	#KWD_SYS_END 				; write end of line marker.
		jsr 	TOKWriteA
		rts

; ************************************************************************************************
;
;						Tokenise one element. Return CS to continue
;
; ************************************************************************************************

TOKTokeniseOne:	
		jsr 	TOKGet 						; get first character skipping spaces.
		beq 	_TOKExitFail	
		cmp 	#' '+1 	 					; > ' ', decide what to do with it.	
		bcs 	_TOKDecideWhat
		jsr 	TOKGetNext
		bra 	TOKTokeniseOne
		;
		;		Dispatcher.
		;
_TOKDecideWhat:
		;
		;		first character $, e.g. a hexadecimal constant
		;
		cmp 	#'$'
		bne 	_TOKNotHexadecimal
		jsr 	TokeniseHexConst
		sec
		rts
_TOKNotHexadecimal:
		;
		;		first character '"' e.g. a string constant.
		;
		cmp 	#'"'
		bne 	_TOKNotString
		jsr 	TokeniseString
		sec
		rts
_TOKNotString:
		;
		;		first character 0-9, e.g. an integer constant, possibly followed by a decimal set.
		;
		jsr 	TOKIsDigit 					
		bcc 	_TOKNotIntConst
		jsr 	TokeniseIntConst
		jsr 	TOKGet 
		cmp 	#"." 						; does it have a decimal
		bne 	_TOKNotDecimal
		jsr 	TokeniseDecimal
_TOKNotDecimal:		
		sec
		rts
_TOKNotIntConst:

		; 	TODO:If '.', do the packed BCD Decimal code.
		; 	If Identifier extract identifier, check trailing $( and write as token or identifier.
		; 	Check first 2 characters
		; 	Check 1 character

_TOKExitFail:		 						; tokenisation complete
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

