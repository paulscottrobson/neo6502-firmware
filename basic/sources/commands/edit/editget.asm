; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		editget.asm
;		Purpose:	Editor - Get line by number.
;		Created:	29th March 2024
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;									Get line # ControlParameters
;
; ************************************************************************************************

		.section code

EDGetLine:
		jsr 	EDUFindStart 				; find the start to zTemp0
		jsr 	EDUFindLine 				; find line according to param[0],param[1]
		lda 	zTemp0 						; set up to detokenise
		sta 	codePtr
		lda 	zTemp0+1
		sta 	codePtr+1
		jsr 	TOKDetokenise 				; detokenise it.
		lda		#inputPos & $FF  			; save link to the result.
		sta 	ControlParameters+0
		lda 	#inputPos >> 8
		sta 	ControlParameters+1
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

