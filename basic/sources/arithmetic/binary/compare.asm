; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		compare.asm
;		Purpose:	Compare operators
;		Created:	1st December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section code	

compare_equals .macro
		jsr 	CMPCompareBaseCode
		cmp 	#\1
		beq 	CMPReturnTrue
		bra 	CMPReturnFalse
		.endm

compare_not_equals .macro
		jsr 	CMPCompareBaseCode
		cmp 	#\1
		bne 	CMPReturnTrue
		bra 	CMPReturnFalse
		.endm

; ***************************************************************************************
;
; 						Return True/False as function or value
;
; ***************************************************************************************

CMPReturnTrue:		
		lda 	#$FF
		bra 	CMPSetTOS
CMPReturnFalse:		
		lda 	#0
CMPSetTOS:
		sta 	XSNumber3,x
		sta 	XSNumber2,x
		sta 	XSNumber1,x
		sta 	XSNumber0,x
		stz 	XSControl,x
		jmp 	EXPRMainLoop

; ***************************************************************************************
;
; 								> = < (compare == value)
;
; ***************************************************************************************

CMPCompareEqual: 			;; [=]
		.compare_equals 0
		
CMPCompareLess: 			;; [<]
		.compare_equals $FF

CMPCompareGreater: 			;; [>]
		.compare_equals 1

; ***************************************************************************************
;
; 								> = < (compare <> value)
;
; ***************************************************************************************

CMPCompareNotEqual: 		;; [<>]
		.compare_not_equals 0

CMPCompareLessEqual: 		;; [<=]
		.compare_not_equals 1

CMPCompareGreaterEqual: 	;; [>=]
		.compare_not_equals $FF

; ************************************************************************************************
;
;									Common compare code
;
; ************************************************************************************************

CMPCompareBaseCode:
		jsr 	DereferenceCheckTypes 		; dereference, check types match.
		bmi 	_CMPString 					; string ?
		;
		lda 	XSControl,x 				; comparing integers
		ora 	XSControl+1,x
		and 	#XS_TYPEMASK
		beq 	_CMPInteger

		lda 	#6 							; use coprocessor to compare
		jsr 	DOMathCommand
		lda 	ControlStatus 				; get result.
		rts
		;
		;		Integer comparison
		;
_CMPInteger:
		phy 								; save Y
		sec 								; subtract
		.binop 	sbc
		bvc 	_CMPINoOverflow 			; signed comparison
		eor 	#$80
_CMPINoOverflow:
		lda 	XSNumber0,x 				; check if result zero
		ora 	XSNumber1,x
		ora 	XSNumber2,x
		ora 	XSNumber3,x
		tay 								; store in Y
		beq 	_CMPIExit 					; if so, return zero.
		ldy 	#$FF 						; return -1
		lda 	XSNumber3,x 				; if bit 31 set
		bmi 	_CMPIExit 					
		ldy 	#$01 						; else +ve
_CMPIExit:
		clc 								; fix up result, non destructive.
		.binop 	adc 				
		tya 								; result in A
		ply 								; restore Y
		rts
		;
		;		String comparison
		;
_CMPString:
		phy
		phx 					

		lda 	XSNumber0,x
		sta 	zTemp0
		lda 	XSNumber1,x
		sta 	zTemp0+1

		lda 	XSNumber0+1,x
		sta 	zTemp1
		lda 	XSNumber1+1,x
		sta 	zTemp1+1

		lda 	(zTemp1) 					; length of smaller of the two in X.
		cmp 	(zTemp0) 					; check it matches so far normally.
		bcc 	_EXCBCSmaller
		lda 	(zTemp0)
_EXCBCSmaller:
		cmp 	#0							; if zero common length matches
		beq 	_EXCBCMatches 
		tax
		;
		;		Check the common length parts match.
		;
		ldy 	#0 							; match the strings.		
_EXCBCCheckSmallerMatches:
		iny 								; compare directly as far as common length
		sec
		lda 	(zTemp0),y
		sbc 	(zTemp1),y
		bne 	_EXCBCExit2
		dex 
		bne 	_EXCBCCheckSmallerMatches
		;
		;		The common length parts match, so we compare the lengths.
		;
_EXCBCMatches:
		sec
		lda 	(zTemp0) 					; common length matches. If same length equal
		sbc 	(zTemp1)					; if len(r1) > len(r0) then r1 is longer

_EXCBCExit2:
		plx 								; restore X & Y
		ply
		cmp 	#0 							; 0 equal.
		beq 	_EXCBCReturn
		bmi 	_EXCBCFF 					; return $FF if <
		lda 	#1 							; return 1 if >
_EXCBCReturn:
		rts
_EXCBCFF:
		lda 	#$FF
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

