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
		;		Display error message
		;
_EHDisplayMessage:		
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
		ldy 	#0
_EHMPrint:		
		lda 	(zTemp0),y
		jsr 	CPPrintA
		iny
		lda 	(zTemp0),y
		bne 	_EHMPrint
		;
		lda 	ERRLine 					; check for line #
		ora 	ERRLine+1		
		beq 	_EHWarmStart
		ldy 	#_EHAtMsg >> 8
		lda 	#_EHAtMsg & $FF
		jsr 	CPPrintYA 
		lda 	ERRLine 					; print line #
		ldx 	ERRLine+1
		jsr 	PrintNumberXA

_EHWarmStart:
		lda 	#13 						; CR
		jsr 	WriteCharacter
		jmp 	WarmStart

_EHAtMsg:
		.text 	9," at line "

PrintNumberXA:
		phy
		stx 	XSNumber1
		sta 	XSNumber0
		ldx 	#0 							; print line number.
		stz 	XSNumber2
		stz 	XSNumber3
		stz 	XSControl
		jsr 	CPNumberToString		
		jsr 	CPPrintYA 
		ply
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

