 ; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		mouse.asm
;		Purpose:	Return mouse state
;		Created:	9th April 2024
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;		MOUSE(x,y) 		Sets X,Y to mouse position, returns buttons (bit 0 left, bit 1 right)
;		MOUSE(x,y,s)	Same but returns scroll wheel.
;
; ************************************************************************************************

		.section code	

EXPUnaryMouse: ;; [mouse(]
		jsr 	EvaluateTerm 					; first parameter.
		jsr 	ERRCheckComma
		inx
		jsr 	EvaluateTerm 					; second parameter.
		inx
		lda 	#$FF 							; default 3rd parameter (don't use if $FF)
		sta 	XSControl,x

		lda 	(codePtr),y 					; , follows ?
		cmp 	#KWD_COMMA  					; we have three parameters
		bne 	_EUMNoScroll 
		iny 									; consume it
		jsr 	EvaluateTerm 					; third parameter
_EUMNoScroll:
		dex 									; restore stack position.
		dex		
		jsr 	ERRCheckRParen 					; check closing )

		.DoSendMessage 							; read the mouse state
		.byte 	11,3 					
		.DoWaitMessage

		phx 									; save stack pos
		phy 									; save code pos

		ldy 	#0 								; control parameter 0,1
		jsr 	EMOCopyToVariable 				; copy to first

		ldy 	#2 								; control parameter 2,3
		inx  									; copy to second
		jsr 	EMOCopyToVariable

		inx 									; check third reference exists
		lda 	XSControl,x
		cmp 	#$FF
		beq 	_EUMNoScroll2
		stz 	ControlParameters+6 			; it's in 5, make it 16 bit value
		ldy 	#5  							; copy to third
		jsr 	EMOCopyToVariable
_EUMNoScroll2:
		ply
		plx 									; restore stack pos.
		lda 	ControlParameters+4 			; get the button bits.
		jmp 	EXPUnaryReturnA  				; and return that value.

EMOCopyToVariable:
		lda 	XSControl,x						; check integer variable reference
		and 	#(XS_TYPEBIT|XS_ISVARIABLE)
		cmp 	#XS_ISVARIABLE
		bne 	_EMOCTJError

		lda 	XSNumber0,x 					; make zTemp0 point to the variable
		sta 	zTemp0
		lda 	XSNumber1,x
		sta 	zTemp0+1

		phy 									; copy the value over
		lda 	ControlParameters,y
		sta 	(zTemp0)
		lda 	ControlParameters+1,y
		ldy 	#1
		sta 	(zTemp0),y
		ply

		rts

_EMOCTJError:
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
;
; ************************************************************************************************

