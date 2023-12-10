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
		jsr 	DereferenceTOS 				; dereference first
		inx
		jsr 	DereferenceTOS 				; dereference second
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
; 											Subtract
;
; ************************************************************************************************

BinaryMinus: ;; [-]
		jsr 	DereferenceCheckTypes
		bmi 	DRCType
		lda 	XSControl,x
		ora 	XSControl+1,x
		and 	#XS_TYPEMASK
		beq 	_BASubInteger
		lda 	#1
		jsr 	DoMathCommand
		jmp 	EXPRMainLoop

_BASubInteger:
		sec
		.binop 	sbc
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
; 											Add
;
; ************************************************************************************************

BinaryAdd: ;; [+]
		jsr 	DereferenceCheckTypes
		bmi 	_BAConcatenate
		lda 	XSControl,x
		ora 	XSControl+1,x
		and 	#XS_TYPEMASK
		beq 	_BAAddInteger
		lda 	#0
		jsr 	DoMathCommand
		jmp 	EXPRMainLoop

_BAADDInteger:
		clc
		.binop 	adc
		jmp 	EXPRMainLoop

_BAConcatenate:
		phy

		lda 	XSNumber0+1,x 				; work out the length of the concatenated string.
		sta 	zTemp0 						; pushing addresses onto the stack
		pha
		lda 	XSNumber1+1,x
		sta 	zTemp0+1
		pha
		lda 	XSNumber0,x
		sta 	zTemp1
		pha
		lda 	XSNumber1,x
		sta 	zTemp1+1
		pha
		;
		clc
		lda 	(zTemp0)
		adc 	(zTemp1)
		bcs 	_BALength
		cmp 	#252
		bcs 	_BALength
		jsr 	StringTempAllocate 			; allocate slot for it.

		ply
		pla
		jsr 	_BAOutString 				; output strings
		
		ply
		pla
		jsr 	_BAOutString

		ply
		jmp 	EXPRMainLoop 				; and loop round.

_BALength:
		.error_stringsize		

_BAOutString: 								; output string at stack,X
		sta 	zTemp0
		sty 	zTemp0+1
		phx
		lda 	(zTemp0) 					; get length
		beq 	_BAOSExit
		tax 								; count in X
		ldy 	#1 							; index to read
_BAOSLoop:
		lda 	(zTemp0),y
		jsr 	StringTempWrite
		iny
		dex
		bne 	_BAOSLoop		
_BAOSExit:
		plx
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

