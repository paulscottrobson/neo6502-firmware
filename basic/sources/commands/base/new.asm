; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		new.asm
;		Purpose:	New program
;		Created:	3rd December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;										NEW Command
;
; ************************************************************************************************

		.section code

Command_NEW:	;; [new]
		jsr 	NewProgram
		jmp 	Command_END 				; Run END

; ************************************************************************************************
;
;			Execute a New - checking for library. Program basic structure now established
; 			at Cold Start.
;
; ************************************************************************************************
		
NewProgram:		
		clc
		lda 	Program  					; access the start of the program part
		adc 	#Program >> 8
		sta 	codePtr+1
		stz 	codePtr

		lda 	(codePtr) 					; any code at all
		beq 	_CNNewClear
		ldy		#1 							; is there a library present ?
		lda 	(codePtr),y 				; e.g. first line # is 0
		iny
		ora 	(codePtr),y
		bne 	_CNNewClear  				; no.
		;
		;		Partial new ; truncate program, leave variables.
		;
		jsr 	SkipZeroCode 				; figure out where the library ends		
		lda 	#0  						; and kill it at that point.
		sta 	(codeptr)
		jsr 	ClearCode 					; and run Clear
		rts

_CNNewClear:
		jsr 	NewColdStart
		jsr 	ClearCode 					; Run CLR
		rts		

; ************************************************************************************************
;
;							Cold Start New - wipes everything
;
; ************************************************************************************************

NewColdStart:
		lda 	#1 							; 1 page of identifiers
		sta 	Program
		stz 	Program+1 					; End of current identifier list.
		stz 	Program+256 				; Erase current program

		lda 	#"A" 						; create required variables A O P X Y
		jsr 	_CNVariable
		lda 	#"O"
		jsr 	_CNVariable
		lda 	#"P"
		jsr 	_CNVariable
		lda 	#"X"
		jsr 	_CNVariable
		lda 	#"Y"
		jsr 	_CNVariable
		rts
;
;		Create new variable called 'A' (single character name)
;
_CNVariable:
		ora 	#$80 						; in the correct format, bit 7 of last character set :)
		sta 	tokElement+1
		lda 	#1
		sta 	tokElement
		jsr 	TOKCreateIdentifier
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
;		18-03-24 		New checks for library.
;
; ************************************************************************************************

