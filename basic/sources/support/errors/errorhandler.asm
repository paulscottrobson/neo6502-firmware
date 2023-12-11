; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		errorhandler.asm
;		Purpose:	Handle Errors
;		Created:	11th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;											Handle Errors
;
; ************************************************************************************************

		.section code	

Unimplemented:
		.error_unimp

ErrorHandler:		
		tay 								; error number in Y
		ldx 	#0
_EHFindMessage: 							; find the message
		dey
		beq 	_EHFoundMessage		
_EHSkip:
		lda 	ErrorMessageText,x
		inx
		cmp 	#0
		bne 	_EHSkip
		bra 	_EHFindMessage
_EHFoundMessage: 							; print the message
		lda 	ErrorMessageText,x
		jsr 	CPPrintA
		inx
		lda 	ErrorMessageText,x 			
		bne 	_EHFoundMessage				
		;
		lda 	ERRLine 					; check for line #
		ora 	ERRLine+1		
		beq 	_EHWarmStart
		ldy 	#_EHAtMsg >> 8
		lda 	#_EHAtMsg & $FF
		jsr 	CPPrintYA 

		ldx 	#0 							; print line number.
		lda 	ERRLine
		sta 	XSNumber0,x
		lda 	ERRLine+1
		sta 	XSNumber1,x
		stz 	XSNumber2,x
		stz 	XSNumber3,x
		stz 	XSControl,x
		jsr 	CPNumberToString		
		jsr 	CPPrintYA 

_EHWarmStart:
		lda 	#13 						; CR
		jsr 	WriteCharacter
		jmp 	WarmStart

_EHAtMsg:
		.text 	9," at line "

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

