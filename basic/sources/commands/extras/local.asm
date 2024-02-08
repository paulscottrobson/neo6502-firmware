; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		local.asm
;		Purpose:	LOCAL command
;		Created:	26th December 2023
;		Reviewed: 	No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;									Local - non array values
;
; ************************************************************************************************

		.section code

Command_LOCAL: ;; [local]
		ldx 	#0 							; at level 0
		jsr 	LocaliseNextTerm 			; convert term to a local.
		lda 	(codePtr),y 				; followed by comma ?
		iny
		cmp 	#KWD_COMMA
		beq 	Command_LOCAL 				; if so, localise another variable.
		dey 								; unpick pre-get
		rts

; ************************************************************************************************
;
;				Get a term reference and push its value on BASIC stack, using Stack[x]
;
; ************************************************************************************************

LocaliseNextTerm:		
		jsr 	EvaluateTerm 				; evaluate the term
LocaliseTermX:		
		lda 	XSControl,x 				; check it is a reference
		and 	#XS_ISVARIABLE
		beq		_LNTError
		;
		lda 	XSNumber0,x 				; copy address of variable to zTemp0
		sta 	zTemp0
		lda 	XSNumber1,x
		sta  	zTemp0+1
		;
		lda 	XSControl,x 				; figure out what it is.
		bmi 	_LNTPushString
		;
		;		Push number onto BASIC stack.
		;
		phy
		ldy 	#0 							; push 0 to 3 inclusive, the number values, on the stack
_LNTPushNumLoop:
		lda		(zTemp0),y
		jsr 	StackPushByte
		iny
		cpy 	#4			
		bne 	_LNTPushNumLoop
		;
		lda 	#STK_LOCALN 				; push local-number marker.
		bra 	_LNTPushTargetExit
		;
		;		Push string. Slightly different, as we push the string, then the length, but then we post the
		;	 	address of the variable record, not the string, as this might be updated with a larger concreted
		; 		string. It will fit the saved string in all circumstances.
		;
_LNTPushString:
		phy

		lda 	(zTemp0) 					; put address of string in zTemp1
		sta 	zTemp1
		ldy 	#1
		lda 	(zTemp0),y
		sta 	zTemp1+1				
		ora 	zTemp1 						; is that address zero, e.g. no value assigned
		beq 	_LNTOutStringTail 			; output string length zero.

		lda 	(zTemp1) 					; get the length
		beq 	_LNTOutStringTail 			; if zerok, output string length zero

		phx
		tax 								; use X as a counter, Y = 1
_LNTPushStrLoop:		
		lda 	(zTemp1),y
		jsr 	StackPushByte
		iny
		dex
		bne 	_LNTPushStrLoop
_LNTStringOut:
		plx
		lda 	(zTemp1) 					; get the length
_LNTOutStringTail:		
		jsr 	StackPushByte 				; write the length
		lda 	#STK_LOCALS 				; output the string type
		;
_LNTPushTargetExit:
		pha 								; save the end marker		
		lda 	XSNumber0,x 				; output address of the string record *not* the string itself
		jsr 	StackPushByte
		lda 	XSNumber1,x
		jsr 	StackPushByte

		pla 								; output end marker.
		jsr 	StackPushByte

		ply
		rts

_LNTError:
		.error_syntax

; ************************************************************************************************
;
;								Pop a local off the stack
;
; ************************************************************************************************

LocalPopValue:
		jsr 	StackPopByte
		cmp 	#STK_LOCALN 				; if not local-N
		bne 	_LPVString
		;
		;		Restore number off BASIC Stack
		;
		jsr 	StackPopByte 				; address
		sta 	zTemp0+1
		jsr 	StackPopByte
		sta 	zTemp0
		phy
		ldy 	#3 							; copy back
_LPVNumberCopy:
		jsr 	StackPopByte
		sta 	(zTemp0),y
		dey
		bpl 	_LPVNumberCopy
		ply 								; and complete		
		rts
		;
		;		Restore string
		;
_LPVString:
		;
		jsr 	StackPopByte 				; address of record copied to zTemp0
		sta 	zTemp0+1
		jsr 	StackPopByte
		sta 	zTemp0		
		;
		phx 								; use X as a counter
		phy 								; use Y as position
		;
		lda 	(zTemp0) 					; address to write string to copied to zTemp1
		sta 	zTemp1
		ldy 	#1
		lda 	(zTemp0),y
		sta 	zTemp1+1

		ora 	zTemp1 						; if uninitialised, leave it like that
		beq 	_LPVUninitialised

		jsr 	StackPopByte 				; # chars to get => y		
		sta 	(zTemp1) 					; write in length byte.
		cmp 	#0
		beq 	_LPVStringCopied 			; we're done if zero.
		tay 								; use as counter/pos

_LPVStringCopy: 							; copy string out to target address (zTemp1)
		jsr 	StackPopByte
		sta 	(zTemp1),y
		dey
		bne 	_LPVStringCopy
		bra 	_LPVStringCopied

_LPVUninitialised:
		jsr 	StackPopByte 				; throw the length.		

_LPVStringCopied:
		ply 								; restore X & Y
		plx		
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
