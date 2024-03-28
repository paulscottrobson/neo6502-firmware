; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		joypad.asm
;		Purpose:	Return settings of default joypad
;		Created:	3rd January 2024
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;			JOYPAD(dx,dy) : returns A (bit 0) B (bit 1) and -1,0,1 in dx,dy variables
;
; ************************************************************************************************

		.section code	

EXPUnaryJoypad: ;; [joypad(]
		.DoSendMessage 							; read the joypad
		.byte 	7,1
		.DoWaitMessage
		lda 	ControlParameters 				; save result on stack twice
		pha
		pha
		jsr 	EUJCopyToVariable 				; copy left/right to variable
		jsr 	ERRCheckComma
		pla 									; get up/down bits
		lsr 	a
		lsr 	a
		jsr 	EUJCopyToVariable 				; copy to variable
		jsr 	ERRCheckRParen 					; )
		pla 									; get second joypad status
		lsr 	a 								; drop the lower 4 bits and shift AB into place
		lsr 	a
		lsr 	a
		lsr 	a
		jmp 	EXPUnaryReturnA

EUJCopyToVariable:
		pha 									; save status.
		jsr 	EvaluateTerm
		lda 	XSControl,x						; check integer variable
		and 	#(XS_TYPEBIT|XS_ISVARIABLE)
		cmp 	#XS_ISVARIABLE
		bne 	_EUCTJError
		;
		pla 									; get status of joypad
		and 	#3 								; only interested in lower 2 bits
		beq 	_EUCTVRetZero 					; zero if either or both pressed.
		cmp 	#3
		beq 	_EUCTVRetZero 			
		lsr 	a 								; left/up into carry
		lda 	#$FF 							; would be filled with -1
		bcs 	_EUCTVRetA
		jsr 	_EUCTVRetZero 					; return 1.
		lda 	#1
		sta 	(zTemp0)
		rts

_EUCTVRetZero:
		lda 	#0
_EUCTVRetA:		
		pha
		lda 	XSNumber0,x 					; make zTemp0 point to the variable
		sta 	zTemp0
		lda 	XSNumber1,x
		sta 	zTemp0+1
		pla
		
		phy
		ldy 	#3 								; fill int with A
_EUCTVRLoop:		
		sta 	(zTemp0),y
		dey
		bpl 	_EUCTVRLoop
		ldy 	#4 								; make type integer
		lda 	#0
		sta 	(zTemp0),y
		ply
		rts

_EUCTJError:
		.error_type

; ************************************************************************************************
;
;								JOYCOUNT() : returns attached joypads count
;
; ************************************************************************************************

EXPUnaryJoycount: ;; [joycount(]
		jsr 	ERRCheckRParen 					; )
		.DoSendMessage 							; read the joypad count
		.byte 	7,2
		.DoWaitMessage
		lda 	ControlParameters 				; read the controller count & return it.
		jmp 	EXPUnaryReturnA

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

