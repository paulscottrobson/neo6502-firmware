; ************************************************************************************************
; ************************************************************************************************
;
;       Name:       basic.asm
;       Purpose:    BASIC main program
;       Created:    25th November 2023
;       Reviewed:   No
;       Author:     Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.weak
BASICBUILD = 0 								; 0: Installable 1: Testmode 2: Tokenise test.
		.endweak

; ************************************************************************************************
;
;                                          Main Program
;
; ************************************************************************************************

		.section code

boot:   jmp     BColdStart 					; $800 cold start
		jmp     BWarmStart 					; $803 warm start
		jmp 	Command_RUN 				; $806 run program
		.byte 	Program & $FF,Program>>8,0 	; $809 address of Program base (var table)
		
BColdStart:
		ldy 	#_CSMsg >> 8
		lda 	#_CSMsg & $FF
		jsr 	CPPrintYA 

		.if 	BASICBUILD==2
		jsr 	NewProgram
		jmp 	TestTokenising  
		.endif

		jsr 	NewProgram
		jmp 	WarmStart

_CSMsg:	.byte 	_CSMSgEnd-_CSMSg-1
		.text 	13,"Welcome to NeoBasic",13,13
_CSMSgEnd:
		.send   code
		.include "_include.inc"
		.section code

BWarmStart:
		jsr 	ClearCode
		jmp 	WarmStart

		.align  256
Program:

		
		.send code


; ************************************************************************************************
;
;                                   Changes and Updates
;
; ************************************************************************************************
;
;       Date            Notes
;       ====            =====
;
; ************************************************************************************************

