; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		sys.asm
;		Purpose:	Call machine code program
;		Created:	3rd December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

push16 	.macro
		lda 	\1
		pha
		lda 	1+\1
		pha
		.endm

pop16 	.macro
		pla
		sta 	1+\1
		pla
		sta 	\1
		.endm
				
; ************************************************************************************************
;
;										SYS Command
;
; ************************************************************************************************

		.section code

Command_Sys:	;; [sys]
		ldx 	#0
		jsr 	EXPEvalInteger16 			; execution address
		phy

		lda 	XSNumber0,x 				; copy address into subroutine call.
		sta 	_SysCall+1
		lda 	XSNumber1,x
		sta 	_SysCall+2

		push16	codePtr 					; save zero page BASIC variables.
		push16 	basicStack

		lda  	VariableA 					; load in AXY
		ldx 	VariableX
		ldy 	VariableY
		
_SysCall:
		jsr 	$FFFF 						; call the code

		pop16 	basicStack 					; restore important zero page BASIC variables
		pop16 	codePtr
		ply
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
;		03/03/24 		Amended to push important BASIC variables.
;
; ************************************************************************************************

