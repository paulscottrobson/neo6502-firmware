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
		lda 	#Program >> 8				; back to the program start
		clc
		adc 	Program
		sta 	dataPtr+1
		stz 	dataPtr
		lda 	#3 							; position start of line
		sta 	dataPos
		stz 	dataInStatement 			; not in statement
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

