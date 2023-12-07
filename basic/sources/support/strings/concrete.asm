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
		.byte 	3
		sta 	zTemp0 						; save address of pointer in zTemp0.
		sty 	zTemp0+1
		;
		;		Is there a concreted string present, if not do concreting immediately.
		;

		;
		;		Does the new string fit in the concreted string, if not reconcrete
		;

		;
		;		Concrete the string.
		;

		;
		;		Copy string into concreted address and update.
		;
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
