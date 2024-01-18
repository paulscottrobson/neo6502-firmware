; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		spritexy.asm
;		Purpose:	Sprite position retrieval.
;		Created:	9th January 2024
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;									Check sprite collision
;
; ************************************************************************************************

		.section code	

EXPUnarySpriteX: ;; [spritex(]
		lda 	#1  						; offset to use.
		bra	 	EXPUnarySpriteGet
EXPUnarySpriteY: ;; [spritey(]
		lda 	#3

EXPUnarySpriteGet:
		pha 								; save index to get from
		jsr 	EXPEvalInteger8 			; sprite # to read from
		sta 	ControlParameters 			; save as parameter[0]
		jsr 	ERRCheckRParen 				; )
		;
		.DoSendMessage 						; spriteread coomand.
		.byte 	6,5
		.DoWaitMessage		
		lda 	ControlError 				; error ?
		bne 	_EUSError

		pla 								; restore index
		phy 								; save code position
		tay 								; Y now points to data to copy

		lda 	ControlParameters,y 		; copy 2 bytes
		sta 	XSNumber0,x
		lda 	ControlParameters+1,y
		sta 	XSNumber1,x
		and 	#$80 						; sign extend it
		beq 	_EUSSX
		lda 	#$FF
_EUSSX:	sta 	XSNumber2,x 				; fill in the return values.
		sta 	XSNumber3,x
		stz 	XSControl,x		

		ply 								; restore code position
		rts

_EUSError:
		.error_range

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

