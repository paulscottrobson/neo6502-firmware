; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		run.asm
;		Purpose:	Run Program
;		Created:	3rd December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************

; ************************************************************************************************

; ************************************************************************************************
;
;										RUN Command
;
; ************************************************************************************************

		.section code

Command_RUN:	;; [run]
		lda 	(codePtr),y 				; RUN something ?
		cmp 	#KWD_SYS_END
		beq 	Command_RUN_Always
		cmp 	#KWD_COLON
		beq 	Command_RUN_Always
		jsr 	LoadCode 					; load the program, then run it.

Command_RUN_Always:
		jsr 	ClearCode					; clear everything out.
		lda 	Program 					; back to the program start, get the count of var pages.
		clc 								; make an actual address.
		adc 	#(Program >> 8)
		sta 	codePtr+1
		stz 	codePtr
		bra 	RUNNewLine

		; ----------------------------------------------------------------------------------------
		;
		;								End of current line
		;
		; ----------------------------------------------------------------------------------------

RUNEndOfLine: ;; [!!end]
		clc 								; advance to next line.
		lda 	(codePtr)
		adc 	codePtr
		sta 	codePtr
		bcc 	_RELNoCarry
		inc 	codePtr+1
_RELNoCarry:		

		; ----------------------------------------------------------------------------------------
		;
		;									New line
		;
		; ----------------------------------------------------------------------------------------

RUNNewLine:
		ldx 	#$FF 						; 6502 stack reset.
		txs
		lda 	(codePtr) 					; check off end of program
		beq 	Command_END
		ldy 	#1 							; copy error line#
		lda 	(codePtr),y 
		sta 	ERRLine
		iny
		lda 	(codePtr),y 
		sta 	ERRLine+1
		iny 								; offset into codePtr for start of line.

		; ----------------------------------------------------------------------------------------
		;
		;									New command
		;
		; ----------------------------------------------------------------------------------------

RUNNewCommand:		
		stz 	stringInitialised 			; reset string system flag.
		bit 	ControlStatus
		bmi 	_RUNBreak

_RNCNoCheck:		
		lda 	(codePtr),y 				; get next token
		bpl		_RUNNotToken 				; probably an identifier
		iny 								; consume token
		cmp 	#KWD_COLON 					; fast skip colon
		beq 	RUNNewCommand

		cmp 	#$B0 				 		; check unary function ?
		bcc 	_RUNSyntax

		asl 	a 							; double into X.
		tax
		jsr 	_RUNDispatchMain			; call the main dispatcher
		bra 	RUNNewCommand

_RUNDispatchMain:
		jmp 	(StandardVectorTable,x)
	
_RUNSyntax:
		.error_syntax
_RUNBreak:
		.error_break

		; ----------------------------------------------------------------------------------------
		;
		; 							Handle non-token commands - identifiers.
		;
		; ----------------------------------------------------------------------------------------

_RUNNotToken:	
		cmp 	#$20 						; error if not identifier
		bcs 	_RUNSyntax
		jsr 	Command_Let 				; do a LET implied	
		bra 	RUNNewCommand

; ************************************************************************************************
;
;										  End program
;
; ************************************************************************************************

Command_END: ;; [end]
		jmp 	WarmStart

; ************************************************************************************************
;
;										Shifted commands
;
; ************************************************************************************************

Command_Shift1_Handler: ;; [!!sh1]
		lda 	(codePtr),y 				; get token shifted
		iny
		asl 	a 							; double into X
		tax
		jmp 	(ExtendedVectorTable,x) 	; and go there.

Command_Shift2_Handler: ;; [!!sh2]
		lda 	(codePtr),y 				; get token shifted
		cmp 	#$D0 						; D0 up are unary functions
		bcs 	_CS2Fail
		iny
		asl 	a 							; double into X
		tax
		jmp 	(AssemblerVectorTable,x) 	; and go there.
_CS2Fail:
		.error_syntax

; ************************************************************************************************
;
;						Advance the pointer (codePtr) over zero numbered lines.
;
; ************************************************************************************************

SkipZeroCode:
		lda 	(codePtr) 					; reached program end
		beq 	_SZCExit
		ldy 	#1 							; line number zero ?
		lda 	(codePtr),y
		iny
		ora 	(codePtr),y
		bne 	_SZCExit 					; if not, exit
		clc  								; next line.
		lda 	(codePtr)
		adc 	codePtr
		sta 	codePtr
		bcc 	SkipZeroCode
		inc 	codePtr+1
		bra 	SkipZeroCode
_SZCExit:
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
;		23-02-24 		Added RUN <x> functionality.
;
; ************************************************************************************************

