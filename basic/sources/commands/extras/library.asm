; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		library.asm
;		Purpose:	"Librarisation" command
;		Created:	19th March 2024
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;										LIBRARY Command
;
; ************************************************************************************************

		.section code

Command_LIBRARY:	;; [library]
		lda 	(codePtr),y 				; end of line e.g. just "LIBRARY"
		cmp 	#KWD_COLON
		beq 	_CLRenumber
		cmp 	#KWD_SYS_END
		bne 	_CLLibrarise
_CLRenumber:
		jsr 	RenumberForce  				; forcibly renumber
_CLExit:		
		jmp 	WarmStart

_CLLibrarise:
		jsr 	LISTGetLinesToFrom  		; get range to librarise

		lda 	Program 					; point out code start.
		clc
		adc 	#Program >> 8
		sta 	codePtr+1
		stz 	codePtr
		;
_CLLoop:
		lda 	(codePtr) 					; finished
		beq 	_CLExit
		ldx 	#CLFrom-CLFrom 				; compare line number vs from
		jsr 	LISTCompareLine
		cmp 	#255 						; < from then skip
		beq 	_CLNext
		ldx 	#CLTo-CLFrom   				; compare line number vs IFR0
		jsr 	LISTCompareLine
		cmp 	#1 							; > to then skip
		beq 	_CLNext
		phy
		ldy 	#1  						; set line number to zero.
		lda 	#0
		sta 	(codePtr),y
		iny
		sta 	(codePtr),y
_CLNext:		
		clc 								; advance to next line.
		lda 	(codePtr)
		adc 	codePtr
		sta 	codePtr
		bcc 	_CLNoCarry
		inc 	codePtr+1
_CLNoCarry:		
		bra 	_CLLoop		

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

