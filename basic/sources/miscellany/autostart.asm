; ************************************************************************************************
; ************************************************************************************************
;
;       Name:       autostart.asm
;       Purpose:    Run autostart.base
;       Created:    25th March 2024
;       Reviewed:   No
;       Author:     Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

		.section code

; ************************************************************************************************
;
;										Check autostart 
;
; ************************************************************************************************

AutoStartCheck:
		lda 	#4 							; Check if 'A' pressed
		sta 	ControlParameters
		.DoSendMessage
		.byte 	1,2
		.DoWaitMessage
		lda 	ControlParameters			; if pressed then exit
		bne 	_ASCFail 					; without checking

		stz 	ControlParameters+0			; open to channel 0
		stz 	ControlParameters+3 		; open read only.
		lda 	#_ASCFile & $FF 			; set file name.
		sta 	ControlParameters+1
		lda 	#_ASCFile >> 8
		sta 	ControlParameters+2

		.DoSendMessage  					; open it.
		.byte 	3,4
		.DoWaitMessage

		lda 	ControlError 				; failed ?
		bne 	_ASCFail

		stz 	ControlParameters+0 		; close file opened.
		.DoSendMessage  				
		.byte 	3,5
		.DoWaitMessage

		lda 	#_ASCFile & $FF 			; set file name.
		sta 	ControlParameters+0
		lda 	#_ASCFile >> 8
		sta 	ControlParameters+1

		stz 	ControlParameters+2 		; load into program space
		lda 	#Program >> 8
		sta 	ControlParameters+3

		jsr 	LoadExtended 				; call the extended load code
		lda 	ControlError  				; error check
		bne 	_CLFileError

		jmp 	Command_RUN_Always 			; and run it, no file name check.

_CLFileError:
		.error_file

_ASCFail: 
		rts

_ASCFile:
		.byte 	_ASCFileEnd-_ASCFile-1
		.text 	'autostart.bas'
_ASCFileEnd:

		.send code

; ************************************************************************************************
;
;                                   Changes and Updates
;
; ************************************************************************************************
;
;       Date            Notes
;       ====            =====
;
; ************************************************************************************************

