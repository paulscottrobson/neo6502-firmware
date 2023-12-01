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
		ldx 	#0
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
		.byte 	3
		jsr 	DereferenceCheckTypes 		; dereference, check types match.
		bmi 	_CMPTypeError
		bvs 	_CMPString 					; string ?
		lda 	#6 							; use coprocessor to compare
		jsr 	DOMathCommand
		lda 	ControlStatus 				; get result.
		rts

_CMPTypeError:
		.error_type

		;
		;		String comparison
		;
_CMPString:
		.byte 	3
;		phy
;		;
;		lda 	(IFR1) 						; length of smaller of the two in X.
;		cmp 	(IFR0) 						; check it matches so far normally.
;		bcc 	_EXCBCSmaller
;		lda 	(IFR0)
;_EXCBCSmaller:
;		tax
;		beq 	_EXCBCMatches 				; if zero common length matches
;		;
;		;		Check the common length parts match.
;		;
;		ldy 	#0 							; match the strings.		
;_EXCBCCheckSmallerMatches:
;		iny 								; compare directly as far as common length
;		sec
;		lda 	(IFR0),y
;		sbc 	(IFR1),y
;		bne 	_EXCBCExit2
;		dex 
;		bne 	_EXCBCCheckSmallerMatches
		;
		;		The common length parts match, so we compare the lengths.
		;
;_EXCBCMatches:
;		sec
;		lda 	(IFR0) 						; common length matches. If same length equal
;		sbc 	(IFR1)						; if len(r1) > len(r0) then r1 is longer

;_EXCBCExit2:
;		ply
;		cmp 	#0 							; 0 equal.
;		beq 	_EXCBCReturn
;		bmi 	_EXCBCFF 					; return $FF if <
;		lda 	#1 							; return 1 if >
;_EXCBCReturn:
;		rts
;_EXCBCFF:
;		lda 	#$FF
;		rts				
;
;_EXCBCRange:
;		.error_range				
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

