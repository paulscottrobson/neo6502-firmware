; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		tilemap.asm
;		Purpose:	Specify tilemap and offset 
;		Created:	21st January 2024
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;										TILEMAP Command
;
; ************************************************************************************************

		.section code

CommandTileMap: ;; [tilemap]
		ldx 	#0
_CTNextParam: 								; read 3 parameters into control parameters		
		jsr 	EXPEvalInteger16 		
		jsr 	ERRCheckComma
		inx
		jsr 	EXPEvalInteger16 		
		jsr 	ERRCheckComma
		inx
		jsr 	EXPEvalInteger16 		
		phy
		ldx 	#0
		ldy 	#0
_CTCopyParam:
		lda 	XSNumber0,x
		sta 	ControlParameters,y
		lda 	XSNumber1,x
		sta 	ControlParameters+1,y
		iny
		iny
		inx
		cpx 	#3
		bne 	_CTCopyParam
		ply

		.DoSendMessage 						; transmit information
		.byte 	5,35
		.DoWaitMessage
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

	