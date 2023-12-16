; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		tokidcreate.asm
;		Purpose:	Create new identifier.
;		Created:	15th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section code

; ************************************************************************************************
;
;									   Create identifier
;
;				tokElement is preprocessed, e.g. bit 7 of the last character is set.
;
; ************************************************************************************************

TOKCreateIdentifier:
		lda 	#Program >> 8 				; find the end of the variable list.
		sta 	zTemp0+1
		lda 	#1
		sta 	zTemp0
		;
_TCIFind:
		lda 	(zTemp0) 					; follow the chain.
		beq 	_TCIFound
		clc
		adc 	zTemp0
		sta 	zTemp0
		bcc 	_TCIFind
		inc 	zTemp0+1
		bra 	_TCIFind
		;
_TCIFound:
		lda 	zTemp0 						; push the new record address on the stack.
		pha
		ldx 	zTemp0+1
		phx
		;
		pha 								; push the LSB on the stack, for expansion test when crossing $80
		;
		;		Length byte, and four null data bytes
		;
		lda 	tokElement 					; write the offset, which is the length + 6
		clc 
		adc 	#6
		jsr 	_TCIWrite
		;
		lda 	#0 							; then write four zeros out, this is the data.
		jsr 	_TCIWrite
		jsr 	_TCIWrite
		jsr 	_TCIWrite
		jsr 	_TCIWrite
		;
		;		Type byte
		;
		ldx 	tokElement 					; now work out the type.
		ldy 	#0 
		lda 	tokElement,x 				; last character ?
		cmp 	#"("+$80 					; is it an array / procedure e.g. last character is (
		bne 	_TCINotArray
		ldy 	#XS_ISARRAY 				; yes, set array flag
		dex  								; check string on previous character
_TCINotArray:
		lda 	tokElement,x 				; get last/2nd last.
		and 	#$7F 						; it could have bit 7 set.
		cmp 	#'$'						; string marker.
		bne 	_TCINotString
		tya
		ora 	#XS_ISSTRING
		tay
_TCINotString:
		tya 								; write out the type byte.
		jsr 	_TCIWrite				
		;
		;		Name of identifier
		;
		ldx 	#0 							; now output the name
_TCINameLoop:
		inx 								; output until bit 7 set.
		lda 	tokElement,x
		jsr 	_TCIWrite
		asl 	a
		bcc 	_TCINameLoop		

		lda 	#0 							; write a zero indicating end of the list.
		sta 	(zTemp0)
		;
		;		If creating this has cross the mid point e.g. $7F->$80 then add a new empty page.
		;
		pla 								; get the LSB of the new record want to check if crossed $7F-$80
		bmi 	_TCINoCrossing
		lda 	zTemp0
		bpl 	_TCINoCrossing
		jsr 	TOKInsertNewVariablePage 	; if so, add a new variable page.
_TCINoCrossing:		
		;
		pla 								; restore address to XA
		sec 								; make it an offset 
		sbc 	#Program >> 8
		tax
		pla
		inc 	a 							; bump XA so it points to the data.
		bne 	_TCINoCarry
		inx
_TCINoCarry:		
		rts
;
;		Write a byte out to the variable table.
;
_TCIWrite:
		sta 	(zTemp0)
		inc 	zTemp0
		bne 	_TICWExit
		inc 	zTemp0+1
_TICWExit:
		rts

; ************************************************************************************************
;
;									Insert new variable page
;
; ************************************************************************************************

TOKInsertNewVariablePage:
		jsr 	ClearResetFreeMemory 		; points zTemp0 to program end.
		;
		clc 	 							; X contains the terminal copy point, copying backwards
		lda 	Program 	
		adc 	#Program>>8
		tax
_TINVLoop:
		lda 	(zTemp0)					; copy it up.
		inc 	zTemp0+1
		sta 	(zTemp0)
		dec 	zTemp0+1		
		;
		lda 	zTemp0 						; back one.
		bne 	_TINVNoBorrow
		cpx 	zTemp0+1 					; done the last page
		beq 	_TINVExit
		dec 	zTemp0+1
_TINVNoBorrow:
		dec 	zTemp0
		bra 	_TINVLoop
_TINVExit:		
		inc 	Program 					; one more page of variables now.
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

