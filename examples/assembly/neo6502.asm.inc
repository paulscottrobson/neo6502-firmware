; Convenience macros for Neo6502 applications programming
; SPDX-License-Identifier: CC0-1.0


;-----------------------------------------------------;
; Neo6502 Kernel jump vectors (see kernel/kernel.asm) ;
;-----------------------------------------------------;

ReadLine       = $FFEB
ReadCharacter  = $FFEE
WriteCharacter = $FFF1
WaitMessage    = $FFF4
SendMessage    = $FFF7


;--------------------------------------;
; Neo6502 Kernel API control addresses ;
;--------------------------------------;

ControlPort = $FF00
DCommand    = ControlPort + 0 ; function group address
DFunction   = ControlPort + 1 ; function address
DParameters = ControlPort + 4 ; function parameters base address (+0-7)


;------------------------------------------------------------------;
; Neo6502 Kernel API control codes (see documents/release/api.pdf) ;
;------------------------------------------------------------------;

; Console functions (Group 2)
API_GROUP_CONSOLE = #$02 ; API function group ID
API_FN_WRITE_CHAR = #$06 ; API function ID
API_FN_CURSOR_POS = #$07 ; API function ID

; Sound functions (Group 8)
API_GROUP_SOUND     = #$08 ; API function group ID
API_FN_BEEP         = #$03 ; API function ID
API_FN_PLAY_SOUND   = #$05 ; API function ID
API_SOUND_CH_00     = #$00 ; API function parameter ID (only 0 by default)
API_SOUND_EFFECT_06 = #$06 ; API function parameter ID (currently 00-17 hex)


;--------------------------;
; color control characters ;
;--------------------------;

COLOR_BLACK       = #$80
COLOR_RED         = #$81
COLOR_GREEN       = #$82
COLOR_YELLOW      = #$83
COLOR_BLUE        = #$84
COLOR_MAGENTA     = #$85
COLOR_CYAN        = #$86
COLOR_WHITE       = #$87
COLOR_ALT_BLACK   = #$88
COLOR_DARK_GREY   = #$89
COLOR_DARK_GREEN  = #$8A
COLOR_ORANGE      = #$8B
COLOR_DARK_ORANGE = #$8C
COLOR_BROWN       = #$8D
COLOR_PINK        = #$8E
COLOR_LIGHT_GREY  = #$8F


;---------------------------------;
; limited-case convenience macros ;
;---------------------------------;

DoSendMessage .macro
  jsr  SendMessage
  .endm

DoWaitMessage .macro
  jsr  WaitMessage
  .endm
