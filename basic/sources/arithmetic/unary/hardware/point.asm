; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		point.asm
;		Purpose:	Read pixel from screen
;		Created:	18th January 2024
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************


		.section code	

; ************************************************************************************************
;
;									Read Pixel from screen
;
; ************************************************************************************************

EXPUnaryPoint: ;; [point(]
		jsr 	PointCommonCode
		.DoSendMessage 						; read pixel
		.byte 	5,33
		.DoWaitMessage

PointCommonExit:
		lda 	ControlError 				; failed ?
		bne 	_EUPError
		lda 	ControlParameters+0 		; true/false option.
		jmp 	EXPUnaryReturnA
_EUPError:
		.error_range

; ************************************************************************************************
;
;								Read Pixel from sprite layer
;
; ************************************************************************************************

EXPUnarySPoint: ;; [spoint(]
		jsr 	PointCommonCode
		.DoSendMessage 						; read pixel on sprite layer
		.byte 	5,36
		.DoWaitMessage
		bra 	PointCommonExit

PointCommonCode:		
		jsr 	EXPEvalInteger16 			; x
		inx
		jsr 	ERRCheckComma
		jsr 	EXPEvalInteger16 			; y
		dex
		jsr 	ERRCheckRParen

		lda 	XSNumber0,x 				; copy into parameter block.
		sta 	ControlParameters+0
		lda 	XSNumber1,x
		sta 	ControlParameters+1
		lda 	XSNumber0+1,x
		sta 	ControlParameters+2
		lda 	XSNumber1+1,x
		sta 	ControlParameters+3
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
; 		30-01-24 		Added support for SPOINT which is almost identical.
;
; ************************************************************************************************

