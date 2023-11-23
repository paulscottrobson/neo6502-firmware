; ***************************************************************************************
; ***************************************************************************************
;
;                                KEYBOARD READ ROUTINE
;
; ***************************************************************************************
; ***************************************************************************************

; Get character for keyboard, return in A.

        .if     NEO!=0

rdkey:  jsr     $FFEE
        ora     #$80
        rts        
        nop
        nop
        nop
        .else

rdkey:  LDA     KBDCR           ; Read control register
        BPL     rdkey           ; Loop if no key pressed
        LDA     KBD             ; Read key data
        RTS                     ; and return

        .endif

