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
		lda 	#1 							; 1 page of identifiers
		sta 	Program
		stz 	Program+1 					; empty it.
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

		jsr 	ClearCode 					; Run CLR
		jmp 	Command_END 				; Run END
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
;
; ************************************************************************************************

