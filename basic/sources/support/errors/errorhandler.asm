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
		tax 								; error number in X
		sta 	errNumber		 			; save error number
		lda 	ERRLine 		 			; save error line number
		sta 	errLineNumber
		lda 	ERRLine+1
		sta 	errLineNumber+1

		lda 	errCode 					; do we have ON ERROR active		
		ora 	errCode+1
		beq 	_EHDisplayMessage 			; if no, display the message.
		;
		;		Handle ON ERROR code
		;
		lda 	errCode 					; set up to run that code
		sta 	codePtr
		lda 	errCode+1
		sta 	codePtr+1
		ldy 	errCode+2

		stz 	errCode 					; zero code handler
		stz 	errCode+1
		ldx 	#$FF 						; 6502 stack reset.
		txs
		jmp 	RUNNewCommand 				; and go do that.

		;
		;		Build & Display error message
		;

_EHDisplayMessage:
		jsr 	EHBuildErrorMessage 		; create the error message.


		lda 	#errorBuffer & $FF 			; print it out.
		ldy 	#errorBuffer >> 8
		jsr 	CPPrintYA 
		lda 	#13 						; CR
		jsr 	WriteCharacter
		jmp 	WarmStart

; ************************************************************************************************
;
;								Build error message, error ID in X
;
; ************************************************************************************************

EHBuildErrorMessage:
		lda 	#ErrorMessageText & $FF		
		sta 	zTemp0
		lda 	#ErrorMessageText >> 8
		sta 	zTemp0+1
_EHFindMessage: 							; find the message
		dex
		beq 	_EHFoundMessage		
_EHSkip:
		lda 	(zTemp0)
		inc 	zTemp0
		bne 	_EHNoCarry
		inc 	zTemp0+1
_EHNoCarry:		
		cmp 	#0
		bne 	_EHSkip
		bra 	_EHFindMessage

_EHFoundMessage: 							; print the message
		stz 	errorBuffer 				; start building the temp buffer
		jsr 	_EHMPrint 					; output error message text
		;
		lda 	ERRLine 					; check for line #
		ora 	ERRLine+1		
		beq 	_EHEndMessage

		lda 	#_EHAtMsg >> 8 				; output " at line "
		sta 	zTemp0+1
		lda 	#_EHAtMsg & $FF
		sta 	zTemp0
		jsr 	_EHMPrint

		lda 	errLineNumber+1 			; convert line# to string
		sta 	XSNumber1
		lda 	errLineNumber
		sta 	XSNumber0
		stz 	XSNumber2
		stz 	XSNumber3
		stz 	XSControl
		ldx 	#0
		jsr 	CPNumberToString		
		sty 	zTemp0+1  					; write to error buffer
		sta 	zTemp0
		ldy 	#0
_EHDisplayLineNumber:
		iny
		lda 	(zTemp0),y
		inc 	errorBuffer
		ldx 	errorBuffer
		sta 	errorBuffer,x
		tya
		cmp 	(zTemp0)
		bne 	_EHDisplayLineNumber
_EHEndMessage:
		rts
_EHAtMsg:
		.text 	" at line ",0

_EHMPrint: 									; output string at zTemp0		
		ldy 	#0
_EHMPrintLoop:		
		lda 	(zTemp0),y
		inc 	errorBuffer
		ldx 	errorBuffer
		sta 	errorBuffer,x
		iny
		lda 	(zTemp0),y
		bne 	_EHMPrintLoop
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
;		28/03/24 		Code to put error message in a buffer.
;
; ************************************************************************************************

