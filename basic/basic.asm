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

boot:   jmp     ColdStart
		jmp     CheckSpeed
		
ColdStart:
		ldy 	#_CSMsg >> 8
		lda 	#_CSMsg & $FF
		jsr 	CPPrintYA 

		.if 	BASICBUILD==2
		jsr 	NewProgram
		jmp 	TestTokenising  
		.endif
		.if 	BASICBUILD==1
		jmp     Command_RUN
		.endif

		jsr 	NewProgram
		jmp 	WarmStart

_CSMsg:	.byte 	_CSMSgEnd-_CSMSg-1
		.text 	13,"NeoBasic",13,13
_CSMSgEnd:
		.send   code
		.include "_include.inc"
		.section code

		.align  256
Program:
		.if 	BASICBUILD==1
		.binary "build/tokenised.dat"
		.endif
		
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

