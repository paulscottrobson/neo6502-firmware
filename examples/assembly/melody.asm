; Melody assembly example for the Neo6502
;
; Copyright 2024 bill-auger <bill-auger@programmer.net>
; SPDX-License-Identifier: CC0-1.0
;
; This is a very basic assembly program,
; which demonstrates how to use the API to play a melody.
; Refer to api.pdf for more API functions.
;
; To assemble and run:
;   $ 64tass --mw65c02 --nostart --output=melody.neo melody.asm
;   $ ../bin/neo melody.neo@800 cold


; RAM load address
* = $800

; Neo6502 Kernel API convenience macros
.include 'neo6502.asm.inc'


;--------------;
; Main Program ;
;--------------;

start:
  ;-----------------------------------------;
  ; Queue sound - (API Group 8, Function 4) ;
  ;-----------------------------------------;

  ; these note durations will be 60s / 80 / 3
  ; that is, 6/8 compound time eighth-notes at 80BPM
  ; for 4/4 time eighth-notes, divide API_TEMPO_* by 2 instead
  lda API_SOUND_CH_00    ; sound channel               (API::sound->queue->channel)
  sta API_PARAMETERS + 0 ; set API 'Parameter0'        (API::sound->queue->channel)
  lda <API_NOTE_C4       ; sound frequency low byte    (API::sound->queue->frequency)
  sta API_PARAMETERS + 1 ; set API 'Parameter1'        (API::sound->queue->frequency)
  lda >API_NOTE_C4       ; sound frequency high byte   (API::sound->queue->frequency)
  sta API_PARAMETERS + 2 ; set API 'Parameter2'        (API::sound->queue->frequency)
  lda <API_TEMPO_80 / 3  ; sound duration low byte     (API::sound->queue->duration)
  sta API_PARAMETERS + 3 ; set API 'Parameter3'        (API::sound->queue->duration)
  lda >API_TEMPO_80 / 3  ; sound duration high byte    (API::sound->queue->duration)
  sta API_PARAMETERS + 4 ; set API 'Parameter4'        (API::sound->queue->duration)
  lda <API_SLIDE_NONE    ; sound slide low byte        (API::sound->queue->slide)
  sta API_PARAMETERS + 5 ; set API 'Parameter5'        (API::sound->queue->slide)
  lda >API_SLIDE_NONE    ; sound slide high byte       (API::sound->queue->slide)
  sta API_PARAMETERS + 6 ; set API 'Parameter6'        (API::sound->queue->slide)
  lda API_SOUND_SRC_BEEP ; sound source                (API::sound->queue->source)
  sta API_PARAMETERS + 7 ; set API 'Parameter7'        (API::sound->queue->source)
  lda API_FN_QUEUE_SOUND ; queue sound function        (API::sound->queue)
  sta API_FUNCTION       ; set API 'Function'          (API::sound->queue)
  lda API_GROUP_SOUND    ; 'Sound' API function group  (API::sound)
  sta API_COMMAND        ; trigger 'Sound' API routine (API::sound)

  lda <API_NOTE_E4
  sta API_PARAMETERS + 1
  lda >API_NOTE_E4
  sta API_PARAMETERS + 2
  lda API_GROUP_SOUND
  sta API_COMMAND

  lda <API_NOTE_G4
  sta API_PARAMETERS + 1
  lda >API_NOTE_G4
  sta API_PARAMETERS + 2
  lda API_GROUP_SOUND
  sta API_COMMAND

  lda <API_NOTE_C5
  sta API_PARAMETERS + 1
  lda >API_NOTE_C5
  sta API_PARAMETERS + 2
  lda API_GROUP_SOUND
  sta API_COMMAND

  lda <API_NOTE_G4
  sta API_PARAMETERS + 1
  lda >API_NOTE_G4
  sta API_PARAMETERS + 2
  lda API_GROUP_SOUND
  sta API_COMMAND

  lda <API_NOTE_E4
  sta API_PARAMETERS + 1
  lda >API_NOTE_E4
  sta API_PARAMETERS + 2
  lda API_GROUP_SOUND
  sta API_COMMAND

  lda <API_NOTE_C4
  sta API_PARAMETERS + 1
  lda >API_NOTE_C4
  sta API_PARAMETERS + 2
  lda API_GROUP_SOUND
  sta API_COMMAND

  ; this "note" is silent (a rest note)
  lda <API_NOTE_REST
  sta API_PARAMETERS + 1
  sta API_PARAMETERS + 2 ; high and low bytes are the same (0)
  lda API_GROUP_SOUND
  sta API_COMMAND

  ; the next note durations will be 60s / 80 / 6
  ; twice as rapid that is, 6/8 compound time sixteenth-notes at 80BPM
  lda <API_TEMPO_80 / 6
  sta API_PARAMETERS + 3
  lda >API_TEMPO_80 / 6
  sta API_PARAMETERS + 4

  lda <API_NOTE_C5
  sta API_PARAMETERS + 1
  lda >API_NOTE_C5
  sta API_PARAMETERS + 2
  lda API_GROUP_SOUND
  sta API_COMMAND

  lda <API_NOTE_G4
  sta API_PARAMETERS + 1
  lda >API_NOTE_G4
  sta API_PARAMETERS + 2
  lda API_GROUP_SOUND
  sta API_COMMAND

  lda <API_NOTE_C4
  sta API_PARAMETERS + 1
  lda >API_NOTE_C4
  sta API_PARAMETERS + 2
  lda API_GROUP_SOUND
  sta API_COMMAND

  ; original tempo
  lda <API_TEMPO_80 / 3
  sta API_PARAMETERS + 3
  lda >API_TEMPO_80 / 3
  sta API_PARAMETERS + 4

  ; two rests
  lda <API_NOTE_REST
  sta API_PARAMETERS + 1
  sta API_PARAMETERS + 2
  lda API_GROUP_SOUND
  sta API_COMMAND
  lda API_GROUP_SOUND
  sta API_COMMAND

  ; slide (dotted quarter)
  lda <API_NOTE_C4
  sta API_PARAMETERS + 1
  lda >API_NOTE_C4
  sta API_PARAMETERS + 2
  lda <API_TEMPO_80
  sta API_PARAMETERS + 3
  lda >API_TEMPO_80
  sta API_PARAMETERS + 4
  lda <API_SLIDE_FAST
  sta API_PARAMETERS + 5
  lda >API_SLIDE_FAST
  sta API_PARAMETERS + 6
  lda API_GROUP_SOUND
  sta API_COMMAND

  ; ending note
  lda <API_NOTE_C5
  sta API_PARAMETERS + 1
  lda >API_NOTE_C5
  sta API_PARAMETERS + 2
  lda <API_SLIDE_NONE
  sta API_PARAMETERS + 5
  lda >API_SLIDE_NONE
  sta API_PARAMETERS + 6
  lda API_GROUP_SOUND
  sta API_COMMAND


end:
  jmp end ; infinite loop
