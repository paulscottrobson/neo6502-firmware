; Hello World assembly example for the Neo6502
;
; Copyright 2024 bill-auger <bill-auger@programmer.net>
; SPDX-License-Identifier: CC0-1.0
;
; This is a very basic assembly program,
; which sounds a beep and displays some text at a precise screen location.
; It demonstrates how to use some of the limited-case convenience macros,
; and also how to use the complete API.
; Refer to api.pdf for more API functions.
;
; To assemble and run:
;   $ 64tass --mw65c02 --nostart --output=hello.neo hello.asm
;   $ ../bin/neo hello.neo@800 cold


; RAM load address
* = $800

; Neo6502 Kernel API convenience macros
.include 'neo6502.asm.inc'

; Program constants
CURSOR_POS_X = #0  ; character display 'X' coordinate
CURSOR_POS_Y = #21 ; character display 'Y' coordinate
NEWLINE_CHAR = #13 ; ASCII character code


;--------------;
; Main Program ;
;--------------;

start:
  ;-----------------------------------------------;
  ; Play sound effect - (API Group 8, Function 5) ;
  ;-----------------------------------------------;

  lda API_SOUND_CH_00    ; sound channel               (API::sound->play->channel)
  sta API_PARAMETERS + 0 ; set API 'Parameter0'        (API::sound->play->channel)
  lda API_SFX_COIN       ; sound effect index          (API::sound->play->effect)
  sta API_PARAMETERS + 1 ; set API 'Parameter1'        (API::sound->play->effect)
  lda API_FN_PLAY_SOUND  ; sound effect function       (API::sound->play)
  sta API_FUNCTION       ; set API 'Function'          (API::sound->play)
  lda API_GROUP_SOUND    ; 'Sound' API function group  (API::sound)
  sta API_COMMAND        ; trigger 'Sound' API routine (API::sound)


  ;--------------------------------------------------;
  ; Set cursor position - (API Group 2, Function 7) ;
  ;--------------------------------------------------;

  ; reposition the cursor to overwrite the default welcome text
  lda API_FN_SET_CURSOR_POS ; set cursor position function  (API::console->cursor)
  sta API_FUNCTION          ; set API 'Function'            (API::console->cursor)
  lda CURSOR_POS_X          ; cursor 'X' coordinate         (API::console->cursor->x)
  sta API_PARAMETERS + 0    ; set API 'Parameter0'          (API::console->cursor->x)
  lda CURSOR_POS_Y          ; cursor 'Y' coordinate         (API::console->cursor->y)
  sta API_PARAMETERS + 1    ; set API 'Parameter1'          (API::console->cursor->y)
  lda API_GROUP_CONSOLE     ; 'Console' API function group  (API::console)
  sta API_COMMAND           ; trigger 'Console' API routine (API::console)

  ; this simply repeats the same routine as the previous block,
  ; but using the generic convenience macro, for the sake of demonstration
  lda CURSOR_POS_X
  sta API_PARAMETERS + 0
  lda CURSOR_POS_Y
  sta API_PARAMETERS + 1
  #DoSendMessage ; send command 2,7
  .byte 2,7


  ;--------------------------------------------------------;
  ; Write character to console - (API Group 2, Function 6) ;
  ;--------------------------------------------------------;

  ; first, write a single newline character, using the special convenience macro
  lda NEWLINE_CHAR
  jsr WriteCharacter
  ; the text foreground color can also be set by injecting a control character
  lda COLOR_DARK_GREEN
  jsr WriteCharacter

  ; next, print the welcome message (a string of characters), using the API
  ldx #0                 ; initialize string iteration index
  lda API_FN_WRITE_CHAR  ; console write function        (API::console->write)
  sta API_FUNCTION       ; set API 'Function'            (API::console->write)
print_next_char:
  lda API_COMMAND        ; previous API routine status
  bne print_next_char    ; wait for previous API routine to complete

  lda hello_msg , x      ; next character of 'hello_msg' (API::console->write->char)
  beq end                ; test for string end null byte
  sta API_PARAMETERS + 0 ; set API 'Parameter0'          (API::console->write->char)
  lda API_GROUP_CONSOLE  ; 'Console' API function group  (API::console)
  sta API_COMMAND        ; trigger 'Console' API routine (API::console)

  inx                    ; increment iteration index
  jmp print_next_char    ; continue 'hello_msg' print loop

end:
  jmp end ; infinite loop


;--------------;
; Program data ;
;--------------;

hello_msg:
  .text "                   Hello Neo6502"                      ; line 1 to display
  .text 13                                                      ; newline
  .text "                                                     " ; 53 blanks
  .text 13                                                      ; newline
  .text "          Now you're playing with Neo Power!"          ; line 2 to display
  .text 13                                                      ; newline
  .text "              (Some assembly required)"                ; line 3 to display
  .text 0                                                       ; null-terminated
