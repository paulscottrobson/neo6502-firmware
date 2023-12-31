; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		undefined.asm
;		Purpose:	Undefined keywords
;		Created:	9th December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;										STOP Command
;
; ************************************************************************************************

		.section code

Command_SN1:	;; [proc]
Command_SN2:	;; [then]
Command_SN3:	;; [!!dec]
Command_SN4:	;; [to]
Command_SN5:	;; [downto]
Command_SN7:	;; [;]
Command_SN8:	;; [:]
Command_SN9:	;; [)]
Command_SN10:	;; [,]
Command_SN11:	;; [!!un1]

		.error_syntax
		
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

