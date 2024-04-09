; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		instruction.asm
;		Purpose:	Assemble one instruction with type
;		Created:	27th December 2023
;		Reviewed: 	No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section 	code

; ************************************************************************************************
;
;		Try to Assemble Base Opcode, Operand (in Stack[0]), type X, CS if successful
;
; ************************************************************************************************

AssembleModeX:
		phy
		;
		;		First check if the instruction is supported.
		;
		lda 	isGroup1
		beq 	_AMXGroup2

		; ----------------------------------------------------------------------------------------
		;
		;		Group 1 code (LDA, STA etc.) the main group of 8
		;
		; ----------------------------------------------------------------------------------------

		txa 							; is it in group # 1
		and 	#AM_ISG1 				
		beq 	_AMXFail 				; no, give up.
		;
		txa 							; get back.
		and 	#$1F 					; this is now the offset.
		cpx 	#AM_IMMEDIATE 			; if not immediate, we can continue.
		bne 	_AMXHaveInfo
		;
		lda 	baseOpcode 				; cannot store immediate.
		cmp 	#$81 
		beq 	_AMXFail
		;
		lda 	#8 						; if immediate we use offset 8.
		bra 	_AMXHaveInfo 			; odd design decision there.

		; ----------------------------------------------------------------------------------------
		;
		;		Group 2, all the other non branch/singles outside the main group of 8
		;
		; ----------------------------------------------------------------------------------------

_AMXGroup2:
		txa 							; is it in group 2 ?
		and 	#AM_ISG2 				
		beq 	_AMXFail 				; no, give up.
		;
		txa 							; get the offset into Y
		and 	#$1F
		lsr 	a 						; make it 0-7.
		lsr  	a
		tay
		lda 	modeMask 				; shift the modemask left so you can check it against the requested offset
_AMXCheckOkay:
		asl 	a 						; shift modemask left enough times to check legitimate.
		dey
		bpl 	_AMXCheckOkay
		bcc 	_AMXFail 				; not allowed.
		txa  							; get mask back
		and 	#$1F 		

		; ----------------------------------------------------------------------------------------
		;
		;		At this point, X is the address mode, and A is the offset from base.
		;
		; ----------------------------------------------------------------------------------------

_AMXHaveInfo:
		cpx 	#$00 					; does it *require* an 8 bit operand
		bpl 	_AMXAnySize 			; checking bit 7, the zero page opcode flag.

		ldy 	XSNumber1 				; fail if it is outside range, e.g. the MSB is set.
		bne 	_AMXFail
		;
_AMXAnySize:		
		clc 							; add offset to the base opcode
		adc 	baseOpcode 				
_AMXOutputCode:		
		jsr 	AssemblerWriteByte 		; write the opcode out.

		cpx 	#AM_ACC 				; A mode (e.g. ASL)
		beq 	_AMXExit

		lda 	XSNumber0 				; write LSB operand
		jsr 	AssemblerWriteByte
		cpx 	#$00 					; zeropage ?
		bmi 	_AMXExit		
		lda 	XSNumber1 				; write MSB operand
		jsr 	AssemblerWriteByte
_AMXExit:		
		ply
		sec
		rts
;
;		Didn't match. Check the list of oddities.
;
_AMXFail:
		ldy 	#0
_AMXCheckOddities:
		txa 	
		cmp 	ExtraOpcode+2,y 		; match address mode	
		bne 	_AMXCONext
		lda 	ExtraOpcode+0,y 		; do the base opcodes match
		cmp 	baseOpcode
		bne 	_AMXCONext
		;
		cpx 	#$00 					; requires zero page ?
		bpl 	_AMXCONotZero
		lda 	XSNumber1
		bne 	_AMXCONext
_AMXCONotZero:		
		lda 	ExtraOpcode+1,y 		; get new opcode
		bra 	_AMXOutputCode 			; and assemble it
		;
_AMXCONext:
		iny		
		iny		
		iny		
		lda 	ExtraOpcode+0,y 		; end of table
		bne 	_AMXCheckOddities
_AMXCOFail:		
		ply
		clc
		rts
;
;		Table of non-standards. Base Opcode, Final Opcode, Address mode.
;
ExtraOpcode:
		.byte $40,$6c,AM_INDABS 	; jump
		.byte $40,$7C,AM_INDABSX 	; jump 
		.byte $a2,$BE,AM_ABSY 		; ldx
		.byte $a2,$B6,AM_ZEROY 		; ldx
		.byte $82,$96,AM_ZEROY 		; stx 
		.byte $e2,$1a,AM_ACC 		; inc
		.byte $c2,$3a,AM_ACC 		; dec
		.byte $20,$89,AM_IMMEDIATE 	; bit
		.byte $60,$9c,AM_ABS 		; stz 
		.byte $60,$9e,AM_ABSX 		; stz 
		.byte $00

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
