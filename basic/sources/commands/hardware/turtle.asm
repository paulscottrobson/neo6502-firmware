; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		turtle.asm
;		Purpose:	Turtle commands
;		Created:	26th January 2024
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section code

; ************************************************************************************************
;
;									Reset the turtle system
;
; ************************************************************************************************

TurtleResetSystem:	
		stz 	turtleInitialised
		lda 	#1
		sta 	turtlePenDown
		lda 	#6
		sta 	turtlePenColour
		stz 	turtleFast
		rts

; ************************************************************************************************
;
;						Check turtle initialised, if not initialise it
;
; ************************************************************************************************

TurtleCheckInitialised:
		lda 	turtleInitialised 			; already initialised ?
		bne 	_TCIExit
		inc 	turtleInitialised 			; set flag
		lda 	#$7F 						; sprite $7F
		sta 	ControlParameters+0
		.DoSendMessage 					 	; and reset it.
		.byte 	9,1
		.DoWaitMessage		
_TCIExit:
		rts		

; ************************************************************************************************
;
;								Turtle CLEAR|FAST|HIDE|SHOW
;
; ************************************************************************************************

CommandTurtle: ;; [turtle]
		jsr 	TurtleCheckInitialised 		; check the turtle has been initialised.
		lda 	(codePtr),y 				; all 3 are in the minor group
		iny
		cmp 	#KWD_SYS_SH1 
		bne 	_CTSyntax

		lda 	(codePtr),y 				; check what it is
		iny
		cmp 	#KWD_HOME-$100
		beq 	_CTClear
		cmp 	#KWD_FAST-$100
		beq 	_CTFast
		cmp 	#KWD_Hide-$100
		beq 	_CTHide
		cmp 	#KWD_SHOW-$100
		beq 	_CTShow
_CTSyntax:
		.error_syntax		

_CTClear:
		stz 	turtleInitialised 			; force reinitialisation
		jsr 	TurtleCheckInitialised 		; reinitialise it
_CTShow:		
		stz 	ControlParameters+0 		; rotate 0 to draw it
		stz 	ControlParameters+1
		.DoSendMessage 					 	
		.byte 	9,2
		.DoWaitMessage		
		rts

_CTHide:		
		.DoSendMessage 					 	; hide command
		.byte 	9,4
		.DoWaitMessage		
		rts

_CTFast:
		lda 	#1  						; move fast
		sta 	turtleFast
		rts		

; ************************************************************************************************
;
;									   Pen up
;
; ************************************************************************************************

CommandPenup: ;; [penup]
		stz 	turtlePenDown
		rts

; ************************************************************************************************
;
;									   Pen down
;
; ************************************************************************************************

CommandPendown: ;; [pendown]
		lda 	#$FF
		sta 	turtlePenDown
		lda 	(codePtr),y
		cmp 	#KWD_COLON
		beq 	_CPDNoColour
		cmp 	#KWD_SYS_END
		beq 	_CPDNoColour
		ldx 	#0
		jsr 	EXPEvalInteger8
		sta 	turtlePenColour
_CPDNoColour:
		rts		

; ************************************************************************************************
;
;									 Rotate Turtle
;
; ************************************************************************************************

CommandLeft: ;; [left]
		sec
		bra 	TurtleLRMain
CommandRight: ;; [right]
		clc
TurtleLRMain:
		php 								; save carry (set left, clear right)
		jsr 	TurtleCheckInitialised 		; initialise if not already initialised.
		ldx 	#0 							; get rotation value
		jsr 	EXPEvalInteger16 
		plp 								; left ? (e.g. negative)		
		bcc 	_TLMNotLeft
		jsr 	UnaryNegateInteger 			; if so, negate it
_TLMNotLeft:
		lda 	XSNumber0 					; copy to rotate
		sta 	ControlParameters+0
		lda 	XSNumber1
		sta 	ControlParameters+1
		.DoSendMessage 					 	; and rotate it
		.byte 	9,2
		.DoWaitMessage		
		jsr 	TurtleDelay
		rts

; ************************************************************************************************
;
;									 Forward Turtle
;
; ************************************************************************************************

CommandForward: ;; [forward]
		jsr 	TurtleCheckInitialised 		; initialise if not already initialised.
		ldx 	#0 							; get rotation value
		jsr 	EXPEvalInteger16 
		lda 	XSNumber0 					; move distance
		sta 	ControlParameters+0
		lda 	XSNumber1
		sta 	ControlParameters+1
		lda 	turtlePenColour  			; colour and status
		sta 	ControlParameters+2
		lda 	turtlePenDown
		sta 	ControlParameters+3

		.DoSendMessage 					 	; and move it
		.byte 	9,3
		.DoWaitMessage		
		jsr 	TurtleDelay
		jsr 	TurtleDelay
		rts

; ************************************************************************************************
;
;									 Delay so turtle can be seen
;
; ************************************************************************************************

TurtleDelay:
		phy
		lda 	turtleFast
		bne 	_TDExit
		lda 	#1
_TD0:	dey
		bne 	_TD0
		dex
		bne 	_TD0
		dec 	a
		bne 	_TD0
_TDExit:		
		ply
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
;		30-01-24 		TURTLE HOME replaces TURTLE CLEAR
;
; ************************************************************************************************

