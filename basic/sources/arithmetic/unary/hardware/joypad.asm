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
;		JOYPAD(dx,dy) : returns A (bit 0) B (bit 1) X(bit 2) Y(bit 3) and -1,0,1 in dx,dy variables
;		JOYPAD(index,dx,dy) : same for specific joypad
;
; ************************************************************************************************

		.section code	

EXPUnaryJoypad: ;; [joypad(]
		jsr 	EvaluateTerm 					; first parameter.
		jsr 	ERRCheckComma
		inx
		jsr 	EvaluateTerm 					; second parameter.
		dex
		lda 	(codePtr),y 					; , follows ?
		cmp 	#KWD_COMMA  					; we have three parameters
		beq 	_EUJIndividual
		jsr 	ERRCheckRParen 					; check closing )
		;
		;		General interrogation function, old API.
		;
		.DoSendMessage 							; read the joypad
		.byte 	7,1
		.DoWaitMessage

		jsr 	EUJCopyDirectionData 			; copy the UDLR data

_EUJReturnButtons:		
		lda 	ControlParameters 				; get the bits.
		lsr 	a 								; drop the lower 4 bits and shift AB into place
		lsr 	a
		lsr 	a
		lsr 	a
		jmp 	EXPUnaryReturnA
		;
		;		Handle specific interrogation of a joypad new API
		;
_EUJIndividual:
		iny 									; skip comma
		inx  									; get the third parameter
		inx 
		jsr 	EvaluateTerm 					; second parameter.
		dex
		dex
		jsr 	ERRCheckRParen 					; check )		
		;
		jsr 	DereferenceTOS					; dereference the joypad #
		bit 	XSControl,x	 					; fail if string
		bmi 	_EUJRange
		lda 	XSNumber1,x 					; check it's 8 bit
		ora 	XSNumber2,x
		ora 	XSNumber3,x
		bne 	_EUJRange

		.DoSendMessage 							; read the joypad count
		.byte 	7,2
		.DoWaitMessage
		
		lda 	XSNumber0,x 					; joypad # required
		cmp 	ControlParameters 				; fail if > the count
		beq 	_EUJIndexOkay
		bcc 	_EUJIndexOkay
_EUJRange:		
		.error_range
		;
_EUJIndexOkay:
		lda 	XSNumber0,x 					; joypad to read.
		sta 	ControlParameters
		.DoSendMessage 							; read the specific joypad
		.byte 	7,3
		.DoWaitMessage
		;
		inx  									; copy the direction data out. 	
		jsr 	EUJCopyDirectionData
		dex
		bra 	_EUJReturnButtons 				; return the buttons, currently 8 bit.
		;
		;		Copy directional data at ControlParameters to stack, stack+1
		;
EUJCopyDirectionData:
		lda 	ControlParameters 				; copy left/right to variable
		jsr 	EUJCopyToVariable 				
		lda 	ControlParameters 				; copy up/down to variable
		inx
		lsr 	a
		lsr 	a
		jsr 	EUJCopyToVariable 				
		dex
		rts

EUJCopyToVariable:
		pha 									; save status.
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

