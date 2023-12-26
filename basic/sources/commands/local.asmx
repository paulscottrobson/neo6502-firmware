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
		lda 	NSStatus,x
		and 	#NSBIsReference 			; check it is a reference, must be a variable.
		beq		_LNTError
		;
		lda 	NSMantissa0,x 				; copy address of variable to zTemp0
		sta 	zTemp0
		lda 	NSMantissa1,x
		sta  	zTemp0+1
		;
		lda 	NSStatus,x 					; figure out what it is.
		and 	#NSBIsString
		bne 	_LNTPushString
		;
		;		Push number onto BASIC stack.
		;
		phy
		ldy 	#0 							; push 0 to 4 inclusive, the number values, on the stack, and zero them as you go.
_LNTPushNumLoop:
		lda		(zTemp0),y
		jsr 	StackPushByte
;		lda 	#0
;		sta 	(zTemp0),y
		iny
		cpy 	#5			
		bne 	_LNTPushNumLoop
		;
		lda 	zTemp0 						; push the actual target address to write on the stack
		jsr 	StackPushByte
		lda 	zTemp0+1
		jsr 	StackPushByte
		;
		lda 	#STK_LOCALN 				; push local-number marker.
		jsr 	StackPushByte
		ply
		rts
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
		ldy 	#0 							; output string
		cmp 	#0 							; if not assigned string
		beq 	_LNTStringOut
_LNTPushStrLoop:		
		lda 	(zTemp1),y
		beq 	_LNTStringOut
		jsr 	StackPushByte
		iny
		bra 	_LNTPushStrLoop
_LNTStringOut:
		tya									; output length (chars written).
		jsr 	StackPushByte
		;
;		lda 	#0 							; clear original string (currently disabled).
;		sta 	(zTemp1)
		;
		lda 	NSMantissa0,x 				; output address of the string record *not* the string itself
		jsr 	StackPushByte
		lda 	NSMantissa1,x
		jsr 	StackPushByte

		lda 	#STK_LOCALS 				; push local-string marker.
		jsr 	StackPushByte

		ply
		rts

_LNTError:
		jmp 	SyntaxError

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
		ldy 	#4 							; copy back
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
		; 	CAN POP NULL STRING HERE.
		;
		jsr 	StackPopByte 				; address of record copied to zTemp0
		sta 	zTemp0+1
		jsr 	StackPopByte
		sta 	zTemp0
		;
		phy
		;
		lda 	(zTemp0) 					; address to write string to copied to zTemp1
		sta 	zTemp1
		ldy 	#1
		lda 	(zTemp0),y
		sta 	zTemp1+1
		;
		jsr 	StackPopByte 				; # chars to get => y
		tay		

		lda 	zTemp1+1 					; if no target (e.g. was "" originally) exit
		beq 	_LPVStringCopied

		lda 	#0 							; NULL on end
		sta 	(zTemp1),y
_LPVStringCopy: 							; copy string out to target address (zTemp1)
		dey
		bmi 	_LPVStringCopied		
		jsr 	StackPopByte
		sta 	(zTemp1),y
		bra 	_LPVStringCopy
_LPVStringCopied:
		;
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
