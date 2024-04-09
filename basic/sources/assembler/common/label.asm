; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		label.asm
;		Purpose:	Handle assembler labels
;		Created:	27th December 2023
;		Reviewed: 	No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section 	code

; ************************************************************************************************
;
;											Define a label
;
; ************************************************************************************************

LabelCommand: ;; [.]
		ldx 	#0 							; get a term
		jsr 	EvaluateTerm 				
		lda 	XSControl 					; needs to be an integer reference.
		cmp 	#XS_ISVARIABLE
		bne 	_ALType 					; otherwise typing error

		lda 	XSNumber0,x 				; copy reference address to zTemp0
		sta 	zTemp0 						; (e.g. where we store the address)
		lda 	XSNumber1,x
		sta 	zTemp0+1

		phy 								; copy address in (64k)

		ldy 	#1 							; if the value is zero, we can write it.
		lda 	(zTemp0)
		ora 	(zTemp0),y
		beq 	_ALSetValue

		lda 	(zTemp0) 					; otherwise check it doesn't change
		cmp 	VariableP+0
		bne 	_ALError
		lda 	(zTemp0),y
		cmp 	VariableP+1
		beq 	_ALExit
_ALError:
		.error_label		

_ALSetValue
		lda	 	VariableP+0
		sta 	(zTemp0)
		lda	 	VariableP+1
		sta 	(zTemp0),y
_ALExit:		
		ply
		rts

_ALType:
		.error_type

		.send 	code

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
