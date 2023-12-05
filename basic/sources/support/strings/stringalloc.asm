; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		stringalloc.asm
;		Purpose:	String allocation handler
;		Created:	5th December 2023
;		Reviewed:   No
;		Author : 	Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section code

; ************************************************************************************************
;
;			Reset the concreted string pointer, A is high byte of string memory top
;
; ************************************************************************************************

StringSystemInitialise:
		dec 	a 							; make last byte of previous page.
		sta 	stringMemory+1
		sta 	zTemp0+1
		lda 	#$FF
		sta 	stringMemory
		sta 	zTemp0

		lda 	#0 							; put a zero at the end, so know end of string memory.
		sta 	(zTemp0)
		rts

; ************************************************************************************************
;
;							  Initialise the string space
;
; ************************************************************************************************

StringSpaceInitialise:
		lda 	#$FF 						; only initialise once (set to $FF, bit used to test it)
		sta 	stringInitialised
		;
		lda 	stringMemory 				; allocate 256 bytes for one concreted string
		sta 	stringTempPointer 			; so temporary string space is allocated below that.
		lda 	stringMemory+1
		dec 	a
		sta 	stringTempPointer+1
		rts

; ************************************************************************************************
;
;		Allocate bytes for string of length A, temporary, and put in zsTemp and on TOS
;
;					  * FOR TEMPORARY STRING USAGE DURING ONE INSTRUCTION *
;
; ************************************************************************************************

StringTempAllocate:
		cmp 	#252+1 						; max length of strings
		bcs 	_STALength 			
		;
		bit 	stringInitialised 			; already initialised
		bmi 	_STAAllocate 				; then don't call the temp initialise routine
		;
		pha 								; save value to subtract.
		jsr 	StringSpaceInitialise 		; initialise string memory allocation
		pla 								; restore it
_STAAllocate:
		;
		eor 	#$FF 						; 2's complement add to StringTempPointer
		clc  								; deliberate allows one more
		adc 	stringTempPointer 			; subtract from temp pointer
		sta 	stringTempPointer  			
		sta 	XSNumber0,x	 				; address in mantissa too
		sta 	zsTemp 						; and zsTemp
		lda 	stringTempPointer+1
		adc 	#$FF
		sta 	stringTempPointer+1 		
		sta 	zsTemp+1
		sta 	XSNumber1,x
		stz 	XSNumber2,x 				; set the typing data and the upper address bytes.
		stz 	XSNumber3,x
		lda 	#XS_STRING					; it's a string.
		sta 	XSControl,x
		;
		lda 	#0 							; clear the target string
		sta 	(zsTemp)
		;
		rts

_STALength:
		.error_string

; ************************************************************************************************
;
;								Write A to temp string
;
; ************************************************************************************************

StringTempWrite:
		phy
		pha
		lda 	(zsTemp)
		inc 	a
		sta 	(zsTemp)
		tay
		pla
		sta 	(zsTemp),y
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
