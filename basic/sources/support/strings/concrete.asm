; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		concrete.asm
;		Purpose:	Concrete string (e.g. place it in permanent memory)
;		Created:	6th December 2023
;		Reviewed:   No
;		Author : 	Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section code

; ************************************************************************************************
;
;			Concrete string at zsTemp. On entry YA points to the address of any currently
;			concreted string.
;
; ************************************************************************************************

StringConcrete:
		sta 	zTemp0 						; save address of pointer in zTemp0.
		sty 	zTemp0+1
		;
		;		Is there a concreted string present, if not do concreting immediately.
		;
		ldy 	#1 							; check if the address stored is zero, e.g. no string here.
		lda 	(zTemp0),y
		ora 	(zTemp0)
		beq 	_SCConcreteNewString 		; if so, then start from scratch.
		;
		;		Does the new string fit in the concreted string, if not reconcrete
		;
		sec 								; point zTemp1 to concrete-1 e.g. the length allocated.
		lda 	(zTemp0)
		sbc 	#1
		sta 	zTemp1
		lda 	(zTemp0),y
		sbc 	#0
		sta 	zTemp1+1

		lda 	(zsTemp) 					; is the max length available <= length allocated
		cmp 	(zTemp1)
		bcc 	_SCCopyString 				; if so, we can just copy it over.
		beq 	_SCCopyString 				; if not, we need to reconcrete the string as this one is too small.
		;
		;		Concrete the string in zsTemp
		;
_SCConcreteNewString:
		lda 	(zsTemp) 					; string length required.
		inc 	a 							; one for length byte
		inc 	a 							; one for allocated length byte
		clc
		adc 	#10 						; allow extra space.
		bcc 	_SCCNSNocarry
		lda 	#$FF 						; max out at 255 bytes
_SCCNSNoCarry:		
		pha 								; save size.
		eor 	#$FF 						; 2's complement subtract from string memory.
		sec 								; also put in zTemp1
		adc 	stringMemory
		sta 	stringMemory
		sta 	zTemp1
		lda 	#$FF
		adc 	stringMemory+1
		sta 	stringMemory+1
		sta 	zTemp1+1
		pla 								; save the allocated length in the first byte, string size in 2nd byte.
		sta 	(zTemp1)
		;
		clc 								; copy the actual address into the targe.
		lda 	zTemp1
		adc 	#1
		sta 	(zTemp0)
		lda 	zTemp1+1
		adc 	#0
		ldy 	#1
		sta 	(zTemp0),y
		;
		;		Copy string at zsTemp into concreted address at zTemp1 
		;
_SCCopyString:
		lda 	(zsTemp) 					; X = length of string.
		tax
		inx 								; add 1 for length.

		ldy 	#0
_SCCopyLoop:
		lda 	(zsTemp),y 					; copy it one forward.
		iny
		sta 	(zTemp1),y		
		dex 								; copy given number of chars + 1
		bne 	_SCCopyLoop
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
