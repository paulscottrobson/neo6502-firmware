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
		;		Reset variable memory pointer
		;
;		jsr 	PGMEndProgram 				; end program => zTemp0
;		;
;		stz 	freeMemory 					; start on next free page
;		lda 	zTemp0+1 					; for variables.
;		inc 	a
;		sta 	freeMemory+1
		;
		;		Reset stack
		;
;		lda 	PGMEndMemoryHigh
;		jsr 	StackReset 					; page passed on in A
		;
		;		Initialise string usage.
		;
;		jsr 	StringSystemInitialise 		
		;
		;		Scan for procedures
		;
;		jsr 	ScanProcedures
		;
		;		Reset READ/DATA
		;
;		jsr 	Command_RESTORE
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

