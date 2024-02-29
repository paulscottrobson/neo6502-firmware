; Graphics assembly example for the Neo6502
;
; Copyright 2024 bill-auger <bill-auger@programmer.net>
; SPDX-License-Identifier: CC0-1.0
;
; This is a very basic assembly program,
; which demonstrates how to use the API to draw some graphics.
; Refer to api.pdf for more API functions.
;
; To assemble and run:
;   $ 64tass --mw65c02 --nostart --output=graphics.neo graphics.asm
;   $ ../bin/neo graphics.neo@800 cold


; RAM load address
* = $800

; Neo6502 Kernel API convenience macros
.include 'neo6502.asm.inc'


start:
  ;---------------------------------------;
  ; Draw line - (API Group 5, Function 2) ;
  ;---------------------------------------;

  lda API_FN_DRAW_LINE
  sta API_FUNCTION
  lda #$00               ; Begin X low-byte
  sta API_PARAMETERS + 0
  lda #$00               ; Begin X high-byte
  sta API_PARAMETERS + 1
  lda #$00               ; Begin Y low-byte
  sta API_PARAMETERS + 2
  lda #$00               ; Begin Y high-byte
  sta API_PARAMETERS + 3
  lda #$E0               ; End X low-byte
  sta API_PARAMETERS + 4
  lda #$00               ; End X high-byte
  sta API_PARAMETERS + 5
  lda #$E0               ; End Y low-byte
  sta API_PARAMETERS + 6
  lda #$00               ; End Y high-byte
  sta API_PARAMETERS + 7
  lda API_GROUP_GRAPHICS
  sta API_COMMAND


  ;--------------------------------------------;
  ; Draw Rectangle - (API Group 5, Function 3) ;
  ;--------------------------------------------;

  lda API_FN_DRAW_RECT
  sta API_FUNCTION
  lda #$00               ; Begin X low-byte
  sta API_PARAMETERS + 0
  lda #$00               ; Begin X high-byte
  sta API_PARAMETERS + 1
  lda #$00               ; Begin Y low-byte
  sta API_PARAMETERS + 2
  lda #$00               ; Begin Y high-byte
  sta API_PARAMETERS + 3
  lda #$E0               ; End X low-byte
  sta API_PARAMETERS + 4
  lda #$00               ; End X high-byte
  sta API_PARAMETERS + 5
  lda #$E0               ; End Y low-byte
  sta API_PARAMETERS + 6
  lda #$00               ; End Y high-byte
  sta API_PARAMETERS + 7
  lda API_GROUP_GRAPHICS
  sta API_COMMAND


  ;------------------------------------------;
  ; Draw Ellipse - (API Group 5, Function 4) ;
  ;------------------------------------------;

  lda API_FN_DRAW_ELLIPSE
  sta API_FUNCTION
  lda #$00               ; Begin X low-byte
  sta API_PARAMETERS + 0
  lda #$00               ; Begin X high-byte
  sta API_PARAMETERS + 1
  lda #$00               ; Begin Y low-byte
  sta API_PARAMETERS + 2
  lda #$00               ; Begin Y high-byte
  sta API_PARAMETERS + 3
  lda #$E0               ; End X low-byte
  sta API_PARAMETERS + 4
  lda #$00               ; End X high-byte
  sta API_PARAMETERS + 5
  lda #$E0               ; End Y low-byte
  sta API_PARAMETERS + 6
  lda #$00               ; End Y high-byte
  sta API_PARAMETERS + 7
  lda API_GROUP_GRAPHICS
  sta API_COMMAND


  ;------------------------------------------;
  ; Set graphics - (API Group 5, Function 1) ;
  ;------------------------------------------;

  ; set color mask and fill mode to modify existing pixels
  lda API_FN_SET_GFX
  sta API_FUNCTION
  lda #$0F               ; AND existing pixel color
  sta API_PARAMETERS + 0
  lda #$08               ; XOR the AND'ed color
  sta API_PARAMETERS + 1
  lda #$01               ; fill mode - solid
  sta API_PARAMETERS + 2
  lda #$00               ; TODO: seems to have no effect
  sta API_PARAMETERS + 3 ;
  lda #$00               ; flip mode - none
  sta API_PARAMETERS + 4 ;
  lda API_GROUP_GRAPHICS
  sta API_COMMAND


  ;------------------------------------------;
  ; Draw Ellipse - (API Group 5, Function 4) ;
  ;------------------------------------------;

  ; a smaller ellipse, in masked fill mode
  lda API_FN_DRAW_ELLIPSE
  sta API_FUNCTION
  lda #$20               ; Begin X low-byte
  sta API_PARAMETERS + 0
  lda #$00               ; Begin X high-byte
  sta API_PARAMETERS + 1
  lda #$20               ; Begin Y low-byte
  sta API_PARAMETERS + 2
  lda #$00               ; Begin Y high-byte
  sta API_PARAMETERS + 3
  lda #$C0               ; End X low-byte
  sta API_PARAMETERS + 4
  lda #$00               ; End X high-byte
  sta API_PARAMETERS + 5
  lda #$C0               ; End Y low-byte
  sta API_PARAMETERS + 6
  lda #$00               ; End Y high-byte
  sta API_PARAMETERS + 7
  lda API_GROUP_GRAPHICS
  sta API_COMMAND


  ;------------------------------------------;
  ; Set graphics - (API Group 5, Function 1) ;
  ;------------------------------------------;

  ; set opaque color and fill mode to obscure existing pixels
  lda API_FN_SET_GFX
  sta API_FUNCTION
  lda #$00               ; AND $00 to mask out underlying pixels
  sta API_PARAMETERS + 0
  lda #$0D               ; XOR $0C to set new color
  sta API_PARAMETERS + 1
  lda #$01               ; fill mode - solid
  sta API_PARAMETERS + 2
  lda #$00               ; TODO: seems to have no effect
  sta API_PARAMETERS + 3 ;
  lda #$00               ; flip mode - none
  sta API_PARAMETERS + 4 ;
  lda API_GROUP_GRAPHICS
  sta API_COMMAND


  ;------------------------------------------;
  ; Draw Ellipse - (API Group 5, Function 4) ;
  ;------------------------------------------;

  ; a yet smaller ellipse, in opaque fill mode
  lda API_FN_DRAW_ELLIPSE
  sta API_FUNCTION
  lda #$40               ; Begin X low-byte
  sta API_PARAMETERS + 0
  lda #$00               ; Begin X high-byte
  sta API_PARAMETERS + 1
  lda #$40               ; Begin Y low-byte
  sta API_PARAMETERS + 2
  lda #$00               ; Begin Y high-byte
  sta API_PARAMETERS + 3
  lda #$A0               ; End X low-byte
  sta API_PARAMETERS + 4
  lda #$00               ; End X high-byte
  sta API_PARAMETERS + 5
  lda #$A0               ; End Y low-byte
  sta API_PARAMETERS + 6
  lda #$00               ; End Y high-byte
  sta API_PARAMETERS + 7
  lda API_GROUP_GRAPHICS
  sta API_COMMAND


end:
  jmp end ; infinite loop
