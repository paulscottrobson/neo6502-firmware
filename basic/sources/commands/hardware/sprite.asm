; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		sprite.asm
;		Purpose:	Chainable sprite commands
;		Created:	6th January 2024
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;									SPRITE command
;						IMAGE n FLIP n TO x,y BY x,y supported
;
; ************************************************************************************************

		.section code

Command_Sprite: ;; [sprite]
		ldx 	#spriteBlockSize-1 			; fill the sprite space with $FF
		lda 	#$80  						; these will force no change.
_CSReset:
		sta 	spriteID,x
		dex
		bpl 	_CSReset				

		lda 	(codePtr),y 				; check for CLEAR
		cmp 	#KWD_SYS_SH1
		bne 	CSNextSprite
		iny
		lda 	(codePtr),y 	
		cmp 	#KWD_CLEAR-$100
		bne 	CSNewSprite 				; this failing will cause a syntax error.
		iny
		.DoSendMessage 						; call code to CLEAR all sprites and layer
		.byte 	6,1
		.DoWaitMessage
		rts
		;
		;		End of command, or another SPRITE updates current
		;
CSNewSprite:
		jsr 	CSUpdateSprite		
		;
		;		Sprite outer loop, new sprite ID
		;	
CSNextSprite:		
		ldx 	#0	 						; this is the sprite #
		jsr 	EXPEvalInteger8
		sta 	spriteID 					; which sets the ID
		bpl 	CSSpriteLoopInner
		.error_range 						; bad sprite ID
		;
		;		Sprite inner loop.
		;
CSSpriteLoopInner:		
		lda 	(codePtr),y 				; check for end of statement
		cmp 	#KWD_COLON
		beq 	_CSExit
		cmp 	#KWD_SYS_END
		beq 	_CSExit
		iny 								; consume it.
		;
		cmp 	#KWD_TO 					; TO x,y
		beq 	_CSToPosition

		cmp 	#KWD_IMAGE 					; IMAGE x,y
		beq 	_CSWriteImage

		cmp 	#KWD_SPRITE 				; new SPRITE command
		beq 	CSNewSprite

		cmp 	#KWD_SYS_SH1 				; check for shift 1 (BY ANCHOR and FLIP)
		beq 	_CSIsShift1
_CSSyntax:
		.error_syntax		
_CSExit:
		jsr 	CSUpdateSprite 				; update sprite and exit.
		rts		
		;
		;		Handle Shift 1 (CLEAR, BY and FLIP)
		;
_CSIsShift1:		
		lda 	(codePtr),y 				; get the shifted thing & consume it
		iny

		cmp 	#KWD_BY-$100 				; go to the TO/BY code
		beq 	_CSToPosition
		ldx 	#spriteFlip-spriteID 		; single value 
		cmp 	#KWD_FLIP-$100
		beq 	_CSWriteValue
		ldx 	#spriteAnchor-spriteID
		cmp 	#KWD_ANCHOR-$100
		beq 	_CSWriteValue
		;
		cmp 	#KWD_HIDE-$100 				; hide is a seperate thing.
		bne 	_CSSyntax 					; failed all, syntax error.
		;
		;		Handle HIDE
		;
_CSHide:		
		lda 	spriteID 					; save parameters
		sta 	ControlParameters
		.DoSendMessage 						; call code to HIDE current sprite
		.byte 	6,3
		.DoWaitMessage
		rts
		; 
		;		Handle IMAGE
		;
_CSWriteImage:
		stz 	spriteFlip 					; IMAGE resets FLIP to 0
		ldx 	#0 							; get the value 
		jsr 	EXPEvalInteger8
		and 	#$7F   						; clear bit 7
		sta 	spriteImageSize 			; save image
		bra 	CSSpriteLoopInner
		;
		;		Handle FLIP (and IMAGE and any other single value options)
		;
_CSWriteValue:
		phx 								; save where it goes
		ldx 	#0 							; get the value 
		jsr 	EXPEvalInteger8
		bmi 	_CSValue
		plx 								; and write it
		sta 	spriteID,x
		bra 	CSSpriteLoopInner
		;
		;		TO/BY code (depends on token in A at entrance)
		;
_CSToPosition:		
		pha 								; save what we do with it.
		ldx 	#0 							; get new coordinates.
		jsr 	EXPEvalInteger16
		jsr 	ERRCheckComma
		inx
		jsr 	EXPEvalInteger16
		;
		pla 								; is it TO
		cmp 	#KWD_TO
		beq 	_CSCopyPosition
		;
		lda 	spriteX+1 					; it's BY, check we've done a TO otherwise it makes no sense.
		cmp 	#$80
		beq 	_CSValue
		;
		clc 								; add offset to current position.
		lda 	XSNumber0  					; note, not the sprite position but the last selected position.
		adc 	spriteX
		sta 	spriteX
		lda 	XSNumber1
		adc 	spriteX+1
		sta 	spriteX+1

		clc
		lda 	XSNumber0+1
		adc 	spriteY
		sta 	spriteY
		lda 	XSNumber1+1
		adc 	spriteY+1
		sta 	spriteY+1
		jmp 	CSSpriteLoopInner

_CSCopyPosition:
		lda 	XSNumber0  					; copy sprite position to update buffer.
		sta 	spriteX
		lda 	XSNumber1
		sta 	spriteX+1

		lda 	XSNumber0+1
		sta 	spriteY
		lda 	XSNumber1+1
		sta 	spriteY+1
		jmp 	CSSpriteLoopInner

_CSValue:
		.error_range		

CSUpdateSprite:
		ldx 	#spriteBlockSize-1 			; copy values to parameter buffer
_CSUS:	lda 	spriteID,x
		sta 	ControlParameters,x 		
		dex
		bpl 	_CSUS		
		.DoSendMessage 						; call code to Update current sprite
		.byte 	6,2
		.DoWaitMessage
		lda 	ControlError 				; did it fail ?
		bne 	_CSUSError
		rts
_CSUSError:
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
;		15-01-24 		Added support to pass ANCHOR.
;
; ************************************************************************************************

