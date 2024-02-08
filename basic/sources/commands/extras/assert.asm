; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		assert.asm
;		Purpose:	Asserts an expression
;		Created:	3rd December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;										ASSERT Command
;
; ************************************************************************************************

		.section code

Command_ASSERT:	;; [assert]
		ldx 	#0
		jsr 	EXPEvalNumber 				; get a number to assert
		stz 	XSNumber0+1 				; zero the string part in case not used.
		stz 	XSNumber1+1
		lda 	(codePtr),y 				; check if , present
		cmp 	#KWD_COMMA
		bne 	_CANoComma
		iny 								; consume comma
		inx
		jsr 	EXPEvalString 				; get the message to slot #1
_CANoComma:
		ldx 	#0
		jsr 	CheckIfZero
		beq 	_CAFail 					; if so, the assert fails.
		rts
_CAFail:		
		lda 	XSNumber0+1 				; check if string present
		ora 	XSNumber1+1
		beq 	_CAReport
		lda		XSNumber0+1
		ldy 	XSNumber1+1
		jsr 	CPPrintYA
		lda 	#':'
		jsr 	CPPrintA
_CAReport:
		.error_assert

; ************************************************************************************************
;
;										Check if TOS is zero
;
; ************************************************************************************************
		
CheckIfZero:
		lda 	XSControl,x
		bne 	_CAType
		lda 	XSNumber0,x
		ora 	XSNumber1,x
		ora 	XSNumber2,x
		ora 	XSNumber3,x
		rts

_CAType:
		.error_type

		.send code

; ************************************************************************************************
;
;									Changes and Updates
;
; ************************************************************************************************
;
;		Date			Notes
;		==== 			=====
;		08-02-24 		Added optional message.
;
; ************************************************************************************************

