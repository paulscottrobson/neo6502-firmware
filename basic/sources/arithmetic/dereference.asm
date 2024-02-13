; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		dereference.asm
;		Purpose:	Dereference ... references
;		Created:	29th November 2023
;		Reviewed:	No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section 	code

; ************************************************************************************************
;
;								Dereference a term
;
; ************************************************************************************************

DereferenceTOS:	
		lda 	XSControl,x 				; check if reference ?
		and 	#XS_ISVARIABLE
		beq 	_DRTExit 					; no, exit
		;
		lda 	XSNumber0,x 				; copy address to zTemp0
		sta 	zTemp0
		lda 	XSNumber1,x
		sta 	zTemp0+1
		;
		lda 	XSControl,x 				; clear reference bit.
		and 	#$FF-XS_ISVARIABLE
		sta 	XSControl,x

		bmi 	_DRTString

		phy 								; dereference a number.
		lda 	(zTemp0)
		sta 	XSNumber0,x
		ldy 	#1
		lda 	(zTemp0),y
		sta 	XSNumber1,x
		iny

		lda 	XSControl,x 				; check for word reference
		and 	#XS_ISWORDREF
		bne 	_DRTWordRef

		lda 	(zTemp0),y 					; 32 bit dereference
		sta 	XSNumber2,x
		iny
		lda 	(zTemp0),y
		sta 	XSNumber3,x
		ply
		bra 	_DRTExit

_DRTWordRef:
		stz 	XSControl,x 				; clear type
		lda 	#0  						; clear 2 upper bytes
		sta 	(zTemp0),y
		iny
		sta 	(zTemp0),y
		ply
		bra 	_DRTExit

_DRTString:		
		phy
		ldy 	#1
		lda 	(zTemp0) 				; is it $0000 e.g. unassigned string ?
		ora 	(zTemp0),y
		beq 	_DRTNullString

		lda 	(zTemp0) 				; dereference assigned string.
		sta 	XSNumber0,x
		ldy 	#1
		lda 	(zTemp0),y
		sta 	XSNumber1,x
		ply
		bra 	_DRTExit

_DRTNullString: 							; string dereferenced is $0000 e.g. unassigned.
		lda 	#_DRTNullStringAddr & $FF
		sta 	XSNumber0,x
		lda 	#_DRTNullStringAddr >> 8
		sta 	XSNumber1,x
		ply

_DRTExit:		
		rts

_DRTNullStringAddr:
		.byte 	0

		.send 		code

; ************************************************************************************************
;
;									Changes and Updates
;
; ************************************************************************************************
;
;		Date			Notes
;		==== 			=====
;		12/02/24 		Dereferences 16 bit references successfully (for [])
;
; ************************************************************************************************
