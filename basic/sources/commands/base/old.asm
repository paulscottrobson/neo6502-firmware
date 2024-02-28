; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		old.asm
;		Purpose:	Un-New program
;		Created:	9th February 2024
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;										OLD Command
;
; ************************************************************************************************

		.section code

Command_Old:	;; [old]
		clc 								; start of code.
		lda 	#Program >> 8
		adc 	Program
		sta 	codePtr+1
		stz 	codePtr
		lda 	(codePtr) 					; is there a program here, i.e. an offset
		bne 	_COWarm 					; if so, exit
		ldy 	#3
_COLoop:
		lda 	(codePtr),y 				; end of line
		cmp 	#KWD_SYS_END
		beq 	_COEndLine
		jsr 	SkipOneInstruction 			
		cmp 	#4  						; looped round ?
		bcs 	_COLoop
		.error_syntax 						; give up
_COEndLine:
		iny 								; start of next line
		tya
		sta 	(codePtr) 					; store as first byte
_COWarm:		
		jmp 	WarmStart 					; and warm start				

		.send code
						
; ************************************************************************************************
;
;									Changes and Updates
;
; ************************************************************************************************
;
;		Date			Notes
;		==== 			=====
;		18/02/24 		Poor ; crashes with odd data, doesn't work if program is fine already.
;
; ************************************************************************************************

