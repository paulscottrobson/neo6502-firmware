; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		graphics.asm
;		Purpose:	Chainable graphics commands.
;		Created:	30th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;									Multiple entry points
;
; ************************************************************************************************

		.section code

Command_Move:	;; [move]
Command_Line:	;; [line]
Command_Rect: 	;; [rect]
Command_Ellipse: ;; [ellipse]
		dey 								; point at the original coordinates
_GCommandLoop:
		lda 	(codePtr),y
		cmp 	#KWD_SYS_END 				; end of line or colon, exit
		beq 	_GCExit
		cmp 	#KWD_COLON
		beq 	_GCExit
		;
		ldx 	#1 							; can we change mode ?
		cmp 	#KWD_MOVE
		beq 	_GChangeMode
		inx
		cmp 	#KWD_LINE
		beq 	_GChangeMode
		inx
		cmp 	#KWD_RECT
		beq 	_GChangeMode
		inx
		cmp 	#KWD_ELLIPSE
		bne 	_GNotMode
_GChangeMode:
		stx		graphicsCurrent 			; switch mode to MOVE, LINE, RECT, ELLIPSE
		iny 								; consume token.
		bra 	_GCommandLoop
_GCExit:
		rts		
		;
_GNotMode:
		jsr 	GraphicsCommon 				; check TO_x,y BY_x,y INK_c INK_a,x SOLID and FRAME
		bcc 	_GMoveOnly 					; not recognised, check if coordinate move only
		cmp 	#0 							; do we print (e.g. was it TO or BY)
		beq 	_GCommandLoop 				; no
		;
		lda 	graphicsPosX 				; old coordinates 0-3
		sta 	ControlParameters+0
		lda 	graphicsPosX+1
		sta 	ControlParameters+1
		lda 	graphicsPosY
		sta 	ControlParameters+2
		lda 	graphicsPosY+1
		sta 	ControlParameters+3
		;
		jsr 	_GCopyCoordinates
		;
		DoWaitMessage 						; wait till hardware free
		lda	 	graphicsCurrent 			; current action.
		cmp 	#1 							; if move, do nothing
		beq 	_GCommandLoop
		sta 	ControlFunction 			
		lda 	#5
		sta 	ControlCommand_Check		; do command 5 (graphics)
		DoWaitMessage 		
		bra 	_GCommandLoop

_GMoveOnly:
		dey
		jsr 	GCGetCoordinates 			; get new coordinates
		jsr 	_GCopyCoordinates 			; update them
		bra 	_GCommandLoop 				; and go back.

_GCopyCoordinates:
		lda 	XSNumber0 					; copy new coordinates into 4-7
		sta 	ControlParameters+4 		; also update last position.
		sta 	graphicsPosX
		lda 	XSNumber1
		sta 	ControlParameters+5
		sta 	graphicsPosX+1
		lda 	XSNumber0+1
		sta 	ControlParameters+6
		sta 	graphicsPosY
		lda 	XSNumber1+1
		sta 	ControlParameters+7
		sta 	graphicsPosY+1
		rts

; ************************************************************************************************
;
;		 Process TO<c> BY<c> INK SOLID FRAME, return CS if processed, A#0 if should execute
;
; ************************************************************************************************

GraphicsCommon:
		cmp 	#KWD_TO 					; is it TO X,Y
		beq 	_GCTo 						
		cmp 	#KWD_SYS_SH1 				; BY INK SOLID FRAME are all shift 1s.
		bne 	_GCUnknown
		iny 								; get next
		lda 	(codePtr),y
		ldx 	#$FF
		cmp 	#KWD_SOLID & $FF
		beq 	_GCSolidFrame
		ldx 	#$00
		cmp 	#KWD_FRAME & $FF
		beq 	_GCSolidFrame
		cmp 	#KWD_INK & $FF
		beq 	_GCInk
		cmp 	#KWD_BY & $FF
		beq 	_GCBy
		dey 								; point back to shift and exit as unknown.
_GCUnknown:
		clc
		rts		
		;
		;		Handle SOLID and FRAME
		;
_GCSolidFrame:
		stx 	graphicsSolidMode 			; get mode to X for frame/solid
		iny 								; consume 2nd half of token.
		jsr 	_GCSendDrawingInfo
		lda 	#0 							; do not execute.
		sec
		rts
		;
		;		Handle TO and BY
		;
_GCBy:										; by code (offset from current)		
		jsr 	GCGetCoordinates 			; get new coordinates - offset.
		clc 	 							; add offset to old coordinates
		lda 	graphicsPosX
		adc 	XSNumber0
		sta 	XSNumber0
		lda 	graphicsPosX+1
		adc 	XSNumber1
		sta 	XSNumber1
		clc 	 		
		lda 	graphicsPosY
		adc 	XSNumber0+1
		sta 	XSNumber0+1
		lda 	graphicsPosY+1
		adc 	XSNumber1+1
		sta 	XSNumber1+1
		bra 	_GCExitDo
_GCTo:										; to code.	
		jsr 	GCGetCoordinates 			; get new coordinates
_GCExitDo:
		sec 								; exit successful and draw
		lda 	#$FF
		rts
		;
		;		Handle INK <col> and INK <and>,<xor>
		;
_GCInk:
		iny 								; consume INK
		stz 	inkAndByte 					; zero and
		ldx 	#0
		jsr 	EXPEvalInteger8 			; get the ink colour
		sta 	inkXorByte
		lda 	(codePtr),y 				; does a , follow
		cmp 	#KWD_COMMA
		bne 	_GCIExit
		iny 								; consume comma
		lda 	inkXorByte 					; Xor byte -> And byte
		sta 	inkAndByte
		jsr 	EXPEvalInteger8 			; get the xor value
		sta 	inkXorByte
_GCIExit:
		jsr 	_GCSendDrawingInfo
		sec  								; found but don't draw.
		lda 	#0
		rts
		;
		;		If Ink, Size, solid mode changed notify the co processor
		;
_GCSendDrawingInfo:
		lda 	inkAndByte
		sta 	ControlParameters+0
		lda 	inkXorByte
		sta 	ControlParameters+1
		lda 	graphicsSolidMode
		sta 	ControlParameters+2
		lda 	graphicsSize
		sta 	ControlParameters+3
		DoSendMessage
		.byte 	5,1
		DoWaitMessage
		rts

GCGetCoordinates:
		iny 								; consume TO or BY
		ldx 	#0
		jsr 	EXPEvalInteger16 			; x offset/pos
		jsr 	ERRCheckComma
		inx
		jsr 	EXPEvalInteger16 			; y offset/pos
		rts

; ************************************************************************************************
;
;									Reset graphics state
;
; ************************************************************************************************

GraphicsReset:
		stz 	graphicsPosX 				; position to 0,0
		stz 	graphicsPosX+1
		stz 	graphicsPosY
		stz 	graphicsPosY+1
		lda 	#1  						; move mode, size 1
		sta 	graphicsCurrent
		sta 	graphicsSize
		stz 	inkAndByte 					; colour draw white
		lda 	#7
		sta 	inkXorByte
		stz 	graphicsSolidMode 			; and frame mode.
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

