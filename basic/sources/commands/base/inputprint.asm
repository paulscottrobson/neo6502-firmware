; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		inputprint.asm 
;		Purpose:	Input (from keyboard) Print (to Screen)
;		Created:	5th December 2023
;		Reviewed:	5No
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
		bcs 	_CPNotInput 				
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
		ldx 	#0 							; slot 0
		jsr 	EvaluateTerm 				; term, which is the variable to assign to in slot 0.
_CPReInput:		
		jsr 	InputLine 					; input line
		;
		lda 	#inputBuffer & $FF 			; store in level 1, also param buffer for conversion.
		sta 	XSNumber0+1
		sta 	ControlParameters+4
		lda 	#inputBuffer >> 8
		sta 	XSNumber1+1
		sta 	ControlParameters+5
		lda 	#XS_ISSTRING
		sta 	XSControl+1
		;
		lda 	XSControl 					; are we expecting a string e.g. input name$
		bmi 	_CPDoAssign
		ldx 	#1
		lda 	#33
		jsr 	DoMathCommand
		lda 	ControlError 				; error occurred, reget
		beq 	_CPDoAssign
		lda 	#'?'
		jsr 	CPPrintA
		jsr 	CPPrintA
		bra 	_CPReInput
		
_CPDoAssign:
		ldx 	#0 							; do the assignment
		jsr 	AssignValueToReference
		rts

; ************************************************************************************************
;
;									Input a string to buffer
;
; ************************************************************************************************

InputLine:	
		stz 	inputBuffer 				; empty input buffer
_InputLoop:
		jsr 	CPInputA 					; get character
		cmp 	#13
		beq 	_InputExit
		cmp 	#8
		beq 	_InputDelete
		cmp 	#32
		bcc 	_InputLoop
		ldx 	inputBuffer					; new character
		cpx 	#80 						; too many ?
		beq 	_InputLoop
		sta 	inputBuffer+1,x 			; add and echo
		inc 	inputBuffer		
		jsr 	CPPrintA
		bra 	_InputLoop
_InputDelete: 								; backspace
		ldx 	inputBuffer 				; start of input ?
		beq 	_InputLoop		
		dec 	inputBuffer 				; backspace and echo out.
		jsr 	CPPrintA
		bra 	_InputLoop
_InputExit: 
		jmp 	CPPrintA					; return and exit

; ************************************************************************************************
;
;								Convert number to string at YA
;
; ************************************************************************************************

CPNumberToString:
		lda 	#tempBuffer & $FF  			; set up pointer.
		sta 	ControlParameters+4		
		pha
		lda 	#tempBuffer >> 8
		sta 	ControlParameters+5
		pha
		lda 	#34 						; convert number to string
		jsr 	DoMathCommand		
		ply
		pla
		rts

; ************************************************************************************************
;
;					Print string at YA on output device, return chars printed
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
		lda 	(zTemp0)
		rts		

; ************************************************************************************************
;
;								Input/Print vectors (potentially !)
;
; ************************************************************************************************

CPInputA:
		jmp 	ReadCharacter
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
