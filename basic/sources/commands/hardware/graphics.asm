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

Command_Text: 	;; [text]
Command_Move:	;; [move]
Command_Line:	;; [line]
Command_Rect: 	;; [rect]
Command_Ellipse: ;; [ellipse]
Command_Plot: 	;; [plot]
Command_Image:  ;; [image]

		dey 								; point at the original coordinates
GCommandLoop:
		lda 	(codePtr),y
		cmp 	#KWD_SYS_END 				; end of line or colon, exit
		beq 	_GCExit
		cmp 	#KWD_COLON
		beq 	_GCExit
		;
		iny 								; consume
		cmp 	#KWD_FROM 					; FROM is syntactic sugar
		beq 	GCommandLoop
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
		beq 	_GChangeMode
		inx
		cmp 	#KWD_PLOT
		beq 	_GChangeMode
		inx
		cmp 	#KWD_TEXT
		beq 	_GChangeModeText
		inx
		cmp 	#KWD_IMAGE
		beq 	_GChangeModeImage
		dey 								; unconsume
		bra 	_GNotMode

_GChangeModeImage:
		phx
		stz 	graphicsFlip 				; default flip
		ldx 	#0
		jsr 	EXPEvalInteger8 			; the image number
		sta 	graphicsImageID 			; save ID
		lda 	(codePtr),y 				; followed by comma ?
		cmp 	#KWD_COMMA
		bne 	_GCMINotComma
		iny 								; consume comma
		jsr 	EXPEvalInteger8 			; get flip
		sta 	graphicsFlip 		
_GCMINotComma:		
		jsr 	GCSendDrawingInfo 			; update flip
		plx 
		bra 	_GChangeMode
_GChangeModeText: 							; switch mode to text
		phx
		ldx 	#0
		jsr 	EXPEvalString 				; get string to print out
		lda 	XSNumber0
		sta 	graphicsText
		lda 	XSNumber1
		sta 	graphicsText+1
		plx

_GChangeMode:
		stx		graphicsCurrent 			; switch mode to MOVE, LINE, RECT, ELLIPSE, PLOT
		bra 	GCommandLoop
_GCExit:
		rts		
		;
_GNotMode:
		jsr 	GraphicsCommon 				; check TO_x,y BY_x,y INK_c INK_a,x SOLID and FRAME
		bcc 	_GMoveOnly 					; not recognised, check if coordinate move only
		cmp 	#0 							; do we print (e.g. was it TO or BY)
		beq 	GCommandLoop 				; no
		;
		lda 	graphicsPosX 				; old coordinates 4-7
		sta 	ControlParameters+4
		lda 	graphicsPosX+1
		sta 	ControlParameters+5
		lda 	graphicsPosY
		sta 	ControlParameters+6
		lda 	graphicsPosY+1
		sta 	ControlParameters+7
		;
		jsr 	_GCopyCoordinates
		;
		lda 	graphicsCurrent 			; are we in text
		cmp 	#6
		bne 	_GNotText
		lda 	graphicsText 				; copy address into slot 4.
		sta 	ControlParameters+4
		lda 	graphicsText+1
		sta 	ControlParameters+5
_GNotText:
		lda 	graphicsCurrent 			; are we in image
		cmp 	#7
		bne 	_GNotImage
		lda 	graphicsImageID 			; copy image ID into slot 4.
		sta 	ControlParameters+4		
_GNotImage:		
		DoWaitMessage 						; wait till hardware free
		lda	 	graphicsCurrent 			; current action.
		cmp 	#1 							; if move, do nothing
		beq 	_GCommandLoop2
		sta 	ControlFunction 			
		lda 	#5
		sta 	ControlCommand_Check		; do command 5 (graphics)
		DoWaitMessage 		
_GCommandLoop2:		
		jmp 	GCommandLoop

_GMoveOnly:
		dey
		jsr 	GCGetCoordinates 			; get new coordinates
		jsr 	_GCopyCoordinates 			; update them
		jmp 	GCommandLoop 				; and go back.

_GCopyCoordinates:
		lda 	XSNumber0 					; copy new coordinates into 0-3
		sta 	ControlParameters+0 		; also update last position.
		sta 	graphicsPosX
		lda 	XSNumber1
		sta 	ControlParameters+1
		sta 	graphicsPosX+1
		lda 	XSNumber0+1
		sta 	ControlParameters+2
		sta 	graphicsPosY
		lda 	XSNumber1+1
		sta 	ControlParameters+3
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
		cmp 	#KWD_DIM & $FF
		beq 	_GCDim
		dey 								; point back to shift and exit as unknown.
_GCUnknown:
		clc
		rts		
		;
		;		Handle DIM
		;		
_GCDim:	
		iny 								; consume DIM
		ldx 	#0
		jsr 	EXPEvalInteger8 			; get size
		sta 	graphicsSize 				; update it.
		jsr 	GCSendDrawingInfo
		lda 	#0 							; do not execute.
		sec
		rts
		;
		;		Handle SOLID and FRAME
		;
_GCSolidFrame:
		stx 	graphicsSolidMode 			; get mode to X for frame/solid
		iny 								; consume 2nd half of token.
		jsr 	GCSendDrawingInfo
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
		jsr 	GCSendDrawingInfo
		sec  								; found but don't draw.
		lda 	#0
		rts
		;
		;		If Ink, Size, solid mode changed notify the co processor
		;
GCSendDrawingInfo:
		lda 	inkAndByte
		sta 	ControlParameters+0
		lda 	inkXorByte
		sta 	ControlParameters+1
		lda 	graphicsSolidMode
		sta 	ControlParameters+2
		lda 	graphicsSize
		sta 	ControlParameters+3
		lda 	graphicsFlip
		sta 	ControlParameters+4
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
		stz 	graphicsText 				; and selected text.
		stz 	graphicsText+1
		stz 	graphicsImageID 			; image ID
		stz 	graphicsFlip 				; flip
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
;		15/01/24 		Added FROM syntactic sugar really.
;
; ************************************************************************************************

