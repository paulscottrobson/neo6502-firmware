; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		restore.asm
;		Purpose:	Restore Data Pointer to start
;		Created:	26th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;										RESTORE Command
;
; ************************************************************************************************

		.section code

Command_RESTORE:	;; [restore]
		jsr 	RestoreComplete
		cmp 	#KWD_COLON
		beq 	_CRExit
		cmp 	#KWD_SYS_END
		beq 	_CRExit
		ldx 	#0
		jsr 	EXPEvalInteger16 			; get line number to restore to
		phy

_CRSearch:
		lda 	(dataPtr) 					; end of program.
		beq 	_CRError
		ldy 	#1 							; found line #
		lda 	(dataPtr),y
		cmp 	XSNumber0
		bne 	_CRNext
		iny
		lda 	(dataPtr),y
		cmp 	XSNumber1
		bne 	_CRNext
		ply
_CRExit:		
		rts

_CRNext:		
		clc 								; advance to next line.
		lda 	(dataPtr)
		adc 	dataPtr
		sta 	dataPtr
		bcc 	_CRSearch
		inc 	dataPtr+1
		bra 	_CRSearch
		
_CRError:		
		.error_line


RestoreComplete:
		lda 	#Program >> 8				; back to the program start
		clc
		adc 	Program
		sta 	dataPtr+1
		stz 	dataPtr
		lda 	#3 							; position start of line
		sta 	dataPos
		stz 	dataInStatement 			; not in statement
		lda 	(codePtr),y 				; end of statement ?
		rts

; ************************************************************************************************
;
;										Swap Code and Data
;
; ************************************************************************************************

SwapCodeDataPointers:
		lda 	dataPtr 					; swap LSB of code/data
		ldx 	codePtr
		sta 	codePtr
		stx 	dataPtr

		lda 	dataPtr+1 					; swap MSB of code/data
		ldx 	codePtr+1
		sta 	codePtr+1
		stx 	dataPtr+1

		lda 	dataPos 					; swap dataPos and Y
		sty 	dataPos
		tay
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
;		17-01-24 		Missing CLC when calculating initial data search address.
;
; ************************************************************************************************

