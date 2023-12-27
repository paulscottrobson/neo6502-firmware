; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		operands.asm
;		Purpose:	Operand code
;		Created:	27th December 2023
;		Reviewed: 	No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section 	code

; ************************************************************************************************
;
;		Type the Instruction into one of : Immediate, Zero, ZeroX, ZeroY, Ind, IndX, Indy
;		We do not worry about Abs,AbsX,AbsY which are handled with two passes through the modes
; 		e.g. first pass we check for zeroX and then we try for absX
;
;		The absind etc. are dealt with seperately. Returns type in X.
;
;		X and Y will be tokenised as variables , so they will always exist, but there value
;		does not affect the assembler or their use as variables.
;
; ************************************************************************************************

TypeAndCalculateOperand:
		.cget 								; get first character

		; ---------------------------------------------------------------------------------------
		;
		;		Check if just stand alone e.g. LSR INC - we don't support ASL (etc) A
		;
		; ---------------------------------------------------------------------------------------

		ldx 	#AM_ACC 					; ACC if : or EOL
		cmp 	#KWD_COLON
		beq 	_TACOExit
		cmp 	#KWC_EOL
		beq 	_TACOExit
		;
		; ---------------------------------------------------------------------------------------
		;
		;		Check if Immediate e.g. LDA #42
		;
		; ---------------------------------------------------------------------------------------

		iny
		ldx 	#AM_IMMEDIATE
		cmp 	#KWD_HASH 					; if hash, then immediate - constant.
		beq 	CalculateOperand 		

		; ---------------------------------------------------------------------------------------
		;
		;		Check for indirect, if not must be operand operand,x operand,y
		;
		; ---------------------------------------------------------------------------------------

		cmp 	#KWD_LPAREN 				; if ( then check for indirection
		beq 	_TACOIndirect
		;
		dey 								; undo get of first character
		jsr 	CalculateOperand 			; get operand
		.cget 								; is it followed by , ?
		ldx 	#AM_ZEROPAGE 				; if not, try zero page (then absolute)
		cmp 	#KWD_COMMA
		bne 	_TACOExit

		; ---------------------------------------------------------------------------------------
		;
		;		Now it must be operand,x or operand,y
		;
		; ---------------------------------------------------------------------------------------

		iny
		jsr 	TACOCheckXY 				; extract X,Y or 0 (neither) from the variable.		
		ldx 	#AM_ZEROX 					; check for X
		cmp 	#'X'
		beq 	_TACOExit
		ldx 	#AM_ZEROY 					; check for Y
		cmp 	#'Y'
		beq 	_TACOExit
_TACOSyntax:		
		jmp 	SyntaxError					
		;
		; ---------------------------------------------------------------------------------------
		;
		;		Process (xx,X) (xx),Y or (xx)
		;
		; ---------------------------------------------------------------------------------------

_TACOIndirect:
		jsr 	CalculateOperand 			; get the operand
		.cget 								; what follows ?
		cmp 	#KWD_COMMA 					; if , then it must be ,X)
		beq 	_TACOIndX
		;
		; 		We have (xx) so it's either (xx) or (xx),Y
		;
		jsr 	CheckRightBracket  			; otherwise must be ) or ),Y
		.cget 							
		ldx 	#AM_IND 					
		cmp 	#KWD_COMMA 					; if not comma then exit with (xx)
		bne 	_TACOExit
		iny

		jsr 	TACOCheckXY 				; get the next one as X/Y/0
		cmp 	#'Y' 						; check Y
		bne 	_TACOSyntax
		ldx 	#AM_INDY
		bra 	_TACOExit
		;
		;		Check for (xx,X)
		;
_TACOIndX:
		iny
		jsr 	TACOCheckXY 				; get the next one as X/Y/0
		cmp 	#'X' 						; check X
		bne 	_TACOSyntax
		jsr 	CheckRightBracket			; check )
		ldx 	#AM_INDX
_TACOExit:
		rts		

; ************************************************************************************************
;
;								Calculate an operand (16 bit max)
;
; ************************************************************************************************

CalculateOperand:
		pha
		phx
		ldx 	#0 							; get 16 bit integer in Slot 0
		jsr 	Evaluate16BitInteger
		plx
		pla
		rts


; ************************************************************************************************
;
;		an X or Y must follow. These will have been tokenised as variables, so check their
;		variable records. Returns the ASCII codes for 'X' and 'Y', or zero if it is neither.
;
; ************************************************************************************************

TACOCheckXY:	
		.cget  								; get next	
		and 	#$C0 						; check it is an identifier reference.
		cmp 	#$40
		bne 	_TCXYFail
		;
		.cget 								; get variable address to zTemp
		clc 								
		adc 	#((VariableSpace >> 8) - $40) & $FF		
		sta 	zTemp0+1
		iny
		.cget 	
		iny
		sta 	zTemp0
		phy 								; save position
		;
		ldy 	#2 							; type is integer ?
		lda 	(zTemp0),y
		bne 	_TCXYPopFail
		ldy 	#8 							; get first character, should have bit 7 set as also last.
		lda 	(zTemp0),y
		cmp 	#'X'+$80 					; should be X or Y
		beq 	_TCXYFound		
		cmp 	#'Y'+$80
		beq 	_TCXYFound		
_TCXYPopFail:
		ply
_TCXYFail:
		lda 	#0
		rts		
_TCXYFound:
		ply 								; restore position
		and 	#$7F 						; throw bit 7
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
