; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		for.asm
;		Purpose:	For/Next loops
;		Created:	8th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section code

; ************************************************************************************************
;
;										FOR command
;
; ************************************************************************************************

Command_FOR:	;; [for]

		lda 	#STK_FOR 					; create frame
		jsr 	StackOpen 
		jsr 	Command_LET 				; do "I = 1" bit

		lda 	XSControl 					; must be an integer.
		and 	#XS_TYPEMASK
		bne 	_CFType
		;
		lda 	(CodePtr),y 				; look for TO or DOWNTO
		iny
		ldx 	#0
		cmp 	#KWD_TO
		beq 	_CFContinue
		dex
		cmp 	#KWD_DOWNTO
		bne 	_CFSyntax
_CFContinue:
		phx  								; X is direction $00 up $FF down
		ldx 	#2 							; terminal value into slot 2
		jsr 	EXPEvalInteger
		plx

		phy 								

		ldy 	#4 							; save variable address to +4,+5
		lda 	XSNumber0
		sta 	(basicStack),y
		iny
		lda 	XSNumber1
		sta 	(basicStack),y
		iny
		;
		txa 								; set direction to $00 or $FF in +6
		sta 	(basicStack),y 	
		iny
		;
		;
		lda 	XSNumber0+2 				; write terminal value onto basic stack.
		sta 	(basicStack),y
		iny
		lda 	XSNumber1+2
		sta 	(basicStack),y
		iny
		lda 	XSNumber2+2
		sta 	(basicStack),y
		iny
		lda 	XSNumber3+2
		sta 	(basicStack),y

		ply

		jsr 	STKSaveCodePosition 		; save loop address on stack.
		rts

_CFType:
		.error_type
_CFSyntax:
		.error_syntax		

; ************************************************************************************************
;
;										NEXT command
;
; ************************************************************************************************

Command_NEXT:	;; [next]
		lda 	#STK_FOR 					; check in a FOR.
		jsr 	StackCheckFrame
		;
		lda 	(codePtr),y 				; is it NEXT <something>
		cmp 	#KWD_SYS_END
		beq 	_CNNoVariable
		cmp 	#KWD_COLON
		beq 	_CNNoVariable

		ldx 	#0
		jsr 	EvaluateTerm 				; term, which should match the variable
		lda 	XSControl,x 				; check it is a variable
		and 	#XS_ISVARIABLE
		beq 	_CNBadVariable 				; no, we have a problem.

		phy 								; check variable
		ldy 	#4
		lda 	(basicStack),y
		cmp 	XSNumber0,x
		bne 	_CNBadVariable
		iny
		lda 	(basicStack),y
		cmp 	XSNumber1,x
		bne 	_CNBadVariable
		ply

_CNNoVariable:
		;
		phy 								; save code pointer which we don't use.
		ldy 	#4 							; copy variable address to zTemp0
		lda 	(basicStack),y
		sta 	zTemp0
		iny
		lda 	(basicStack),y
		sta 	zTemp0+1
		iny
		lda 	(basicStack),y 				; get direction ($00 up, $FF down)
		bmi 	_CNDownCount
		;
		;		Upcounting code.
		;
		jsr 	_CNCompare 					; is value < terminal, if >= already then exit
		bcc 	_CNExitLoop		
		sec 								; code to add 1 (add 0 with carry set.)
		ldx 	#0
		jsr 	_CNAdjust
		bra 	_CNLoopBack
		;
		;		Downcounting code
		;		
_CNDownCount:
		clc 								; decrement counter (add $FF with carry clear)
		ldx 	#$FF		
		jsr 	_CNAdjust
		jsr 	_CNCompare 					; is value < terminal, if so then exit
		bcs 	_CNExitLoop
		;
		;		Loop back
		;
_CNLoopBack:
		ply 								; restore Y
		jsr 	STKLoadCodePosition 		; loop back
		rts
		;
		;		End Loop
		;
_CNExitLoop:
		ply 								; restore Y
		jsr 	StackClose		 			; remove stack frame
		rts

_CNBadVariable:
		.error_structure
;
;		Add X to each byte of the index, either increments (CS,X = 0) or decrements (CC,X = $FF)
;		

_CNAdjust:
		txa
		adc 	(zTemp0)
		sta 	(zTemp0)
		ldy 	#1
		txa
		adc 	(zTemp0),y
		sta 	(zTemp0),y
		iny
		txa
		adc 	(zTemp0),y
		sta 	(zTemp0),y
		iny
		txa
		adc 	(zTemp0),y
		sta 	(zTemp0),y
		rts	

;
;		Signed comparison of index vs terminal value.
;
_CNCompare:
		clc 								; self modifying code, allows access of BASIC stack indexed by Y.
		lda 	basicStack
		adc 	#7
		sta 	_CNCLTV+1
		lda 	basicStack+1
		adc 	#0
		sta 	_CNCLTV+2

		ldy 	#0 							; offset
		ldx 	#4 							; count
		sec
_CNCLoop:
		lda 	(zTemp0),y
_CNCLTV:		
		sbc 	$FFFF,y				
		iny
		dex
		bne 	_CNCLoop
		bvc 	_CNCNoOverflow 				; fix for signed
		eor 	#$80		
_CNCNoOverflow:
		asl 	a 							; CS if -ve
		rts

; ************************************************************************************************
;
;		+0 		Token
;		+1,2,3 	Loop back position
;		+4,+5 	Address of the index variable (numeric integeer)
;		+6		Increment (0) Decrement ($FF)
;		+7..10	Terminal Value
;
; ************************************************************************************************

		.send code

; ************************************************************************************************
;
;		Date			Notes
;		==== 			=====
;
; ************************************************************************************************
