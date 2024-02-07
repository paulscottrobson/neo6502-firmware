; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		delete.asm
;		Purpose:	Delete line range in program
;		Created:	7th February 2024
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;										DELETE Command
;
; ************************************************************************************************

		.section code

Command_Delete:	;; [delete]
		jsr 	LISTGetLinesToFrom 			; get range.
		lda 	Program 					; point out code start.
		clc
		adc 	#Program >> 8
		sta 	codePtr+1
		stz 	codePtr
		;
		;		Main loop
		;
_DLLoop:
		lda 	(codePtr) 					; finished
		beq 	_DLExit
		ldx 	#CLFrom-CLFrom 				; compare line number vs from
		jsr 	LISTCompareLine
		cmp 	#255 						; < from then skip
		beq 	_DLNext
		ldx 	#CLTo-CLFrom   				; compare line number vs IFR0
		jsr 	LISTCompareLine
		cmp 	#1 							; > to then skip
		beq 	_DLNext
		jsr 	DeleteCurrentLine 			; delete this line and loop round, retry the next line
		bra 	_DLLoop

_DLNext:		
		clc 								; advance to next line.
		lda 	(codePtr)
		adc 	codePtr
		sta 	codePtr
		bcc 	_DLNoCarry
		inc 	codePtr+1
_DLNoCarry:		
		bra 	_DLLoop		

_DLExit:
		jsr 	ClearCode 					; clear memory as deleting stuff
		jmp 	WarmStart 					; and warm start

DeleteCurrentLine:
		jsr 	ClearResetFreeMemory 		; end of program into zTemp0
		lda 	(codePtr) 					; memory chunk to delete into Y
		tay
		lda 	codePtr 					; address to delete at -> zTemp1
		sta 	zTemp1
		lda 	codePtr+1
		sta 	zTemp1+1
_DCLCopy:

		lda 	(zTemp1),y 					; delete a line
		sta 	(zTemp1)
		inc 	zTemp1   					; next byte
		bne 	_DCLCopy
		inc 	zTemp1+1

		lda 	zTemp1+1 					; very rough deleting to end, doesn't matter as we're going to CLEAR anyway
		cmp 	zTemp0+1
		bcc 	_DCLCopy
		beq 	_DCLCopy
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

