; ***************************************************************************************
; ***************************************************************************************
;
;      Name :      support.asm
;      Authors :   Paul Robson (paul@robsons.org.uk)
;      Date :      23rd November 2023
;      Reviewed :  No
;      Purpose :   Support functions
;
; ***************************************************************************************
; ***************************************************************************************

KWriteCharacter:	
	jsr KWaitProcess
	and #$7f
	sta $FF04
	lda #0
	sta $FF01
	lda #1
	sta $FF00
	rts

KReadCharacter:
	jsr 	KWaitProcess
	lda 	#1
	sta		$FF01
	sta 	$FF00
	jsr 	KWaitProcess
	lda 	$FF04
	beq 	KReadCharacter
	rts

KWaitProcess:
	pha
KWaitProcess1:
	lda $FF00
	bne KWaitProcess1
	pla
	rts
	