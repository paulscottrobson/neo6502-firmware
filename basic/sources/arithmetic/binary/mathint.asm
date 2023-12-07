; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		mathint.asm
;		Purpose:	Integer Maths
;		Created:	1st December 2023
;		Reviewed:	No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section 	code

; ************************************************************************************************
;
; 											And
;
; ************************************************************************************************

BinaryAnd: ;; [&]
		jsr 	DereferenceCheckTypes
		jsr 	MICheckBothInteger
		.binop 	and
		jmp 	EXPRMainLoop

; ************************************************************************************************
;
; 											Xor
;
; ************************************************************************************************

BinaryXor: ;; [^]
		jsr 	DereferenceCheckTypes
		jsr 	MICheckBothInteger
		.binop 	eor
		jmp 	EXPRMainLoop

; ************************************************************************************************
;
; 											Or
;
; ************************************************************************************************

BinaryOr: ;; [|]
		jsr 	DereferenceCheckTypes
		jsr 	MICheckBothInteger
		.binop 	ora
		jmp 	EXPRMainLoop

; ************************************************************************************************
;
; 									  Shift left/right
;
; ************************************************************************************************

BinaryShiftLeft: ;; [<<]
		sec
		bra 	BinaryShift
BinaryShiftRight: ;; [>>]
		clc
BinaryShift:
		php									; save direction, CS left CC right.
		jsr 	DereferenceCheckTypes
		jsr 	MICheckBothInteger
		;
		lda 	XSNumber0+1,x 				; if shift >=32 will be zero
		and 	#$E0
		ora 	XSNumber1+1,x
		ora 	XSNumber1+2,x
		ora 	XSNumber1+3,x
		bne 	_BSIsZero
		lda 	XSNumber0+1,x 				; check zero at start
		beq 	_BSExit
_BSLoop:		
		plp 								; access diection
		php
		bcs 	_BSLeft

		lsr 	XSNumber3,x 				; shift right
		ror 	XSNumber2,x
		ror 	XSNumber1,x
		ror 	XSNumber0,x
		bra 	_BSExitLoop

_BSLeft:
		asl 	XSNumber0,x 				; shift left
		rol 	XSNumber1,x
		rol 	XSNumber2,x
		rol 	XSNumber3,x

_BSExitLoop:
		dec 	XSNumber0+1,x 				; do that many times
		bne 	_BSLoop
		bra 	_BSExit
		
_BSIsZero:
		stz 	XSNumber0,x					; return zero.
		stz 	XSNumber1,x		
		stz 	XSNumber2,x		
		stz 	XSNumber3,x		
_BSExit:
		plp 								; throw direction.
		jmp 	EXPRMainLoop
	
; ************************************************************************************************
;
; 										Check top two integer.
;
; ************************************************************************************************

MICheckBothInteger:
		lda 	XSControl,x
		ora 	XSControl,x
		and 	#$C0
		bne 	_MICBIType
		rts

_MICBIType:
		.error_type

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

