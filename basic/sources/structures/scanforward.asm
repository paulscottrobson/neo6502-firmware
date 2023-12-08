; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		scanforward.asm
;		Purpose:	Scan Forward
;		Created:	8th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section code

; ************************************************************************************************
;
;		Scan forward looking for A or X, skipping complete structures, A contains match
;
; ************************************************************************************************

ScanForward:
		stz 	zTemp1 						; clear structure count.
ScanForwardMain:		
		sta 	zTemp0 						; save scan options in zTemp0
		stx 	zTemp0+1
		;
		;
		;		Scan forward loop.
		;
_ScanLoop:		
		lda 	zTemp1 						; if structure count non zero, don't check for end.
		bne 	_ScanNoCheck
		;
		lda 	(codePtr),y 				; reached either target token.
		cmp 	zTemp0
		beq 	_ScanExit
		cmp 	zTemp0+1
		beq 	_ScanExit
_ScanNoCheck:
		jsr 	SkipOneInstruction
		bcc 	_ScanLoop
		.error_structure

_ScanExit:
		iny 								; consume final token.		
		rts

; ************************************************************************************************
;
;					Skip one instruction, return CS on underflow error
;
; ************************************************************************************************

SkipOneInstruction:
		lda 	(codePtr),y 				; get the token and consume it.
		iny
		;
		cmp 	#KWD_SYS_DEC 				; check for special multi-byte elements
		beq		_ScanDataItem
		cmp 	#KWD_SYS_STR
		beq 	_ScanDataItem
		;
		cmp 	#KWD_SYS_END 				; handle end of line.
		beq 	_ScanNextLine
		;
		cmp 	#$20 						; identifier $00xx-$1Fxx
		bcc 	_ScanSkip2
		cmp 	#KWD_SYS_SH1 				; shift 1 and 2
		beq 	_ScanSkip2
		cmp 	#KWD_SYS_SH2
		bne 	_ScanNoShift
_ScanSkip2:		
		iny
_ScanNoShift:		
;
;		Handle structures open/close
;
		tax 								; save in X
		and 	#$F0						; nested structures $Bx
		cmp 	#$B0
		bne 	_SOIExit
		;
		txa 								; shift.
		and 	#1 							; bit 0 0 = up, 1 = down
		eor 	#1 							; now 1 up 0 down
		asl 	a 							; now 2 up 0 down and clc
		dec 	a 							; now 1 up -1 down.
		adc 	zTemp1 						; add to structure depth.
		sta 	zTemp1
		bpl		_SOIExit 		 			; error if -ve
		sec
		rts

;
;		Scan over end of line.
;
_ScanNextLine:
		clc	 								; forward to next line.
		lda 	(codePtr)
		adc 	codePtr
		sta 	codePtr
		bcc 	_ScanNoCarry
		inc 	codePtr+1
_ScanNoCarry:
		ldy 	#3		
		lda 	(codePtr) 					; off end of program ?
		bne 	_SOIExit
		sec 								; failed.
		rts
;
;		Scan over [decimal] or [string]
;
_ScanDataItem:	 					
		tya
		sec
		adc 	(codePtr),y
		tay
		bra 	_SOIExit

_SOIExit:
		clc
		rts		

		.send code

; ************************************************************************************************
;
;		Date			Notes
;		==== 			=====
;
; ************************************************************************************************
