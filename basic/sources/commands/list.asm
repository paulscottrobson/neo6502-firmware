; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		list.asm
;		Purpose:	List program
;		Created:	17th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;										LIST Command
;
; ************************************************************************************************

		.section code

Command_LIST:	;; [list]
		stz 	CLIndent 					; reset indent
		stz 	CLFrom 						; default from 
		stz 	CLFrom+1

		lda 	(codePtr),y 				; is there a to line (e.g. LIST ,xxx)
		cmp 	#KWD_COMMA
		beq 	_CLToLine
		cmp 	#KWD_SYS_END 				; EOL, default TO
		beq 	_CLDefaultTo

		ldx 	#0
		jsr 	EXPEvalInteger16 			; from value *and* to value now.
		lda 	XSNumber0
		sta 	CLFrom
		sta 	CLTo
		lda 	XSNumber1
		sta 	CLFrom+1
		sta 	CLTo+1
		lda 	(codePtr),y
		cmp 	#KWD_SYS_END 				; that's the lot ?
		beq 	_CLList
_CLToLine:		
		lda 	(codePtr),y 				; what follows.	
		cmp 	#KWD_SYS_END 				; EOL, default TO
		beq 	_CLDefaultTo		
		jsr 	ERRCheckComma 				; sep comma
		lda 	(codePtr),y 				; if it is just LIST , then default TO
		cmp 	#KWD_SYS_END
		beq 	_CLDefaultTo
		ldx 	#0
		jsr 	EXPEvalInteger16 			; there's a To value.
		lda 	XSNumber0
		sta 	CLTo
		lda 	XSNumber1
		sta 	CLTo+1
		bra 	_CLList
_CLDefaultTo: 								; to the end.
		lda 	#$FF
		sta 	CLTo
		sta 	CLTo+1
_CLList:
		lda 	Program 					; point out code start.
		clc
		adc 	#Program >> 8
		sta 	codePtr+1
		stz 	codePtr
		;
		;		Main loop
		;
_CLLoop:
		lda 	(codePtr) 					; finished
		beq 	_CLExit
		bit 	ControlStatus 				; break pressed ?
		bmi 	_CLBreak
		ldx 	#CLFrom-CLFrom 				; compare line number vs from
		jsr 	_CLCompareLine
		cmp 	#255 						; < from then skip
		beq 	_CLNext
		ldx 	#CLTo-CLFrom   				; compare line number vs IFR0
		jsr 	_CLCompareLine
		cmp 	#1 							; > to then skip
		beq 	_CLNext
		;
		;		Actually list the line.
		;
		ldy 	#2 							; print line #
		lda 	(codePtr),y
		tax
		dey
		lda 	(codePtr),y
		jsr 	PrintNumberXA
		;
		;		Get the indent, save it and add now if negative.
		;
		eor 	#255 						; this is the spacing.
		sec
		adc 	#6
		jsr 	_CLASpaces 					; print that many spaces

		jsr 	GetIndent
		pha		
		bpl 	_CLSpacing 					; skip if +ve
		clc 								; move backwards
		adc 	CLIndent
		bpl 	_CLSaveIndent
		lda 	#0
_CLSaveIndent:		
		sta 	CLIndent 					; update the indent.
_CLSpacing:		
		lda 	CLIndent
		jsr 	_CLASpaces 					; do the indent
		jsr 	TOKDetokenise 				; output the line text.
		lda 	#13	 						; next line
		jsr 	WriteCharacter

		pla 								; get indent up
		bmi 	_CLNext 				 	; if +ve add to indent
		clc
		adc 	CLIndent
		sta 	CLIndent
_CLNext:
		clc 								; advance to next line.
		lda 	(codePtr)
		adc 	codePtr
		sta 	codePtr
		bcc 	_CLNoCarry
		inc 	codePtr+1
_CLNoCarry:		
		bra 	_CLLoop		
_CLExit:
		jmp 	WarmStart		
_CLBreak:
		.error_break

_CLASpaces:
		cmp 	#0
		beq 	_CLASExit
		pha
		lda 	#32
		jsr 	WriteCharacter
		pla
		dec 	a
		bra 	_CLASpaces
_CLASExit:
		rts

_CLCompareLine:
		ldy 	#1
		sec
		lda 	(codePtr),y
		sbc 	CLFrom,x
		sta 	zTemp0
		iny
		lda 	(codePtr),y
		sbc 	CLFrom+1,x
		bcc 	_CLIsNegative
		bne 	_CLIsPositive
		lda 	zTemp0
		bne 	_CLIsPositive
		rts
_CLIsPositive:		
		lda 	#1
		rts
_CLIsNegative:
		lda 	#255		
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
;
; ************************************************************************************************

