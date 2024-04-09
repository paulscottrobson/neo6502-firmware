; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		constants.inc
;		Purpose:	Assembler constants
;		Created:	27th December 2023
;		Reviewed: 	No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

;
;		Bit 7 : Zero Page (e.g. 1 byte only)
;		Bit 6 : Legitimate in Group 1
;		Bit 5 : Legitimate in Group 2
;		Bit 4..0 : Offset
;

AM_ISZP = $80
AM_ISG1 = $40
AM_ISG2 = $20

AM_IMMEDIATE = AM_ISZP+AM_ISG1+AM_ISG2+$00				
AM_ZEROPAGE = AM_ISZP+AM_ISG1+AM_ISG2+$04
AM_ACC = AM_ISG2+$08
AM_ABS = AM_ISG1+AM_ISG2+$0C
AM_INDY = AM_ISZP+AM_ISG1+$10
AM_ZEROX = AM_ISZP+AM_ISG1+AM_ISG2+$14
AM_ABSY = AM_ISG1+AM_ISG2+$18
AM_ABSX = AM_ISG1+AM_ISG2+$1C
AM_IND = AM_ISZP+AM_ISG1+$11
AM_ZEROY = AM_ISZP
AM_INDABS = $01
AM_INDABSX = $02
AM_INDX = AM_ISZP+AM_ISG1+$00

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
