; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		editinit.asm
;		Purpose:	Editor
;		Created:	29th March 2024
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;										Initialise Editor
;
; ************************************************************************************************

		.section code

EDInitialise:
		jsr 	EDUFindStart 				; find the start to zTemp0
		stz 	ControlParameters 			; scan forward and renumber
		stz 	ControlParameters+1
_EDRNLoop:
		lda 	(zTemp0) 					; end of code
		beq 	_EDRNExit 					; completed		
		inc 	ControlParameters 			; pre-increment, not only line# but line count.
		bne 	_EDRNNoCarry
		inc 	ControlParameters+1
_EDRNNoCarry:		
		ldy 	#1 							; forcibly renumber
		lda 	ControlParameters
		sta 	(zTemp0),y
		lda 	ControlParameters+1
		iny
		sta 	(zTemp0),y
		jsr 	EDUNext 					; advance to next
		bra 	_EDRNLoop
_EDRNExit:
		rts

; ************************************************************************************************
;
;							Find the start of non library code
;
; ************************************************************************************************

EDUFindStart:
		lda 	Program 					; back to the program start, get the count of var pages.
		clc 								; make an actual address.
		adc 	#(Program >> 8)
		sta 	zTemp0+1
		stz 	zTemp0

_EDUSkipZero:
		lda 	(zTemp0) 					; end ?
		beq 	_EDUFSExit
		ldy 	#1 							; line number zero
		lda 	(zTemp0),y 					; e.g. library code which we don't edit		
		iny
		ora 	(zTemp0),y
		bne 	_EDUFSExit 					; found actual code.
		jsr 	EDUNext
		bra 	_EDUSkipZero
_EDUFSExit:
		rts

; ************************************************************************************************
;
;									Advance zTemp0 to the next line
;
; ************************************************************************************************

EDUNext:
		clc
		lda 	(zTemp0)
		adc 	zTemp0
		sta 	zTemp0
		bcc 	_EDUNExit
		inc 	zTemp0+1
_EDUNExit:	
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

