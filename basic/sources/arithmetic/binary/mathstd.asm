; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		mathstd.asm
;		Purpose:	Standard Maths
;		Created:	1st December 2023
;		Reviewed:	No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section 	code

; ************************************************************************************************
;
;								Dereference top two terms
;
;	Then check the types are compatible - all number or string, put the type into NV via bit
;
; ************************************************************************************************

DereferenceCheckTypes:
		jsr 	DereferenceTerm 			; dereference first
		inx
		jsr 	DereferenceTerm 			; dereference second
		dex
		;
		lda 	XSControl,x 				; eor top bits, so non zero if different.
		eor 	XSControl+1,x
		bmi 	DRCType 					; type mismatch.
		bit 	XSControl,x 				; and set NV from the actual type
		rts

DRCType:
		.error_type

; ************************************************************************************************
;
; 											Add
;
; ************************************************************************************************

BinaryAdd: ;; [+]
		jsr 	DereferenceCheckTypes
		bmi 	DRCType
		lda 	#0
		jsr 	DoMathCommand
		jmp 	EXPRMainLoop

; ************************************************************************************************
;
; 											Subtract
;
; ************************************************************************************************

BinaryMinus: ;; [-]
		jsr 	DereferenceCheckTypes
		bmi 	DRCType
		lda 	#1
		jsr 	DoMathCommand
		jmp 	EXPRMainLoop

; ************************************************************************************************
;
;											Multiply
;
; ************************************************************************************************

BinaryTimes: ;; [*]
		jsr 	DereferenceCheckTypes
		bmi 	DRCType
		lda 	#2
		jsr 	DoMathCommand
		jmp 	EXPRMainLoop

; ************************************************************************************************
;
;											Divide (float)
;
; ************************************************************************************************

BinaryFDiv: ;; [/]
		jsr 	DereferenceCheckTypes
		bmi 	DRCType
		lda 	#3
		jsr 	DoMathCommand
BinaryCheckResult:
		lda 	ControlError
		bne 	_BCRError
		jmp 	EXPRMainLoop
_BCRError:
		.error_divzero

; ************************************************************************************************
;
;											Divide (int)
;
; ************************************************************************************************

BinaryUDiv: ;; [\]
		jsr 	DereferenceCheckTypes
		bmi 	DRCType
		lda 	#4
		jsr 	DoMathCommand
		bra 	BinaryCheckResult

; ************************************************************************************************
;
;											Modulus
;
; ************************************************************************************************

BinaryModulus: ;; [%]
		jsr 	DereferenceCheckTypes
		bmi 	DRCType
		lda 	#5
		jsr 	DoMathCommand
		bra 	BinaryCheckResult

; ************************************************************************************************
;
;							Compare n1 vs n2 - A returns $FF,$00,$01
;
; ************************************************************************************************

BinaryCompare: ;; [=]
		jsr 	DereferenceCheckTypes
		bmi 	DRCType
		lda 	#6
		jsr 	DoMathCommand
		lda 	ControlStatus
		.byte 	3
		rts

		.send 		code

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

