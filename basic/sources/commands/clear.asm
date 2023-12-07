; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		clear.asm
;		Purpose:	Clear variables / general reset
;		Created:	3rd December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;										CLR Command
;
; ************************************************************************************************

		.section code

Command_CLEAR:	;; [clear]
		jmp 	ClearCode

; ************************************************************************************************
;
;									Do CLEAR functionality
;
; ************************************************************************************************

ClearCode:
		;
		;		Reset memory and variables
		;
		jsr 	ClearResetFreeMemory
		jsr 	ClearVariables
		;
		;		Reset stack
		;
		lda 	#HIGHMEMORY >> 8 			; reset the stack.
		jsr 	StackReset 			
		;
		;		Initialise string usage.
		;
		jsr 	StringSystemInitialise 		
		;
		;		Scan for procedures
		;
;		jsr 	ScanProcedures
		;
		;		Reset READ/DATA
		;
;		jsr 	Command_RESTORE
		rts

; ************************************************************************************************
;
;									Reset free memory pointer
;
; ************************************************************************************************

ClearResetFreeMemory:
		lda 	Program 					; number of pages of code.
		clc
		adc 	#(Program >> 8) 			; set zTemp0 to start of program
		sta 	zTemp0+1
		stz 	zTemp0
_CCFindEnd:
		lda 	(zTemp0) 					; found end ?
		beq 	_CCFoundEnd
		clc
		adc 	zTemp0
		sta 	zTemp0
		bcc 	_CCFindEnd
		inc 	zTemp0+1
		bra 	_CCFindEnd
_CCFoundEnd:
		clc 								; space for allocating memory after program.
		lda 	zTemp0
		adc 	#8
		sta 	freeMemory
		lda 	zTemp0+1
		adc 	#0
		sta 	freeMemory+1
		rts

; ************************************************************************************************
;
;										Clear all variables
;
; ************************************************************************************************

ClearVariables:
		lda 	#Program >> 8 				; start of identifier link
		sta 	zTemp0+1
		lda 	#1	
		sta 	zTemp0
_CVLoop:
		lda 	(zTemp0) 					; check complete
		beq 	_CVExit

		ldy 	#1 							; zero the variable values.
		lda 	#0
		sta 	(zTemp0),y
		iny
		sta 	(zTemp0),y
		iny
		sta 	(zTemp0),y
		iny
		sta 	(zTemp0),y

		iny 								; set the type to integer or string.
		lda 	(zTemp0),y
		and 	#$80
		sta 	(zTemp0),y

		clc
		lda 	(zTemp0)
		adc 	zTemp0
		sta 	zTemp0
		bcc 	_CVLoop
		inc 	zTemp0+1
		bra 	_CVLoop
_CVExit:
		rts				

		.send 	code

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

