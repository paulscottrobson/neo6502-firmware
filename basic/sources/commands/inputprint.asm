; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		inputprint.asm 
;		Purpose:	Input (from keyboard) Print (to Screen)
;		Created:	5th December 2024
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section code

; ************************************************************************************************
;
;									INPUT statement
;
; ************************************************************************************************

Command_Input: ;; [input]
		lda 	#$FF 						; this flag determines input ($FF) output $(00)
		sta 	InputFlag
		bra 	Command_IP_Main

; ************************************************************************************************
;
;									PRINT statement
;
; ************************************************************************************************

Command_Print:	;; [print]
		stz 	InputFlag
		;
Command_IP_Main:		
		clc 								; carry being clear means last print wasn't comma/semicolon
		;
		;		Main Input/Print Loop
		;
_CPLoop:
		php 								; save "last action" flag

		lda 	(codePtr),y 				; get next character
		cmp  	#KWD_SYS_END 				; end of line or colon, exit now.
		beq 	_CPExit 					; without consuming
		cmp 	#KWD_COLON
		beq 	_CPExit

		pla 								; throw last action flag		
		;
		;		Decide what's next
		;
		lda 	(codePtr),y 				; next character and consume
		iny
		cmp 	#KWD_SEMICOLON				; is it a semicolon (syntax seperator)
		beq 	_CPContinueWithSameLine
		cmp 	#KWD_COMMA 					; comma (tab)
		beq 	_CPTab
		dey 								; undo the get.
		;
		;		check for INPUT state and identifier
		;
		bit 	InputFlag 					; check for Input
		bpl	 	_CPNotInput
		and 	#$C0 						; check 40-7F e.g. an identifier.
		cmp 	#$40
		bne 	_CPNotInput 				
		jsr 	_CPInputCode 				; input code
		bra 	Command_IP_Main 			; and go round again.
		;
		;		Not input so it's print.
		;
_CPNotInput:		
		jsr 	EXPEvaluateExpressionAt0 	; evaluate expression.
		jsr 	DereferenceTOS
		bit 	XSControl,x 				; is it a number ?
		bpl 	_CPNumber
		;
		;		Print string
		;
		phy 
		clc 								; string address to YA
		lda 	XSNumber1,x
		tay 	
		lda 	XSNumber0,x
		jsr 	CPPrintYA 					; print YA
		ply
		clc
		bra 	Command_IP_Main 			; loop round clearing carry so NL if end		
		;
		;		Print number
		;
_CPNumber:
		phy
		jsr 	CPNumberToString 			; convert to string at YA 
		jsr 	CPPrintYA 					; print string at YA
		ply		
		bra 	Command_IP_Main				; loop round clearing carry so NL if end		
		;
		;		Comma, Semicolon, Tab come here.
		;
_CPTab:	
		lda 	#9 							; print TAB
		jsr 	CPPrintA
_CPContinueWithSameLine:		
		sec 								; loop round with carry set, which
		bra 	_CPLoop 					; will inhibit final CR
		;
		;		Exit
		;
_CPExit:
		plp 								; get last action flag
		bcs 	_CPExit2  					; carry set, last was semicolon or comma
		lda 	#13 						; print new line
		jsr 	CPPrintA
_CPExit2:		
		rts
		;
		;		Input code
		;
_CPInputCode:
;		jsr 	EXPTermR0 					; get the term, the thing being input to
;											; (type being identifier is checked above)
;		phy 								; save position
;		jsr 	CPInputA					; input a line to YX
;
;		lda 	IFR0+IFExponent 			; string ?
;		bmi 	_CPInputString
;		;
;		;		Number Input Code
;		;
;		lda 	IFR0+IFMantissa0 			; push target address on stack
;		pha
;		lda 	IFR0+IFMantissa1
;		pha
;		;
;		stx 	zTemp0 						; use VAL Code to convert.
;		sty 	zTemp0+1
;		jsr 	VALConversionZTemp0
;
;		pla 								; do the assign.
;		sta 	zTemp0+1
;		pla
;		sta 	zTemp0
;		jsr 	AssignNumber
;
;		ply
;		rts
;
		;
		;										String Input Code
		;

;_CPInputString:
;		lda 	IFR0+IFMantissa0 			; copy target address to zTemp0
;		sta 	zTemp0
;		lda 	IFR0+IFMantissa1
;		sta 	zTemp0+1
;		;
;		stx 	IFR0+IFMantissa0 			; string YX in result register
;		sty 	IFR0+IFMantissa1
;		jsr 	AssignString 				; assign the string
;		ply 								; exit
;		rts


; ************************************************************************************************
;
;								Convert number to string at YA
;
; ************************************************************************************************

CPNumberToString:
		lda 	#tempBuffer & $FF  			; set up pointer.
		sta 	ControlCommand+8		
		pha
		lda 	#tempBuffer >> 8
		sta 	ControlCommand+9
		pha
		lda 	#34 						; convert number to string
		jsr 	DoMathCommand		
		ply
		pla
		rts

; ************************************************************************************************
;
;							Print string at YA on output device
;
; ************************************************************************************************

CPPrintYA:
		sta 	zTemp0
		sty 	zTemp0+1
		lda 	(zTemp0)
		tax
		beq 	_CPPrintExit
		ldy 	#1
_CPPrintAYXLoop:
		lda 	(zTemp0),y
		jsr 	CPPrintA
		iny
		dex
		bne 	_CPPrintAYXLoop
_CPPrintExit:	
		rts		

; ************************************************************************************************
;
;								Input/Print vectors (potentially !)
;
; ************************************************************************************************

CPInputA:
		.byte 	3
CPPrintA:
		jmp 	WriteCharacter
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
