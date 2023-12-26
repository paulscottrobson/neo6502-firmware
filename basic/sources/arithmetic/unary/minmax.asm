; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		minmax.asm
;		Purpose:	Minimum and maximum functions
;		Created:	26th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;											Min/Max
;
; ************************************************************************************************

		.section code	

EXPUnaryMin: ;; [min(]
		lda 	#0
		bra 	UMinMaxCode
EXPUnaryMax: ;; [max(]		
		lda 	#254

UMinMaxCode:
		pha 								; we toggle the final result with this.

		jsr 	EXPEvaluateExpressionAtX	; get a value
		jsr 	ERRCheckComma 				; check comma
		inx
		jsr 	EXPEvaluateExpressionAtX	; get a second value
		dex
		jsr 	ERRCheckRParen 				; )
		;
		jsr 	CMPCompareBaseCode			; compare deref/type check/compare returning value in A.
		beq 	_UMMExit1
		sta 	zTemp0 						; XOR with toggle value
		pla
		eor 	zTemp0
		bmi 	_UMMExit2 					; < after toggle exit
		;
		lda 	XSControl+1,x 				; copy 2nd to 1st
		sta 	XSControl,x
		lda 	XSNumber0+1,x
		sta 	XSNumber0,x
		lda 	XSNumber1+1,x
		sta 	XSNumber1,x
		lda 	XSNumber2+1,x
		sta 	XSNumber2,x
		lda 	XSNumber3+1,x
		sta 	XSNumber3,x
		rts

_UMMExit1:
		pla 								; throw toggle and exit
_UMMExit2:		
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

