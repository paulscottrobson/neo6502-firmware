; ***************************************************************************************
; ***************************************************************************************
;
;                                KEYBOARD READ ROUTINE
;
; ***************************************************************************************
; ***************************************************************************************

; Get character for keyboard, return in A.

        .if     NEO!=0

rdkey:  jsr     ReadCharacter
        jsr     FlipCharacterCase
        ora     #$80
        rts        
        .else

rdkey:  LDA     KBDCR           ; Read control register
        BPL     rdkey           ; Loop if no key pressed
        LDA     KBD             ; Read key data
        RTS                     ; and return

        .endif

