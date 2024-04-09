; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		opcodes.asm
;		Purpose:	Opcode handling code code
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

sweetreg .macro
	.endm
;
;		Single byte opcodes.
;
sweetopc .macro
	lda 	#\1
	jsr 	AssemblerWriteByte
	.endm

sweetbra .macro
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
	.sweetreg $10
	; TODO comma,<value>
	rts

; ************************************************************************************************
;										<Opcode> n
; ************************************************************************************************

SweetAsm_LD: ;; [LD]
	.sweetreg $20
	; TODO check for @, bumps opcode by $20
	rts

SweetAsm_ST: ;; [ST]
	.sweetreg $30
	; TODO check for @, bumps opcode by $20
	rts

SweetAsm_LDD: ;; [LDD]
	.checkat
	.sweetreg $50
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
;							16 bit relative subroutine call (added)
; ************************************************************************************************

SweetAsm_BS: ;; [BS]
	.sweetreg $0D
	; TODO 16 bit operand, relative to PC
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
