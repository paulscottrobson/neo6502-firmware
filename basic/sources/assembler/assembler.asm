; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		assembler.asm
;		Purpose:	Assembler main
;		Created:	27th December 2023
;		Reviewed:	No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section 	code

; ************************************************************************************************
;
;								Assemble for groups 1 & 2
;
; ************************************************************************************************

		; ----------------------------------------------------------------------------------------
		;
		;		Group 1 - LDA/STA/ADC etc.
		;
		; ----------------------------------------------------------------------------------------

AssembleGroup1:
		lda 	#$FF 						; flag for group 1 / mask.
		sta 	modeMask 					; initialise the mode mask - all for all
		bra 	AsmGroup12 	

		; ----------------------------------------------------------------------------------------
		;
		;		Group 2 - LDX, INC, LSR etc.
		;
		; ----------------------------------------------------------------------------------------

AssembleGroup2:
		lda 	#$00 						; flag for group 2
AsmGroup12:
		sta 	isGroup1 					; save the 'group 1' flag

		pla 								; pop the return address to access the information following.
		plx
		jsr 	AccessParameters 			; get opcode and save as base
		sta 	baseOpcode
		;
		lda 	isGroup1 					; skip if group 1 as we don't have a complex mask.
		bne 	_AG12HaveMask
		;
		lda 	#2 							; if group 2 the second parameter is the mask
		jsr 	GetParameter		 		; e.g. which modes are supported for this operand
		sta 	modeMask
_AG12HaveMask:		
		jsr 	TypeAndCalculateOperand 	; get zero page type
		;
		;		First, try to see if it can be done as zero page zp,x xp,y
		;
		phx 								; save found address mode
		jsr 	AssembleModeX
		plx  								; restore address mode
		bcs 	_AG12Exit
		;
		;		Then, see if it can be done as absolute abs,x abs,y etc.
		;
		jsr 	PromoteToAbsolute  			; promote ZP to ABS and try that
		jsr 	AssembleModeX
		bcs 	_AG12Exit
		.error_syntax 						; can't do either, so must be wrong mode/operand.
_AG12Exit:	
		rts		

; ************************************************************************************************
;
;				Promote type in X all these convert the zero page typing to absolute typing
;
; ************************************************************************************************

PromoteToAbsolute:
		lda 	#AM_ABS 					; lda xx
		cpx 	#AM_ZEROPAGE
		beq 	_PTADo
		lda 	#AM_ABSX 					; lda xx,X
		cpx 	#AM_ZEROX
		beq 	_PTADo
		lda 	#AM_ABSY 					; lda xx,Y
		cpx 	#AM_ZEROY
		beq 	_PTADo 
		lda 	#AM_INDABS 					; lda (xx) (jump)
		cpx 	#AM_IND
		beq 	_PTADo
		lda 	#AM_INDABSX 				; lda (xx,x) (jump)
		cpx 	#AM_INDX
		beq 	_PTADo
		rts
_PTADo:
		tax
		rts		

; ************************************************************************************************
;
;								Relative Branches (Group 3)
;
; ************************************************************************************************

AssembleGroup3:
		pla 								; get parameters, which is just the opcode.
		plx

		jsr 	AccessParameters 			; get and output opcode
		jsr 	AssemblerWriteByte
		jsr 	CalculateOperand 			; get a 16 bit operand
		;
		lda 	XSNumber0 					; calculate the offset
		sec
		sbc 	VariableP
		pha 								; LSB in A
		lda 	XSNumber1
		sbc 	VariableP+1
		tax 								; MSB in X
		pla
		;
		clc 								; $80 to easy check $80-$7F, plus 1 for offset error
		adc 	#$7F
		bcc 	_AG3NoCarry
		inx
_AG3NoCarry:
		sec 								; fix back and write out anyways.
		sbc 	#$80
		jsr 	AssemblerWriteByte
		cpx 	#0 							; was it in range
		beq 	_AG3Exit
		lda 	VariableO 					; are we allowing bad values ?
		and 	#1
		beq 	_AG3Exit
		.error_range						; no, branch is out of range
_AG3Exit:
		rts		

; ************************************************************************************************
;
;										 No parameters at all
;
; ************************************************************************************************

AssembleGroup4:
		pla 								; pop address
		plx
		jsr 	AccessParameters 			; access and get first
		jsr 	AssemblerWriteByte 			; output it.
		rts

; ************************************************************************************************
;
;					 Save the parameter position popped from the return stack
;
; ************************************************************************************************

AccessParameters:
		sta 	paramStart
		stx 	paramStart+1
		lda 	#1

; ************************************************************************************************
;
;											Get the Ath parameter
;
; ************************************************************************************************

GetParameter:
		phy
		tay
		lda 	paramStart
		sta 	zTemp0		
		lda 	paramStart+1
		sta 	zTemp0+1
		lda 	(zTemp0),y
		ply
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
