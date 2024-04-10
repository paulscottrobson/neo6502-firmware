; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		opcodes.asm
;		Purpose:	Opcode handling code
;		Created:	9th April 2024
;		Reviewed: 	No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section 	code

; ************************************************************************************************
;
;										   Support Macros
;
; ************************************************************************************************

;
;		Single register operand
;
sweetreg .macro
	jsr 	SweetAsmGetRegister
	ora 	#\1
	jsr 	AssemblerWriteByte
	.endm
;
;		Single byte opcodes.
;
sweetopc .macro
	lda 	#\1
	jsr 	AssemblerWriteByte
	.endm
;
;		Short branch like 65C02
;
sweetbra .macro
	lda 	#\1
	bra 	ShortBranchCommon
	.endm
;
;		Check @ follows. May soften ?
;
checkat .macro
	lda 	#KWD_AT
	jsr 	ERRCheckA
	.endm

; ************************************************************************************************
;
;										Standard dispatcher
;
; ************************************************************************************************

; ************************************************************************************************
;										SET n,<constant>
; ************************************************************************************************

SweetAsm_SET: ;; [SET]
	.sweetreg $10 							; $10+x
	jsr 	ERRCheckComma
	ldx 	#0
	jsr 	EXPEvalInteger16 				; operand
	lda 	XSNumber0,x  					; output it
	jsr 	AssemblerWriteByte
	lda 	XSNumber1,x
	jsr 	AssemblerWriteByte
	rts

; ************************************************************************************************
;										<Opcode> n
; ************************************************************************************************

SweetAsm_LD: ;; [LD]
	jsr 	SweetAsmGetAltRegister
	clc
	adc 	#$20
	jsr 	AssemblerWriteByte
	rts

SweetAsm_ST: ;; [ST]
	jsr 	SweetAsmGetAltRegister
	clc
	adc 	#$30
	jsr 	AssemblerWriteByte
	rts

SweetAsm_LDD: ;; [LDD]
	.checkat
	.sweetreg $60
	rts

SweetAsm_STD: ;; [STD]
	.checkat
	.sweetreg $70
	rts

SweetAsm_POP: ;; [POP]
	.checkat
	.sweetreg $80
	rts

SweetAsm_STP: ;; [STP]
	.checkat
	.sweetreg $90
	rts

SweetAsm_ADD: ;; [ADD]
	.sweetreg $A0
	rts

SweetAsm_SUB: ;; [SUB]
	.sweetreg $B0
	rts

SweetAsm_POPD: ;; [POPD]
	.checkat
	.sweetreg $C0
	rts

SweetAsm_CPR: ;; [CPR]
	.sweetreg $D0
	rts

SweetAsm_INR: ;; [INR]
	.sweetreg $E0
	rts

SweetAsm_DCR: ;; [DCR]
	.sweetreg $F0
	rts

; ************************************************************************************************
;											No operand
; ************************************************************************************************

SweetAsm_RTN: ;; [RTN]
	.sweetopc $00
	rts

SweetAsm_RS: ;; [RS]
	.sweetopc $0B
	rts


; ************************************************************************************************
;									8 bit relative branch
; ************************************************************************************************

SweetAsm_BR: ;; [BR]
	.sweetbra $01

SweetAsm_BNC: ;; [BNC]
	.sweetbra $02

SweetAsm_BC: ;; [BC]
	.sweetbra $03

SweetAsm_BP: ;; [BP]
	.sweetbra $04

SweetAsm_BM: ;; [BM]
	.sweetbra $05

SweetAsm_BZ: ;; [BZ]
	.sweetbra $06

SweetAsm_BNZ: ;; [BNZ]
	.sweetbra $07

SweetAsm_BM1: ;; [BM1]
	.sweetbra $08

SweetAsm_BNM1: ;; [BNM1]
	.sweetbra $09

SweetAsm_BK: ;; [BK]
	.sweetbra $0A

SweetAsm_BS: ;; [BS]
	.sweetbra $0C

;
;		Branch common
;
ShortBranchCommon:
	jsr 	AssemblerWriteByte 				; output opcode
	ldx 	#0  							; target address
	jsr 	EXPEvalInteger16  			
	jsr 	AssembleRelativeBranch 			; use 65C02 code.
	rts

; ************************************************************************************************
;							16 bit relative subroutine call (added)
; ************************************************************************************************

SweetAsm_BSL: ;; [BSL]
	.sweetopc $0D 							; subroutine call

	ldx 	#0  							; target address
	jsr 	EXPEvalInteger16  			

	sec 									; calculate target - P 
	lda 	XSNumber0
	sbc 	VariableP
	pha
	lda 	XSNumber1
	sbc 	VariableP+1
	tax
	pla 									; in X:A

	sec  									; sub 2, 2 further back to allow for operand.
	sbc 	#2
	php
	jsr 	AssemblerWriteByte 				; low offset
	plp
	txa
	sbc 	#0
	jsr 	AssemblerWriteByte 				; high offset

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
