; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		transceive.asm
;		Purpose:	Transmit and Receive memory blocks to I2C/SPI
;		Created:	2nd March 2024
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;							IRECEIVE/SRECEIVE/ITRANSMIT/STRANSMIT Commands
;							 All the same except different group function
;
; ************************************************************************************************

		.section code

command_ireceive: ;; [ireceive]
		lda 	#9
		bra 	TransceiveMain
command_itransmit: 	;; [itransmit]
		lda 	#10
		bra 	TransceiveMain

command_sreceive: ;; [sreceive]
		lda 	#11
		bra 	TransceiveMainB
command_stransmit: 	;; [stransmit]
		lda 	#12
		bra 	TransceiveMainB

TransceiveMainB:
		pha
		ldx 	#1
		bra 	TransceiveStartLength

TransceiveMain:
		pha 								; save the function #
		ldx 	#0 							; device #
		jsr 	EXPEvalInteger8
		jsr 	ERRCheckComma
		inx
TransceiveStartLength:		
		jsr 	EXPEvalInteger16 			; start
		jsr 	ERRCheckComma
		inx
		jsr 	EXPEvalInteger16 			; length.

		lda 	XSNumber0 					; set up
		sta 	ControlParameters+0
		lda 	XSNumber0+1
		sta 	ControlParameters+1
		lda 	XSNumber1+1
		sta 	ControlParameters+2
		lda 	XSNumber0+2
		sta 	ControlParameters+3
		lda 	XSNumber1+2
		sta 	ControlParameters+4

		DoWaitMessage  						; do the relevant function.
		pla
		sta 	ControlFunction
		lda 	#10
		sta 	ControlCommand_Check
		DoWaitMessage

		lda 	ControlError
		bne 	_RTHardware
		rts		

_RTHardware:
		.error_hardware

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

