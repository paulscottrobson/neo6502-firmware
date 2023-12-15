; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		tokconstant.asm
;		Purpose:	Output integer
;		Created:	14th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************
		
		.section code

; ************************************************************************************************
;
;							Output token data for constant in Stack,x
;
; ************************************************************************************************

TOKTokeniseConstant:
		lda 	XSNumber0,x					; check > 64
		pha 								; save LSB on stack

		and 	#$C0 						; check upper 26 bits zero.
		ora 	XSNumber1,x
		ora 	XSNumber2,x
		ora 	XSNumber3,x
		beq 	_TTCLess64
		;
		;		If >= 64 then shift 6 right and call recursively.
		;
		phy
		ldy 	#6 							; divide by 64
_TTCShiftRight:		
		lsr 	XSNumber3,x
		ror 	XSNumber2,x
		ror 	XSNumber1,x
		ror 	XSNumber0,x
		dey
		bne 	_TTCShiftRight
		ply
		jsr 	TOKTokeniseConstant
_TTCLess64:
		;
		;		0-63 write token out.
		;
		pla
		and 	#$3F 						; lower 6 bits
		ora 	#$40 						; put into the slot.
		jsr 	TOKWriteA
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

