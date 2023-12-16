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
		.if 	BASICBUILD==2
		jsr 	NewProgram
		jmp 	TestTokenising  
		.endif
		.if 	BASICBUILD==1
		jmp     Command_RUN
		.endif

		jsr 	NewProgram
		jmp 	WarmStart

		.send   code
		.include "_include.inc"
		.section code

WarmStart:
		lda     #$00
		tax
		tay
		.byte   3
		bra     WarmStart

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

