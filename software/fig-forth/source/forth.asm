
ssize   =       256                             ;  size of disk sector
nbuf    =       2                               ;  no of buffers in RAM
sector  =       400                             ;  no of sects/drive
sectl   =       800                             ;  sector limit 2 drives
bmag    =       $404                            ;  total buffer magnitude

bos     =       $02                             ;  bottom of FORTH stack
tos     =       $70                             ;  top of FORTH stack
n       =       $78                             ;  scratch workspace
ip      =       $80                             ;  interpretive pointer
w       =       $83                             ;  codefield pointer
up      =       $85                             ;  user area pointer
xsave   =       $87                             ;  temp store for X reg

orig    =       $1900                           ;  origin of FORTHs dictionary
mem     =       $5800                           ;  top of assigned memory + 1
uarea   =       $480                            ;  128 bytes of user area
darea   =       $5800                           ;  disk buffer area

rubout  =       $7f                             ;  DEL

tibx    =       $100                            ;  terminal input buffer

;    MOS entry points

osbyte  =       (-12 & $ffff)
osword  =       (-15 & $ffff)
oswrch  =       (-18 & $ffff)
osrdch  =       (-32 & $ffff)
osnewl  =       (-25 & $ffff)
oscli   =       (-9 & $ffff)
osasci  =       (-29 & $ffff)

        * =     $1900

enter:  jmp     cold+2                          ;  cold start

reentr: jmp     warm                            ;  warm start

        .word   $6502                           ;  for 6502
        .word   $0000
        .word   ntop                            ;  top word in FORTH
        .word   rubout
        .word   uarea                           ;  pointer to user area
        .word   tos
        .word   $1ff                            ;  top of return stack
        .word   tibx                            ;  terminal input buffer
        .word   $1f                             ;  initial WIDTH
        .word   $00                             ;  warning : 0=no disk
        .word   top                             ;  initial FENCE
        .word   top                             ;  initial top of dictionary
        .word   vlo                             ;  initial VOC-LINK pointer
        .word   0                               ;  fiddle for JMP W-1

; ********************************************************************************
;
;                                       LIT
;
; ********************************************************************************


l22:    .byte   $83
        .text   'LI'
        .byte   $d4

        .word   0                               ;  bottom word LFA contains 0
lit:    .word   *+2                             ;  CFA points to itself

        lda     (ip),y
        pha
        inc     ip
        bne     l30
        inc     ip+1
l30:    lda     (ip),y
l31:    inc     ip
        bne     push
        inc     ip+1
push:   dex                                     ;  adjust FORTH stack ptr
        dex
put:    sta     1,x                             ;  store (high) byte on FTH stack
        pla
        sta     0,x                             ;   "    (low)   "
next:   ldy     #1
        lda     (ip),y                          ;  fetch CFA pointed to by IP
        sta     w+1
        dey
        lda     (ip),y
        sta     w
        clc
        lda     ip
        adc     #2                              ;  bump IP
        sta     ip
        bcc     l54
        inc     ip+1
l54:    jmp     w-1                             ;  W-1 contains JMP (aaaa)

; ********************************************************************************
;
;                                       CLIT
;
; ********************************************************************************


l35:    .byte   $84
        .text   'CLI'
        .byte   $d4

        .word   l22                             ;  LFA
clit:   .word   *+2                             ;  CFA (points to itself)

        lda     (ip),y
        pha
        tya
        beq     l31                             ;  forced branch into LIT
setup:  asl     a                               ;  A = no of (16-bit) words to be
        sta     n-1                             ;  tfr ed to scratchpad
l63:    lda     0,x                             ;  from FTH stack
        sta     n,y
        inx
        iny
        cpy     n-1                             ;  # of bytes
        bne     l63
        ldy     #0
        rts

; ********************************************************************************
;
;                                     EXECUTE
;
; ********************************************************************************


l75:    .byte   $87
        .text   'EXECUT'
        .byte   $c5

        .word   l35                             ;  LFA
exec:   .word   *+2                             ;  CFA

        lda     0,x                             ;  pokes address from top of
        sta     w                               ;  FTH stack into W
        lda     1,x
        sta     w+1
        inx
        inx
        jmp     w-1                             ;  vector through W

; ********************************************************************************
;
;                                      BRANCH
;
; ********************************************************************************


l89:    .byte   $86
        .text   'BRANC'
        .byte   $c8

        .word   l75                             ;  LFA
branch: .word   *+2                             ;  CFA

        clc
        lda     (ip),y                          ;  adds following (signed) 16-bit
        adc     ip                              ;  value to IP, thus forcing a
        pha                                     ;  relative branch
        iny
        lda     (ip),y
        adc     ip+1
        sta     ip+1
        pla
        sta     ip
        jmp     next+2                          ;  Y already = 1

; ********************************************************************************
;
;                                     0BRANCH
;
; ********************************************************************************


l107:   .byte   $87
        .text   '0BRANC'
        .byte   $c8

        .word   l89                             ;  LFA
zbran:  .word   *+2                             ;  CFA

        inx                                     ;  test top stack item
        inx
        lda     $fe,x                           ;  if false then BRANCH
        ora     $ff,x
        beq     branch+2
bump:   clc                                     ;  else bump IP
        lda     ip
        adc     #2                              ;  by 2
        sta     ip
        bcc     l122
        inc     ip+1
l122:   jmp     next

; ********************************************************************************
;
;                                      (LOOP)
;
; ********************************************************************************


l127:   .byte   $86
        .text   '(LOOP'
        .byte   $a9

        .word   l107                            ;  LFA
ploop:  .word   l130                            ;  CFA

l130:   stx     xsave
        tsx
        inc     $101,x                          ;  bump loop count by 1
        bne     pl1                             ;  (on ret'n stack)
        inc     $102,x                          ;     "
pl1:    clc
        lda     $103,x                          ;  tests loop count vs loop limit
        sbc     $101,x
        lda     $104,x
        sbc     $102,x
pl2:    ldx     xsave
        asl     a
        bcc     branch+2
        pla                                     ;  drop loop parameters
        pla
        pla
        pla
        jmp     bump                            ;  leave loop

; ********************************************************************************
;
;                                     (+LOOP)
;
; ********************************************************************************


l154:   .byte   $87
        .text   '(+LOOP'
        .byte   $a9                             ;  (there is an extra parm. on stack)
; (c.f. (LOOP))
        .word   l127                            ;  LFA
pploo:  .word   *+2                             ;  CFA

        inx
        inx
        stx     xsave
        lda     $ff,x
        pha
        pha
        lda     $fe,x
        tsx
        inx
        inx
        clc
        adc     $101,x                          ;  add increment to loop count
        sta     $101,x
        pla                                     ;  inc. h.
        adc     $102,x
        sta     $102,x
        pla
        bpl     pl1                             ;  full parm comp'son test if inc. +ve
        clc
        lda     $101,x                          ;  reverse comparison
        sbc     $103,x
        lda     $102,x
        sbc     $104,x
        jmp     pl2

; ********************************************************************************
;
;                                       (DO)
;
; ********************************************************************************


l185:   .byte   $84                             ;
        .text   '(DO'
        .byte   $a9                             ;  (transfers loop parameters from)
; (FORTH stack to ret'n stack)
        .word   l154                            ;  LFA
pdo:    .word   *+2                             ;  CFA

        lda     3,x                             ;  loop limit hi
        pha
        lda     2,x                             ;  loop limit lo
        pha
        lda     1,x                             ;  loop start hi
        pha
        lda     0,x                             ;  loop start lo
        pha
poptwo: inx                                     ;  drop FORTH stack item
        inx
pop:    inx                                     ;  drop another FORTH stack item
        inx
        jmp     next

; ********************************************************************************
;
;                                        I
;
; ********************************************************************************


l207:   .byte   $81,$c9                         ;  'I'

        .word   l185                            ;  LFA - copy loop counter to FTH stack
i:      .word   r+2                             ;  CFA - same as 'R'

; ********************************************************************************
;
;                                      DIGIT
;
; ********************************************************************************


l214:   .byte   $85
        .text   'DIGI'                          ;  converts ASCII chr to binary equiv
        .byte   $d4                             ;  in relevant BASE leaving num on
; FTH stack + tf if valid ff only
        .word   l207                            ;  if not valid char
digit:  .word   *+2

        sec
        lda     2,x                             ;  get char
        sbc     #$30                            ;  unprintable ?
        bmi     l234
        cmp     #$a                             ;  0-9 ?
        bmi     l227
        sec
        sbc     #7                              ;  A-F ?
        cmp     #$a
        bmi     l234
l227:   cmp     0,x                             ;  compare with number base
        bpl     l234
        sta     2,x                             ;  number valid - stack it
        lda     #1                              ;  with tf
        pha
        tya
        jmp     put                             ;  exit (true) char valid

l234:   tya
        pha
        inx
        inx
        jmp     put                             ;  exit (false) char invalid

; ********************************************************************************
;
;                                      (FIND)
;
; ********************************************************************************


l243:   .byte   $86                             ;  dictionary search for word
        .text   '(FIND'                         ;  from NFA on top of F. stack
        .byte   $a9                             ;  which matches text at addr.

        .word   l214                            ;  beneath it on stack
pfind:  .word   *+2                             ;  CFA (self)

        lda     #2
        jsr     setup
        stx     xsave
l249:   ldy     #0
        lda     (n),y
        eor     (n+2),y
        and     #$3f
        bne     l281
l254:   iny
        lda     (n),y
        eor     (n+2),y
        asl     a
        bne     l280
        bcc     l254
        ldx     xsave
        dex
        dex
        dex
        dex
        clc
        tya
        adc     #5
        adc     n
        sta     2,x
        ldy     #0
        tya
        adc     n+1
        sta     3,x
        sty     1,x
        lda     (n),y
        sta     0,x
        lda     #1
        pha
        jmp     push                            ;  exit (true)

l280:   bcs     l284
l281:   iny
        lda     (n),y
        bpl     l281
l284:   iny
        lda     (n),y
        tax
        iny
        lda     (n),y
        sta     n+1
        stx     n
        ora     n
        bne     l249
        ldx     xsave
        lda     #0
        pha
        jmp     push                            ;  exit (false)

; ********************************************************************************
;
;                                     ENCLOSE
;
; ********************************************************************************


l301:   .byte   $87
        .text   'ENCLOS'
        .byte   $c5

        .word   l243                            ;  LFA
encl:   .word   *+2                             ;  CFA

        lda     #2
        jsr     setup                           ;  copy 2 words to scratchpad
        txa
        sec
        sbc     #8
        tax                                     ;  bump stack ptr by 8 bytes
        sty     3,x                             ;  Y=0
        sty     1,x
        dey
l313:   iny
        lda     (n+2),y
        cmp     n
        beq     l313
        sty     4,x
l318:   lda     (n+2),y
        bne     l327
        sty     2,x
        sty     0,x
        tya
        cmp     4,x
        bne     l326
        inc     2,x
l326:   jmp     next

l327:   sty     2,x
        iny
        cmp     n
        bne     l318
        sty     0,x
        jmp     next

; ********************************************************************************
;
;                                       EMIT
;
; ********************************************************************************


l337:   .byte   $84
        .text   'EMI'
        .byte   $d4
        .word   l301                            ;  LFA

emit:   .word   xemit                           ;  vectored

; ********************************************************************************
;
;                                       KEY
;
; ********************************************************************************


l344:   .byte   $83
        .text   'KE'
        .byte   $d9

        .word   l337                            ;  LFA

key:    .word   xkey                            ;  vectored

; ********************************************************************************
;
;                                    ?TERMINAL
;
; ********************************************************************************


l351:   .byte   $89
        .text   '?TERMINA'
        .byte   $cc

        .word   l344                            ;  LFA
qterm:  .word   xqter                           ;  vectored

; ********************************************************************************
;
;                                        CR
;
; ********************************************************************************


l358:   .byte   $82
        .text   'C'
        .byte   $d2

        .word   l351                            ;  LFA
cr:     .word   xcr                             ;  vectored

; ********************************************************************************
;
;                                      CMOVE
;
; ********************************************************************************


l365:   .byte   $85
        .text   'CMOV'
        .byte   $c5

        .word   l358                            ;  LFA
cmove:  .word   *+2                             ;  CFA

        lda     #3
        jsr     setup
l370:   cpy     n
        bne     l375
        dec     n+1
        bpl     l375
        jmp     next                            ;  finished

l375:   lda     (n+4),y
        sta     (n+2),y
        iny
        bne     l370
        inc     n+5
        inc     n+3
        jmp     l370

; ********************************************************************************
;
;                                        U*
;
; ********************************************************************************


l386:   .byte   $82
        .text   'U'
        .byte   $aa

        .word   l365                            ;  LFA
ustar:  .word   *+2                             ;  CFA

        lda     2,x
        sta     n
        sta     2,x
        lda     3,x
        sta     n+1
        sty     3,x
        ldy     #16
l396:   asl     2,x
        rol     3,x
        rol     0,x
        rol     1,x
        bcc     l411
        clc
        lda     n
        adc     2,x
        sta     2,x
        lda     n+1
        adc     3,x
        sta     3,x
        lda     #0
        adc     0,x
        sta     0,x
l411:   dey
        bne     l396
        jmp     next

; ********************************************************************************
;
;                                        U/
;
; ********************************************************************************


l418:   .byte   $82
        .text   'U'
        .byte   $af

        .word   l386                            ;  LFA
uslash: .word   *+2                             ;  CFA

        lda     4,x
        ldy     2,x
        sty     4,x
        asl     a
        sta     2,x
        lda     5,x
        ldy     3,x
        sty     5,x
        rol     a
        sta     3,x
        lda     #16
        sta     n
l433:   rol     4,x
        rol     5,x
        sec
        lda     4,x
        sbc     0,x
        tay
        lda     5,x
        sbc     1,x
        bcc     l444
        sty     4,x
        sta     5,x
l444:   rol     2,x
        rol     3,x
        dec     n
        bne     l433
        jmp     pop

; ********************************************************************************
;
;                                       AND
;
; ********************************************************************************


l453:   .byte   $83
        .text   'AN'
        .byte   $c4

        .word   l418                            ;  LFA
andd:   .word   *+2                             ;  CFA

        lda     0,x
        and     2,x
        pha
        lda     1,x
        and     3,x

binary: inx
        inx
        jmp     put

; ********************************************************************************
;
;                                        OR
;
; ********************************************************************************


l469:   .byte   $82
        .text   'O'
        .byte   $d2

        .word   l453                            ;  LFA
or:     .word   *+2                             ;  CFA

        lda     0,x
        ora     2,x
        pha
        lda     1,x
        ora     3,x
        inx
        inx
        jmp     put

; ********************************************************************************
;
;                                       XOR
;
; ********************************************************************************


l484:   .byte   $83
        .text   'XO'
        .byte   $d2

        .word   l469                            ;  LFA
xor:    .word   *+2                             ;  CFA

        lda     0,x
        eor     2,x
        pha
        lda     1,x
        eor     3,x
        inx
        inx
        jmp     put

; ********************************************************************************
;
;                                       SP@
;
; ********************************************************************************


l499:   .byte   $83
        .text   'SP'
        .byte   $c0

        .word   l484                            ;  LFA
spat:   .word   *+2                             ;  CFA

        txa
push0a: pha
        lda     #0
        jmp     push

; ********************************************************************************
;
;                                       SP!
;
; ********************************************************************************


l511:   .byte   $83
        .text   'SP'
        .byte   $a1

        .word   l499                            ;  LFA
spsto:  .word   *+2                             ;  CFA

        ldy     #6
        lda     (up),y
        clc                                     ;  MJR
        adc     #2                              ;  MJR
        tax
        jmp     next

; ********************************************************************************
;
;                                       RP!
;
; ********************************************************************************


l522:   .byte   $83
        .text   'RP'
        .byte   $a1

        .word   l511                            ;  LFA
rpsto:  .word   *+2                             ;  CFA

        stx     xsave
        ldy     #8
        lda     (up),y
        tax
        txs
        ldx     xsave
        jmp     next

; ********************************************************************************
;
;                                        ;S
;
; ********************************************************************************


l536:   .byte   $82
        .text   '                               ; '
        .byte   $d3

        .word   l522
semis:  .word   *+2

        pla
        sta     ip
        pla
        sta     ip+1
        jmp     next

; ********************************************************************************
;
;                                      LEAVE
;
; ********************************************************************************


l548:   .byte   $85
        .text   'LEAV'
        .byte   $c5

        .word   l536
leave:  .word   *+2

        stx     xsave
        tsx
        lda     $101,x
        sta     $103,x
        lda     $102,x
        sta     $104,x
        ldx     xsave
        jmp     next

; ********************************************************************************
;
;                                        >R
;
; ********************************************************************************


l563:   .byte   $82
        .text   '>'
        .byte   $d2

        .word   l548                            ;  LFA
tor:    .word   *+2                             ;  CFA

        lda     1,x
        pha
        lda     0,x
        pha
        inx
        inx
        jmp     next

; ********************************************************************************
;
;                                        R>
;
; ********************************************************************************


l577:   .byte   $82
        .text   'R'
        .byte   $be

        .word   l563                            ;  LFA
rfrom:  .word   *+2                             ;  CFA

        dex
        dex
        pla
        sta     0,x
        pla
        sta     1,x
        jmp     next

; ********************************************************************************
;
;                                        R
;
; ********************************************************************************


l591:   .byte   $81,$d2

        .word   l577                            ;  LFA
r:      .word   *+2                             ;  CFA

        stx     xsave                           ;  copy
        tsx                                     ;  top of
        lda     $101,x                          ;  m/c stack
        pha                                     ;  to
        lda     $102,x                          ;  4th stack
        ldx     xsave                           ;  = 'I'
        jmp     push

; ********************************************************************************
;
;                                        0=
;
; ********************************************************************************


l605:   .byte   $82
        .text   '0'
        .byte   $bd

        .word   l591                            ;  LFA
zequ:   .word   *+2                             ;  CFA

        lda     0,x
        ora     1,x
        sty     1,x
        bne     l613
        iny
l613:   sty     0,x
        jmp     next

; ********************************************************************************
;
;                                        0<
;
; ********************************************************************************


l619:   .byte   $82
        .text   '0'
        .byte   $bc

        .word   l605                            ;  LFA
zless:  .word   *+2                             ;  CFA

        asl     1,x                             ;  leave true
        tya                                     ;  if BOS
        rol     a                               ;  -ve else
        sty     1,x                             ;  leave false
        sta     0,x
        jmp     next

; ********************************************************************************
;
;                                        +
;
; ********************************************************************************


l632:   .byte   $81,$ab

        .word   l619                            ;  LFA
plus:   .word   *+2                             ;  CFA

        clc
        lda     0,x
        adc     2,x
        sta     2,x
        lda     1,x
        adc     3,x
        sta     3,x
        inx
        inx
        jmp     next

; ********************************************************************************
;
;                                        D+
;
; ********************************************************************************


l649:   .byte   $82
        .text   'D'
        .byte   $ab

        .word   l632                            ;  LFA
dplus:  .word   *+2                             ;  CFA

        clc
        lda     2,x
        adc     6,x
        sta     6,x
        lda     3,x
        adc     7,x
        sta     7,x
        lda     0,x
        adc     4,x
        sta     4,x
        lda     1,x
        adc     5,x
        sta     5,x
        jmp     poptwo

; ********************************************************************************
;
;                                      MINUS
;
; ********************************************************************************


l670:   .byte   $85
        .text   'MINU'
        .byte   $d3

        .word   l649                            ;  LFA
minus:  .word   *+2                             ;  CFA

        sec
        tya
        sbc     0,x                             ;  leave
        sta     0,x                             ;  2's compliment
        tya                                     ;  of BOS
        sbc     1,x
        sta     1,x
        jmp     next

; ********************************************************************************
;
;                                      DMINUS
;
; ********************************************************************************


l685:   .byte   $86
        .text   'DMINU'
        .byte   $d3

        .word   l670                            ;  LFA
dminus: .word   *+2                             ;  CFA

        sec
        tya
        sbc     2,x
        sta     2,x
        tya
        sbc     3,x
        sta     3,x
        jmp     minus+3

; ********************************************************************************
;
;                                       OVER
;
; ********************************************************************************


l700:   .byte   $84
        .text   'OVE'
        .byte   $d2

        .word   l685                            ;  LFA
over:   .word   *+2                             ;  CFA

        lda     2,x
        pha
        lda     3,x
        jmp     push

; ********************************************************************************
;
;                                       DROP
;
; ********************************************************************************


l711:   .byte   $84
        .text   'DRO'
        .byte   $d0

        .word   l700                            ;  LFA
drop:   .word   pop                             ;  CFA

; ********************************************************************************
;
;                                       SWAP
;
; ********************************************************************************


l718:   .byte   $84
        .text   'SWA'
        .byte   $d0

        .word   l711                            ;  LFA
swap:   .word   *+2

        lda     2,x
        pha
        lda     0,x
        sta     2,x
        lda     3,x
        ldy     1,x
        sty     3,x
        jmp     put

; ********************************************************************************
;
;                                       DUP
;
; ********************************************************************************


l733:   .byte   $83
        .text   'DU'
        .byte   $d0

        .word   l718                            ;  LFA
dup:    .word   *+2                             ;  CFA

        lda     0,x
        pha
        lda     1,x
        jmp     push

; ********************************************************************************
;
;                                        +!
;
; ********************************************************************************


l744:   .byte   $82
        .text   '+'
        .byte   $a1

        .word   l733                            ;  LFA
pstore: .word   *+2

        clc
        lda     (0,x)
        adc     2,x
        sta     (0,x)
        inc     0,x
        bne     l754
        inc     1,x
l754:   lda     (0,x)
        adc     3,x
        sta     (0,x)
        jmp     poptwo

; ********************************************************************************
;
;                                      TOGGLE
;
; ********************************************************************************


l762:   .byte   $86
        .text   'TOGGL'
        .byte   $c5

        .word   l744                            ;  LFA
toggle: .word   *+2                             ;  CFA

        lda     (2,x)
        eor     0,x
        sta     (2,x)
        jmp     poptwo

; ********************************************************************************
;
;                                        @
;
; ********************************************************************************


l773:   .byte   $81,$c0

        .word   l762                            ;  LFA
at:     .word   *+2                             ;  CFA

        lda     (0,x)
        pha
        inc     0,x
        bne     l781
        inc     1,x
l781:   lda     (0,x)
        jmp     put

; ********************************************************************************
;
;                                        C@
;
; ********************************************************************************


l787:   .byte   $82
        .text   'C'
        .byte   $c0

        .word   l773                            ;  LFA
cat:    .word   *+2                             ;  CFA

        lda     (0,x)
        sta     0,x
        sty     1,x
        jmp     next

; ********************************************************************************
;
;                                        !
;
; ********************************************************************************


l798:   .byte   $81,$a1

        .word   l787                            ;  LFA
store:  .word   *+2                             ;  CFA

        lda     2,x
        sta     (0,x)
        inc     0,x
        bne     l806
        inc     1,x
l806:   lda     3,x
        sta     (0,x)
        jmp     poptwo

; ********************************************************************************
;
;                                        C!
;
; ********************************************************************************


l813:   .byte   $82
        .text   'C'
        .byte   $a1

        .word   l798                            ;  LFA
cstore: .word   *+2                             ;  CFA

        lda     2,x
        sta     (0,x)
        jmp     poptwo

; ********************************************************************************
;
;                                        :
;
; ********************************************************************************


l823:   .byte   $c1,$ba

        .word   l813                            ;  LFA
colon:  .word   docol                           ;  CFA

        .word   qexec
        .word   scsp
        .word   curr
        .word   at
        .word   con
        .word   store
        .word   create
        .word   rbrack
        .word   pscod

docol:  lda     ip+1
        pha
        lda     ip
        pha
        clc
        lda     w
        adc     #2
        sta     ip
        tya
        adc     w+1
        sta     ip+1
        jmp     next

; ********************************************************************************
;
;                                        ;
;
; ********************************************************************************


l853:   .byte   $c1,$bb

        .word   l823                            ;  LFA
        .word   docol                           ;  CFA

        .word   qcsp
        .word   comp
        .word   semis
        .word   smudge
        .word   lbrack
        .word   semis

; ********************************************************************************
;
;                                     CONSTANT
;
; ********************************************************************************


l867:   .byte   $88
        .text   'CONSTAN'
        .byte   $d4

        .word   l853                            ;  LFA
const:  .word   docol                           ;  CFA

        .word   create
        .word   smudge
        .word   comma
        .word   pscod

docon:  ldy     #2
        lda     (w),y
        pha
        iny
        lda     (w),y
        jmp     push

; ********************************************************************************
;
;                                     VARIABLE
;
; ********************************************************************************


l885:   .byte   $88
        .text   'VARIABL'
        .byte   $c5

        .word   l867                            ;  LFA
var:    .word   docol                           ;  CFA

        .word   const
        .word   pscod

dovar:  clc
        lda     w
        adc     #2
        pha
        tya
        adc     w+1
        jmp     push

; ********************************************************************************
;
;                                       USER
;
; ********************************************************************************


l902:   .byte   $84
        .text   'USE'
        .byte   $d2

        .word   l885                            ;  LFA
user:   .word   docol                           ;  CFA

        .word   const
        .word   pscod

douse:  ldy     #2
        clc
        lda     (w),y
        adc     up
        pha
        lda     #0
        adc     up+1
        jmp     push

; ********************************************************************************
;
;                                        0
;
; ********************************************************************************


l920:   .byte   $81,$b0

        .word   l902                            ;  LFA
zero:   .word   docon                           ;  CFA

        .word   0

; ********************************************************************************
;
;                                        1
;
; ********************************************************************************


l928:   .byte   $81,$b1

        .word   l920                            ;  LFA
one:    .word   docon                           ;  CFA

        .word   1

; ********************************************************************************
;
;                                        2
;
; ********************************************************************************


l936:   .byte   $81,$b2

        .word   l928                            ;  LFA
two:    .word   docon                           ;  CFA

        .word   2

; ********************************************************************************
;
;                                        3
;
; ********************************************************************************


l944:   .byte   $81,$b3

        .word   l936                            ;  LFA
three:  .word   docon                           ;  CFA

        .word   3

; ********************************************************************************
;
;                                        BL
;
; ********************************************************************************


l952:   .byte   $82
        .text   'B'
        .byte   $cc

        .word   l944                            ;  LFA
bl:     .word   docon                           ;  CFA

        .word   32                              ;  ASCII blank

; ********************************************************************************
;
;                                       C/L
;
; ********************************************************************************


l960:   .byte   $83
        .text   'C/'
        .byte   $cc

        .word   l952                            ;  LFA
csll:   .word   docon                           ;  CFA

        .word   64                              ;  64 chars/line

        .word   semis                           ;  MJR - padding

; ********************************************************************************
;
;                                      FIRST
;
; ********************************************************************************


l968:   .byte   $85
        .text   'FIRS'
        .byte   $d4

        .word   l960                            ;  LFA
first:  .word   docon                           ;  CFA

        .word   darea                           ;  bottom of disk
; buffer

; ********************************************************************************
;
;                                      LIMIT
;
; ********************************************************************************


l976:   .byte   $85
        .text   'LIMI'
        .byte   $d4

        .word   l968                            ;  LFA
limit:  .word   docon                           ;  CFA

        .word   $5800                           ;  end of buffers-see Harrison

; ********************************************************************************
;
;                                      B/BUF
;
; ********************************************************************************


l984:   .byte   $85
        .text   'B/BU'
        .byte   $c6

        .word   l976                            ;  LFA
bbuf:   .word   docon                           ;  CFA

        .word   256                             ;  sector size

; ********************************************************************************
;
;                                      B/SCR
;
; ********************************************************************************


l992:   .byte   $85
        .text   'B/SC'
        .byte   $d2

        .word   l984                            ;  LFA
bscr:   .word   docon                           ;  CFA

        .word   4                               ;  blocks per screen

l1000:  .byte   $87
        .text   '+ORIGI'
        .byte   $ce

        .word   l992                            ;  LFA
porig:  .word   docol                           ;  CFA

        .word   lit
        .word   orig
        .word   plus
        .word   semis

; ********************************************************************************
;
;                                       TIB
;
; ********************************************************************************


l1010:  .byte   $83
        .text   'TI'
        .byte   $c2

        .word   l1000                           ;  LFA
tib:    .word   douse                           ;  CFA

        .byte   $a

; ********************************************************************************
;
;                                      WIDTH
;
; ********************************************************************************


l1018:  .byte   $85
        .text   'WIDT'
        .byte   $c8

        .word   l1010                           ;  LFA
width:  .word   douse                           ;  CFA

        .byte   $c

; ********************************************************************************
;
;                                     WARNING
;
; ********************************************************************************


l1026:  .byte   $87
        .text   'WARNIN'
        .byte   $c7

        .word   l1018                           ;  LFA
warn:   .word   douse                           ;  CFA

        .byte   $e

; ********************************************************************************
;
;                                      FENCE
;
; ********************************************************************************


l1034:  .byte   $85
        .text   'FENC'
        .byte   $c5

        .word   l1026                           ;  LFA
fence:  .word   douse                           ;  CFA

        .byte   $10

; ********************************************************************************
;
;                                        DP
;
; ********************************************************************************


l1042:  .byte   $82
        .text   'D'
        .byte   $d0

        .word   l1034                           ;  LFA
dp:     .word   douse                           ;  CFA

        .byte   $12

; ********************************************************************************
;
;                                     VOC-LINK
;
; ********************************************************************************


l1050:  .byte   $88
        .text   'VOC-LIN'
        .byte   $cb

        .word   l1042                           ;  LFA
voclnk: .word   douse                           ;  CFA

        .byte   $14

; ********************************************************************************
;
;                                       BLK
;
; ********************************************************************************


l1058:  .byte   $83
        .text   'BL'
        .byte   $cb

        .word   l1050                           ;  LFA
blk:    .word   douse                           ;  CFA

        .byte   $16

; ********************************************************************************
;
;                                        IN
;
; ********************************************************************************


l1066:  .byte   $82
        .text   'I'
        .byte   $ce

        .word   l1058                           ;  LFA
in:     .word   douse                           ;  CFA

        .byte   $18

; ********************************************************************************
;
;                                       OUT
;
; ********************************************************************************


l1074:  .byte   $83
        .text   'OU'
        .byte   $d4

        .word   l1066                           ;  LFA
out:    .word   douse                           ;  CFA

        .byte   $1a

; ********************************************************************************
;
;                                       SCR
;
; ********************************************************************************


l1082:  .byte   $83
        .text   'SC'
        .byte   $d2

        .word   l1074                           ;  LFA
scr:    .word   douse                           ;  CFA

        .byte   $1c

; ********************************************************************************
;
;                                      OFFSET
;
; ********************************************************************************


l1090:  .byte   $86
        .text   'OFFSE'
        .byte   $d4

        .word   l1082                           ;  LFA
offset: .word   douse                           ;  CFA

        .byte   $1e

; ********************************************************************************
;
;                                     CONTEXT
;
; ********************************************************************************


l1098:  .byte   $87
        .text   'CONTEX'
        .byte   $d4

        .word   l1090                           ;  LFA
con:    .word   douse                           ;  CFA

        .byte   $20

; ********************************************************************************
;
;                                     CURRENT
;
; ********************************************************************************


l1106:  .byte   $87
        .text   'CURREN'
        .byte   $d4

        .word   l1098                           ;  LFA
curr:   .word   douse                           ;  CFA

        .byte   $22

; ********************************************************************************
;
;                                      STATE
;
; ********************************************************************************


l1114:  .byte   $85
        .text   'STAT'
        .byte   $c5

        .word   l1106                           ;  LFA
state:  .word   douse                           ;  CFA

        .byte   $24

; ********************************************************************************
;
;                                       BASE
;
; ********************************************************************************


l1122:  .byte   $84
        .text   'BAS'
        .byte   $c5

        .word   l1114                           ;  LFA
base:   .word   douse                           ;  CFA

        .byte   $26

; ********************************************************************************
;
;                                       DPL
;
; ********************************************************************************


l1130:  .byte   $83
        .text   'DP'
        .byte   $cc

        .word   l1122                           ;  LFA
dpl:    .word   douse                           ;  CFA

        .byte   $28

; ********************************************************************************
;
;                                       FLD
;
; ********************************************************************************


l1138:  .byte   $83
        .text   'FL'
        .byte   $c4

        .word   l1130                           ;  LFA
fld:    .word   douse                           ;  CFA

        .byte   $2a

; ********************************************************************************
;
;                                       CSP
;
; ********************************************************************************


l1146:  .byte   $83
        .text   'CS'
        .byte   $d0

        .word   l1138                           ;  LFA
csp:    .word   douse                           ;  CFA

        .byte   $2c

; ********************************************************************************
;
;                                        R#
;
; ********************************************************************************


l1154:  .byte   $82
        .text   'R'
        .byte   $a3

        .word   l1146                           ;  LFA
rnum:   .word   douse                           ;  CFA

        .byte   $2e

; ********************************************************************************
;
;                                       HLD
;
; ********************************************************************************


l1162:  .byte   $83
        .text   'HL'
        .byte   $c4

        .word   l1154                           ;  LFA
hld:    .word   douse                           ;  CFA

        .byte   $30

; ********************************************************************************
;
;                                        1+
;
; ********************************************************************************


l1170:  .byte   $82
        .text   '1'
        .byte   $ab

        .word   l1162                           ;  LFA
onep:   .word   docol                           ;  CFA

        .word   one
        .word   plus
        .word   semis

; ********************************************************************************
;
;                                        2+
;
; ********************************************************************************


l1180:  .byte   $82
        .text   '2'
        .byte   $ab

        .word   l1170                           ;  LFA
twop:   .word   docol                           ;  CFA

        .word   two
        .word   plus
        .word   semis

; ********************************************************************************
;
;                                       HERE
;
; ********************************************************************************


l1190:  .byte   $84
        .text   'HER'
        .byte   $c5

        .word   l1180                           ;  LFA
here:   .word   docol                           ;  CFA

        .word   dp
        .word   at
        .word   semis

; ********************************************************************************
;
;                                      ALLOT
;
; ********************************************************************************


l1200:  .byte   $85
        .text   'ALLO'
        .byte   $d4

        .word   l1190                           ;  LFA
allot:  .word   docol                           ;  CFA

        .word   dp
        .word   pstore
        .word   semis

; ********************************************************************************
;
;                                        ,
;
; ********************************************************************************


l1210:  .byte   $81,$ac

        .word   l1200                           ;  LFA
comma:  .word   docol                           ;  CFA

        .word   here
        .word   store
        .word   two
        .word   allot
        .word   semis

; ********************************************************************************
;
;                                        C,
;
; ********************************************************************************


l1222:  .byte   $82
        .text   'C'
        .byte   $ac

        .word   l1210                           ;  LFA
ccomma: .word   docol                           ;  CFA

        .word   here
        .word   cstore
        .word   one
        .word   allot
        .word   semis

; ********************************************************************************
;
;                                        -
;
; ********************************************************************************


l1234:  .byte   $81,$ad

        .word   l1222                           ;  LFA
sub:    .word   docol                           ;  CFA

        .word   minus
        .word   plus
        .word   semis

; ********************************************************************************
;
;                                        =
;
; ********************************************************************************


l1244:  .byte   $81,$bd

        .word   l1234                           ;  LFA
equals: .word   docol                           ;  CFA

        .word   sub
        .word   zequ
        .word   semis

; ********************************************************************************
;
;                                        U<
;
; ********************************************************************************


l1246:  .byte   $82
        .text   'U'
        .byte   $bc

        .word   l1244                           ;  LFA
uless:  .word   docol                           ;  CFA

        .word   sub
        .word   zless
        .word   semis

; ********************************************************************************
;
;                                        <
;
; ********************************************************************************


l1254:  .byte   $81,$bc

        .word   l1246                           ;  LFA
less:   .word   *+2                             ;  CFA

        sec
        lda     2,x
        sbc     0,x
        lda     3,x
        sbc     1,x
        sty     3,x                             ;  zero hi byte
        bvc     l1258
        eor     #$80                            ;  correct o/flow
l1258:  bpl     l1260
        iny                                     ;  invrt flag
l1260:  sty     2,x
        jmp     pop

; ********************************************************************************
;
;                                        >
;
; ********************************************************************************


l1264:  .byte   $81,$be

        .word   l1254                           ;  LFA
great:  .word   docol                           ;  CFA

        .word   swap
        .word   less
        .word   semis

; ********************************************************************************
;
;                                       ROT
;
; ********************************************************************************


l1274:  .byte   $83
        .text   'RO'
        .byte   $d4

        .word   l1264                           ;  LFA
rot:    .word   docol                           ;  CFA

        .word   tor
        .word   swap
        .word   rfrom
        .word   swap
        .word   semis

; ********************************************************************************
;
;                                      SPACE
;
; ********************************************************************************


l1286:  .byte   $85
        .text   'SPAC'
        .byte   $c5

        .word   l1274                           ;  LFA
space:  .word   docol

        .word   bl
        .word   emit
        .word   semis

; ********************************************************************************
;
;                                       -DUP
;
; ********************************************************************************


l1296:  .byte   $84
        .text   '-DU'
        .byte   $d0

        .word   l1286                           ;  LFA
ddup:   .word   docol                           ;  CFA

        .word   dup
        .word   zbran
        .word   4
        .word   dup
        .word   semis

; ********************************************************************************
;
;                                     TRAVERSE
;
; ********************************************************************************


l1308:  .byte   $88
        .text   'TRAVERS'
        .byte   $c5

        .word   l1296                           ;  LFA
trav:   .word   docol                           ;  CFA

        .word   swap
        .word   over
        .word   plus
        .word   clit
        .byte   $7f
        .word   over
        .word   cat
        .word   less
        .word   zbran
        .word   (-15 & $ffff)
        .word   swap
        .word   drop
        .word   semis

; ********************************************************************************
;
;                                      LATEST
;
; ********************************************************************************


l1328:  .byte   $86
        .text   'LATES'
        .byte   $d4

        .word   l1308                           ;  LFA
latest: .word   docol                           ;  CFA

        .word   curr
        .word   at
        .word   at
        .word   semis

; ********************************************************************************
;
;                                       LFA
;
; ********************************************************************************


l1339:  .byte   $83
        .text   'LF'
        .byte   $c1

        .word   l1328                           ;  LFA
lfa:    .word   docol                           ;  CFA

        .word   clit
        .byte   4
        .word   sub
        .word   semis

; ********************************************************************************
;
;                                       CFA
;
; ********************************************************************************


l1350:  .byte   $83
        .text   'CF'
        .byte   $c1

        .word   l1339                           ;  LFA
cfa:    .word   docol                           ;  CFA

        .word   two
        .word   sub
        .word   semis

; ********************************************************************************
;
;                                       NFA
;
; ********************************************************************************


l1360:  .byte   $83
        .text   'NF'
        .byte   $c1

        .word   l1350                           ;  LFA
nfa:    .word   docol                           ;  CFA

        .word   clit
        .byte   5
        .word   sub
        .word   lit
        .word   (-1 & $ffff)
        .word   trav
        .word   semis

; ********************************************************************************
;
;                                       PFA
;
; ********************************************************************************


l1373:  .byte   $83
        .text   'PF'
        .byte   $c1

        .word   l1360                           ;  LFA
pfa:    .word   docol                           ;  CFA

        .word   one
        .word   trav
        .word   clit
        .byte   5
        .word   plus
        .word   semis

; ********************************************************************************
;
;                                       !CSP
;
; ********************************************************************************


l1386:  .byte   $84
        .text   '!CS'
        .byte   $d0

        .word   l1373                           ;  LFA
scsp:   .word   docol                           ;  CFA

        .word   spat
        .word   csp
        .word   store
        .word   semis

; ********************************************************************************
;
;                                      ?ERROR
;
; ********************************************************************************


l1397:  .byte   $86
        .text   '?ERRO'
        .byte   $d2

        .word   l1386                           ;  LFA
qerror: .word   docol                           ;  CFA

        .word   swap
        .word   zbran
        .word   8
        .word   error
        .word   branch
        .word   4
        .word   drop
        .word   semis

; ********************************************************************************
;
;                                      ?COMP
;
; ********************************************************************************


l1412:  .byte   $85
        .text   '?COM'
        .byte   $d0

        .word   l1397                           ;  LFA
qcomp:  .word   docol                           ;  CFA

        .word   state
        .word   at
        .word   zequ
        .word   clit
        .byte   17
        .word   qerror
        .word   semis

; ********************************************************************************
;
;                                      ?EXEC
;
; ********************************************************************************


l1426:  .byte   $85
        .text   '?EXE'
        .byte   $c3

        .word   l1412                           ;  LFA
qexec:  .word   docol                           ;  CFA

        .word   state
        .word   at
        .word   clit
        .byte   18
        .word   qerror
        .word   semis

; ********************************************************************************
;
;                                      ?PAIRS
;
; ********************************************************************************


l1439:  .byte   $85
        .text   '?PAIR'
        .byte   $d3

        .word   l1426                           ;  LFA
qpair:  .word   docol                           ;  CFA

        .word   sub
        .word   clit
        .byte   19
        .word   qerror
        .word   semis

; ********************************************************************************
;
;                                       ?CSP
;
; ********************************************************************************


l1451:  .byte   $84
        .text   '?CS'
        .byte   $d0

        .word   l1439                           ;  LFA
qcsp:   .word   docol                           ;  CFA

        .word   spat
        .word   csp
        .word   at
        .word   sub
        .word   clit
        .byte   20
        .word   qerror
        .word   semis

; ********************************************************************************
;
;                                     ?LOADING
;
; ********************************************************************************


l1466:  .byte   $88
        .text   '?LOADIN'
        .byte   $c7

        .word   l1451                           ;  LFA
qload:  .word   docol                           ;  CFA

        .word   blk
        .word   at
        .word   zequ
        .word   clit
        .byte   22
        .word   qerror
        .word   semis

; ********************************************************************************
;
;                                     COMPILE
;
; ********************************************************************************


l1480:  .byte   $87
        .text   'COMPIL'
        .byte   $c5

        .word   l1466                           ;  LFA
comp:   .word   docol                           ;  CFA

        .word   qcomp
        .word   rfrom
        .word   dup
        .word   twop
        .word   tor
        .word   at
        .word   comma
        .word   semis

; ********************************************************************************
;
;                                        ~[
;
; ********************************************************************************


l1495:  .byte   $81,$db

        .word   l1480                           ;  LFA
lbrack: .word   docol                           ;  CFA

        .word   zero
        .word   state
        .word   store
        .word   semis

; ********************************************************************************
;
;                                        ]
;
; ********************************************************************************


l1507:  .byte   $81,$dd

        .word   l1495                           ;  LFA
rbrack: .word   docol                           ;  CFA

        .word   clit
        .byte   $c0
        .word   state
        .word   store
        .word   semis

; ********************************************************************************
;
;                                      SMUDGE
;
; ********************************************************************************


l1519:  .byte   $86
        .text   'SMUDG'
        .byte   $c5

        .word   l1507                           ;  LFA
smudge: .word   docol                           ;  CFA

        .word   latest
        .word   clit
        .byte   32
        .word   toggle
        .word   semis

; ********************************************************************************
;
;                                       HEX
;
; ********************************************************************************


l1531:  .byte   $83
        .text   'HE'
        .byte   $d8

        .word   l1519                           ;  LFA
hex:    .word   docol                           ;  CFA

        .word   clit
        .byte   16
        .word   base
        .word   store
        .word   semis

; ********************************************************************************
;
;                                     DECIMAL
;
; ********************************************************************************


l1543:  .byte   $87
        .text   'DECIMA'
        .byte   $cc

        .word   l1531                           ;  LFA
decim:  .word   docol                           ;  CFA

        .word   clit
        .byte   10
        .word   base
        .word   store
        .word   semis

; ********************************************************************************
;
;                                     (;CODE)
;
; ********************************************************************************


l1555:  .byte   $87
        .text   '(                              ; COD'
        .byte   $a9

        .word   l1543                           ;  LFA
pscod:  .word   docol                           ;  CFA

        .word   rfrom
        .word   latest
        .word   pfa
        .word   cfa
        .word   store
        .word   semis

; ********************************************************************************
;
;                                      ;CODE
;
; ********************************************************************************


l1568:  .byte   $85
        .text   '                               ; COD'
        .byte   $c5

        .word   l1555                           ;  LFA
        .word   docol
        .word   qcsp
        .word   comp
        .word   pscod
        .word   lbrack
        .word   smudge
        .word   semis

; ********************************************************************************
;
;                                     <BUILDS
;
; ********************************************************************************


l1582:  .byte   $87
        .text   '<BUILD'
        .byte   $d3

        .word   l1568                           ;  LFA
build:  .word   docol                           ;  CFA

        .word   zero
        .word   const
        .word   semis

; ********************************************************************************
;
;                                      DOES>
;
; ********************************************************************************


l1592:  .byte   $85
        .text   'DOES'
        .byte   $be

        .word   l1582                           ;  LFA
does:   .word   docol                           ;  CFA

        .word   rfrom
        .word   latest
        .word   pfa
        .word   store
        .word   pscod

dodoe:  lda     ip+1
        pha
        lda     ip
        pha
        ldy     #2
        lda     (w),y
        sta     ip
        iny
        lda     (w),y
        sta     ip+1
        clc
        lda     w
        adc     #4
        pha
        lda     w+1
        adc     #0
        jmp     push

; ********************************************************************************
;
;                                      COUNT
;
; ********************************************************************************


l1622:  .byte   $85
        .text   'COUN'
        .byte   $d4

        .word   l1592                           ;  LFA
count:  .word   docol                           ;  CFA

        .word   dup
        .word   onep
        .word   swap
        .word   cat
        .word   semis

; ********************************************************************************
;
;                                       TYPE
;
; ********************************************************************************


l1634:  .byte   $84
        .text   'TYP'
        .byte   $c5

        .word   l1622                           ;  LFA
type_w: .word   docol                           ;  CFA

        .word   ddup
        .word   zbran
        .word   24
        .word   over
        .word   plus
        .word   swap
        .word   pdo
        .word   i
        .word   cat
        .word   emit
        .word   ploop
        .word   (-8 & $ffff)
        .word   branch
        .word   4
        .word   drop
        .word   semis

; ********************************************************************************
;
;                                    -TRAILING
;
; ********************************************************************************


l1657:  .byte   $89
        .text   '-TRAILIN'
        .byte   $c7

        .word   l1634                           ;  LFA
dtrai:  .word   docol                           ;  CFA

        .word   dup
        .word   zero
        .word   pdo
        .word   over
        .word   over
        .word   plus
        .word   one
        .word   sub
        .word   cat
        .word   bl
        .word   sub
        .word   zbran
        .word   8
        .word   leave
        .word   branch
        .word   6
        .word   one
        .word   sub
        .word   ploop
        .word   $ffe0
        .word   semis

; ********************************************************************************
;
;                                       (.")
;
; ********************************************************************************


l1685:  .byte   $84
        .text   '(."'
        .byte   $a9

        .word   l1657                           ;  LFA
pdotq:  .word   docol                           ;  CFA

        .word   r
        .word   count
        .word   dup
        .word   onep
        .word   rfrom
        .word   plus
        .word   tor
        .word   type_w
        .word   semis

; ********************************************************************************
;
;                                        ."
;
; ********************************************************************************


l1701:  .byte   $c2
        .text   '.'
        .byte   $a2

        .word   l1685                           ;  LFA
        .word   docol                           ;  CFA

        .word   clit
        .byte   34
        .word   state
        .word   at
        .word   zbran
        .word   20
        .word   comp
        .word   pdotq
        .word   word_w
        .word   here
        .word   cat
        .word   onep
        .word   allot
        .word   branch
        .word   10
        .word   word_w
        .word   here
        .word   count
        .word   type_w
        .word   semis

; ********************************************************************************
;
;                                      EXPECT
;
; ********************************************************************************


l1729:  .byte   $86
        .text   'EXPEC'
        .byte   $d4

        .word   l1701                           ;  LFA
expect: .word   docol                           ;  CFA

        .word   over
        .word   plus
        .word   over
        .word   pdo
        .word   key
        .word   dup
        .word   clit
        .byte   17                              ;  adjust as appropriate
        .word   porig                           ;  rel. NOPS at ORG
        .word   at
        .word   equals
        .word   zbran
        .word   31
        .word   drop
        .word   clit
        .byte   $7f
        .word   over
        .word   i
        .word   equals
        .word   dup
        .word   rfrom
        .word   two
        .word   sub
        .word   plus
        .word   tor
; ********************************************************************************
;
;                                    DW   SUB
;
; ********************************************************************************

        .word   drop                            ;  MJR
        .word   branch
        .word   39
        .word   dup
        .word   clit
        .byte   13
        .word   equals
        .word   zbran
        .word   14
        .word   leave
        .word   drop
        .word   bl
        .word   zero
        .word   branch
        .word   4
        .word   dup
        .word   i
        .word   cstore
        .word   zero
        .word   i
        .word   onep
        .word   store
        .word   emit
        .word   ploop
        .word   $ffa9
        .word   drop
        .word   semis

; ********************************************************************************
;
;                                      QUERY
;
; ********************************************************************************


l1788:  .byte   $85
        .text   'QUER'
        .byte   $d9

        .word   l1729                           ;  LFA
query:  .word   docol                           ;  CFA

        .word   tib
        .word   at
        .word   clit
        .byte   80
        .word   expect
        .word   zero
        .word   in
        .word   store
        .word   semis

; ********************************************************************************
;
;                                   <ASCII NULL>
;
; ********************************************************************************


l1804:  .byte   $c1,$80

        .word   l1788                           ;  LFA
        .word   docol                           ;  CFA

        .word   blk
        .word   at
        .word   zbran
        .word   42
        .word   one
        .word   blk
        .word   pstore
        .word   zero
        .word   in
        .word   store
        .word   blk
        .word   at
        .word   zero
        .word   bscr
        .word   uslash
        .word   drop
        .word   zequ
        .word   zbran
        .word   8
        .word   qexec
        .word   rfrom
        .word   drop
        .word   branch
        .word   6
        .word   rfrom
        .word   drop
        .word   semis

; ********************************************************************************
;
;                                       FILL
;
; ********************************************************************************


l1838:  .byte   $84
        .text   'FIL'
        .byte   $cc

        .word   l1804                           ;  LFA
fill:   .word   docol                           ;  CFA

        .word   swap
        .word   tor
        .word   over
        .word   cstore
        .word   dup
        .word   onep
        .word   rfrom
        .word   one
        .word   sub
        .word   cmove
        .word   semis

; ********************************************************************************
;
;                                      ERASE
;
; ********************************************************************************


l1856:  .byte   $85
        .text   'ERAS'
        .byte   $c5

        .word   l1838                           ;  LFA
erase:  .word   docol                           ;  CFA

        .word   zero
        .word   fill
        .word   semis

; ********************************************************************************
;
;                                      BLANKS
;
; ********************************************************************************


l1866:  .byte   $86
        .text   'BLANK'
        .byte   $d3

        .word   l1856                           ;  LFA
blanks: .word   docol                           ;  CFA

        .word   bl
        .word   fill
        .word   semis

; ********************************************************************************
;
;                                       HOLD
;
; ********************************************************************************


l1876:  .byte   $84
        .text   'HOL'
        .byte   $c4

        .word   l1866                           ;  LFA
hold:   .word   docol                           ;  CFA

        .word   lit
        .word   (-1 & $ffff)
        .word   hld
        .word   pstore
        .word   hld
        .word   at
        .word   cstore
        .word   semis

; ********************************************************************************
;
;                                       PAD
;
; ********************************************************************************


l1890:  .byte   $83
        .text   'PA'
        .byte   $c4

        .word   l1876                           ;  LFA
pad:    .word   docol                           ;  CFA

        .word   here
        .word   clit
        .byte   68
        .word   plus
        .word   semis

; ********************************************************************************
;
;                                      WORD_W
;
; ********************************************************************************


l1902:  .byte   $84
        .text   'WOR'
        .byte   $c4

        .word   l1890                           ;  LFA
word_w: .word   docol                           ;  CFA

        .word   blk
        .word   at
        .word   zbran
        .word   12
        .word   blk
        .word   at
        .word   block
        .word   branch
        .word   6
        .word   tib
        .word   at
        .word   in
        .word   at
        .word   plus
        .word   swap
        .word   encl
        .word   here
        .word   clit
        .byte   34
        .word   blanks
        .word   in
        .word   pstore
        .word   over
        .word   sub
        .word   tor
        .word   r
        .word   here
        .word   cstore
        .word   plus
        .word   here
        .word   onep
        .word   rfrom
        .word   cmove
        .word   semis

; ********************************************************************************
;
;                                      UPPER
;
; ********************************************************************************


l1943:  .byte   $85
        .text   'UPPE'
        .byte   $d2

        .word   l1902                           ;  LFA
upper:  .word   docol                           ;  CFA

        .word   over
        .word   plus
        .word   swap
        .word   pdo
        .word   i
        .word   cat
        .word   clit
        .byte   95
        .word   great
        .word   zbran
        .word   9
        .word   i
        .word   clit
        .byte   32
        .word   toggle
        .word   ploop
        .word   $ffea
        .word   semis

; ********************************************************************************
;
;                                     (NUMBER)
;
; ********************************************************************************


l1968:  .byte   $88
        .text   '(NUMBER'
        .byte   $a9

        .word   l1943                           ;  LFA
pnumb:  .word   docol                           ;  CFA

        .word   onep
        .word   dup
        .word   tor
        .word   cat
        .word   base
        .word   at
        .word   digit
        .word   zbran
        .word   44
        .word   swap
        .word   base
        .word   at
        .word   ustar
        .word   drop
        .word   rot
        .word   base
        .word   at
        .word   ustar
        .word   dplus
        .word   dpl
        .word   at
        .word   onep
        .word   zbran
        .word   8
        .word   one
        .word   dpl
        .word   pstore
        .word   rfrom
        .word   branch
        .word   $ffc6
        .word   rfrom
        .word   semis

; ********************************************************************************
;
;                                      NUMBER
;
; ********************************************************************************


l2007:  .byte   $86
        .text   'NUMBE'
        .byte   $d2

        .word   l1968                           ;  LFA
number: .word   docol                           ;  CFA

        .word   zero
        .word   zero
        .word   rot
        .word   dup
        .word   onep
        .word   cat
        .word   clit
        .byte   45
        .word   equals
        .word   dup
        .word   tor
        .word   plus
        .word   lit
        .word   (-1 & $ffff)
        .word   dpl
        .word   store
        .word   pnumb
        .word   dup
        .word   cat
        .word   bl
        .word   sub
        .word   zbran
        .word   21
        .word   dup
        .word   cat
        .word   clit
        .byte   46
        .word   sub
        .word   zero
        .word   qerror
        .word   zero
        .word   branch
        .word   $ffdd
        .word   drop
        .word   rfrom
        .word   zbran
        .word   4
        .word   dminus
        .word   semis

; ********************************************************************************
;
;                                      -FIND
;
; ********************************************************************************


l2052:  .byte   $85
        .text   '-FIN'
        .byte   $c4

        .word   l2007                           ;  LFA
dfind:  .word   docol                           ;  CFA

        .word   bl
        .word   word_w
        .word   here
        .word   count
        .word   upper
        .word   here
        .word   con
        .word   at
        .word   at
        .word   pfind
        .word   dup
        .word   zequ
        .word   zbran
        .word   $a
        .word   drop
        .word   here
        .word   latest
        .word   pfind
        .word   semis

; ********************************************************************************
;
;                                     (ABORT)
;
; ********************************************************************************


l2078:  .byte   $87
        .text   '(ABORT'
        .byte   $a9

        .word   l2052                           ;  LFA
pabort: .word   docol                           ;  CFA

        .word   abort
        .word   semis

; ********************************************************************************
;
;                                      ERROR
;
; ********************************************************************************


l2087:  .byte   $85
        .text   'ERRO'
        .byte   $d2

        .word   l2078                           ;  LFA
error:  .word   docol                           ;  CFA

        .word   warn
        .word   at
        .word   zless
        .word   zbran
        .word   4
        .word   pabort
        .word   here
        .word   count
        .word   type_w
        .word   pdotq
        .byte   4
        .text   '  ? '
        .word   mess
        .word   spsto
        .word   drop
        .word   drop                            ;  make room
        .word   in                              ;  for 2 error
        .word   at                              ;  values
        .word   blk
        .word   at
        .word   quit
        .word   semis

; ********************************************************************************
;
;                                       ID.
;
; ********************************************************************************


l2113:  .byte   $83
        .text   'ID'
        .byte   $ae

        .word   l2087                           ;  LFA
iddot:  .word   docol                           ;  CFA

        .word   pad
        .word   clit
        .byte   32
        .word   clit
        .byte   95
        .word   fill
        .word   dup
        .word   pfa
        .word   lfa
        .word   over
        .word   sub
        .word   pad
        .word   swap
        .word   cmove
        .word   pad
        .word   count
        .word   clit
        .byte   31
        .word   andd
        .word   type_w
        .word   space
        .word   semis

; ********************************************************************************
;
;                                      CREATE
;
; ********************************************************************************


l2142:  .byte   $86
        .text   'CREAT'
        .byte   $c5

        .word   l2113                           ;  LFA
create: .word   docol                           ;  CFA

        .word   first                           ;  ensure
        .word   here                            ;  room
        .word   clit                            ;  exists
        .byte   $a0                             ;  in
        .word   plus                            ;  diction'y
        .word   uless
        .word   two
        .word   qerror
        .word   dfind
        .word   zbran
        .word   $f
        .word   drop
        .word   nfa
        .word   iddot
        .word   clit
        .byte   4
        .word   mess
        .word   space
        .word   here
        .word   dup
        .word   cat
        .word   width
        .word   at
        .word   min
        .word   onep
        .word   allot
        .word   dp                              ;  code
        .word   cat                             ;  field
        .word   clit                            ;  mustn't
        .byte   $fd                             ;  cross
        .word   equals                          ;  page
        .word   allot                           ;  boundary
        .word   dup
        .word   clit
        .byte   $a0
        .word   toggle
        .word   here
        .word   one
        .word   sub
        .word   clit
        .byte   $80
        .word   toggle
        .word   latest
        .word   comma
        .word   curr
        .word   at
        .word   store
        .word   here
        .word   twop
        .word   comma
        .word   semis

; ********************************************************************************
;
;                                    ~[COMPILE]
;
; ********************************************************************************


l2200:  .byte   $c9
        .text   '~[COMPILE'
        .byte   $dd

        .word   l2142                           ;  LFA
        .word   docol                           ;  CFA

        .word   dfind
        .word   zequ
        .word   zero
        .word   qerror
        .word   drop
        .word   cfa
        .word   comma
        .word   semis

; ********************************************************************************
;
;                                     LITERAL
;
; ********************************************************************************


l2217:  .byte   $c7
        .text   'LITERA'
        .byte   $cc

        .word   l2200                           ;  LFA
liter:  .word   docol                           ;  CFA

        .word   state
        .word   at
        .word   zbran
        .word   8
        .word   comp
        .word   lit
        .word   comma
        .word   semis

; ********************************************************************************
;
;                                     DLITERAL
;
; ********************************************************************************


l2232:  .byte   $c8
        .text   'DLITERA'
        .byte   $cc

        .word   l2217                           ;  LFA
dlit:   .word   docol                           ;  CFA

        .word   state
        .word   at
        .word   zbran
        .word   8
        .word   swap
        .word   liter
        .word   liter
        .word   semis

; ********************************************************************************
;
;                                      ?STACK
;
; ********************************************************************************


l2248:  .byte   $86
        .text   '?STAC'
        .byte   $cb

        .word   l2232                           ;  LFA
qstack: .word   docol                           ;  CFA

        .word   clit
        .byte   tos
        .word   spat
        .word   uless
        .word   one
        .word   qerror
        .word   spat
        .word   clit
        .byte   bos
        .word   uless
        .word   clit
        .byte   7
        .word   qerror
        .word   semis

; ********************************************************************************
;
;                                    INTERPRET
;
; ********************************************************************************


l2269:  .byte   $89
        .text   'INTERPRE'
        .byte   $d4

        .word   l2248                           ;  LFA
inter:  .word   docol                           ;  CFA

        .word   dfind
        .word   zbran
        .word   30
        .word   state
        .word   at
        .word   less
        .word   zbran
        .word   $a
        .word   cfa
        .word   comma
        .word   branch
        .word   6
        .word   cfa
        .word   exec
        .word   qstack
        .word   branch
        .word   28
        .word   here
        .word   number
        .word   dpl
        .word   at
        .word   onep
        .word   zbran
        .word   8
        .word   dlit
        .word   branch
        .word   6
        .word   drop
        .word   liter
        .word   qstack
        .word   branch
        .word   $ffc2

; ********************************************************************************
;
;                                    IMMEDIATE
;
; ********************************************************************************


l2309:  .byte   $89
        .text   'IMMEDIAT'
        .byte   $c5

        .word   l2269                           ;  LFA
        .word   docol                           ;  CFA

        .word   latest
        .word   clit
        .byte   64
        .word   toggle
        .word   semis

; ********************************************************************************
;
;                                    VOCABULARY
;
; ********************************************************************************


l2321:  .byte   $8a
        .text   'VOCABULAR'
        .byte   $d9

        .word   l2309                           ;  LFA
        .word   docol                           ;  CFA

        .word   build
        .word   lit
        .word   $a081
        .word   comma
        .word   curr
        .word   at
        .word   cfa
        .word   comma
        .word   here
        .word   voclnk
        .word   at
        .word   comma
        .word   voclnk
        .word   store
        .word   does
dovoc:  .word   twop
        .word   con
        .word   store
        .word   semis

; ********************************************************************************
;
;                                      FORTH
;
; ********************************************************************************


l2346:  .byte   $85
        .text   'FORT'
        .byte   $c8

        .word   l2321                           ;  LFA
forth:  .word   dodoe                           ;  CFA

        .word   dovoc
        .word   $a081
xfor:   .word   ntop
vlo:    .word   0

; ********************************************************************************
;
;                                   DEFINITIONS
;
; ********************************************************************************


l2357:  .byte   $8b
        .text   'DEFINITION'
        .byte   $d3

        .word   l2346                           ;  LFA
defin:  .word   docol                           ;  CFA

        .word   con
        .word   at
        .word   curr
        .word   store
        .word   semis

; ********************************************************************************
;
;                                        (
;
; ********************************************************************************


l2369:  .byte   $c1,$a8

        .word   l2357                           ;  LFA
        .word   docol                           ;  CFA

        .word   clit
        .byte   41
        .word   word_w
        .word   semis

; ********************************************************************************
;
;                                       QUIT
;
; ********************************************************************************


l2381:  .byte   $84
        .text   'QUI'
        .byte   $d4

        .word   l2369                           ;  LFA
quit:   .word   docol                           ;  CFA

        .word   zero
        .word   blk
        .word   store
        .word   lbrack
        .word   rpsto
        .word   cr
        .word   query
        .word   inter
        .word   state
        .word   at
        .word   zequ
        .word   zbran
        .word   9
        .word   pdotq
        .byte   4
        .text   ' ok '
        .word   branch
        .word   (-25 & $ffff)
        .word   semis

; ********************************************************************************
;
;                                      ABORT
;
; ********************************************************************************


l2406:  .byte   $85
        .text   'ABOR'
        .byte   $d4

        .word   l2381                           ;  LFA
abort:  .word   docol                           ;  CFA

        .word   spsto
        .word   decim
        .word   cr
        .word   pdotq
        .byte   14
        .text   'FIG-Forth V1.0'
        .word   cr
        .word   forth
        .word   defin
        .word   quit

; ********************************************************************************
;
;                                       COLD
;
; ********************************************************************************


l2423:  .byte   $84
        .text   'COL'
        .byte   $c4

        .word   l2406                           ;  LFA
cold:   .word   *+2                             ;  CFA

        lda     orig+15                         ;  from cold start area
        sta     forth+6
        lda     orig+16
        sta     forth+7
        ldy     #21
        bne     l2433
warm:   ldy     #15
l2433:  lda     orig+19
        sta     up
        lda     orig+20
        sta     up+1
l2437:  lda     orig+15,y
        sta     (up),y
        dey
        bpl     l2437
        lda     #<abort
        sta     ip+1
        lda     #>abort+2
        sta     ip
        cld
        lda     #$6c
        sta     w-1
        jmp     rpsto+2

; ********************************************************************************
;
;                                       S->D
;
; ********************************************************************************


l2453:  .byte   $84
        .text   'S->'
        .byte   $c4

        .word   l2423                           ;  LFA
stod:   .word   docol                           ;  CFA

        .word   dup
        .word   zless
        .word   minus
        .word   semis

; ********************************************************************************
;
;                                        +-
;
; ********************************************************************************


l2464:  .byte   $82
        .text   '+'
        .byte   $ad

        .word   l2453                           ;  LFA
pm:     .word   docol

        .word   zless
        .word   zbran
        .word   4
        .word   minus
        .word   semis

; ********************************************************************************
;
;                                       D+-
;
; ********************************************************************************


l2476:  .byte   $83
        .text   'D+'
        .byte   $ad

        .word   l2464                           ;  LFA
dpm:    .word   docol                           ;  CFA

        .word   zless
        .word   zbran
        .word   4
        .word   dminus
        .word   semis

; ********************************************************************************
;
;                                       ABS
;
; ********************************************************************************


l2488:  .byte   $83
        .text   'AB'
        .byte   $d3

        .word   l2476                           ;  LFA
abs_w:  .word   docol                           ;  CFA

        .word   dup
        .word   pm
        .word   semis

; ********************************************************************************
;
;                                       DABS
;
; ********************************************************************************


l2498:  .byte   $84
        .text   'DAB'
        .byte   $d3

        .word   l2488                           ;  LFA
dabs_w: .word   docol                           ;  CFA

        .word   dup
        .word   dpm
        .word   semis

; ********************************************************************************
;
;                                       MIN
;
; ********************************************************************************


l2508:  .byte   $83
        .text   'MI'
        .byte   $ce

        .word   l2498                           ;  LFA
min:    .word   docol                           ;  CFA

        .word   over
        .word   over
        .word   great
        .word   zbran
        .word   4
        .word   swap
        .word   drop
        .word   semis

; ********************************************************************************
;
;                                       MAX
;
; ********************************************************************************


l2523:  .byte   $83
        .text   'MA'
        .byte   $d8

        .word   l2508                           ;  LFA
max:    .word   docol                           ;  CFA

        .word   over
        .word   over
        .word   less
        .word   zbran
        .word   4
        .word   swap
        .word   drop
        .word   semis

; ********************************************************************************
;
;                                        M*
;
; ********************************************************************************


l2538:  .byte   $82
        .text   'M'
        .byte   $aa

        .word   l2523                           ;  LFA
mstar:  .word   docol                           ;  CFA

        .word   over
        .word   over
        .word   xor
        .word   tor
        .word   abs_w
        .word   swap
        .word   abs_w
        .word   ustar
        .word   rfrom
        .word   dpm
        .word   semis

; ********************************************************************************
;
;                                        M/
;
; ********************************************************************************


l2556:  .byte   $82
        .text   'M'
        .byte   $af

        .word   l2538                           ;  LFA
mslash: .word   docol                           ;  CFA

        .word   over
        .word   tor
        .word   tor
        .word   dabs_w
        .word   r
        .word   abs_w
        .word   uslash
        .word   rfrom
        .word   r
        .word   xor
        .word   pm
        .word   swap
        .word   rfrom
        .word   pm
        .word   swap
        .word   semis

; ********************************************************************************
;
;                                        *
;
; ********************************************************************************


l2579:  .byte   $81,$aa

        .word   l2556                           ;  LFA
star:   .word   docol                           ;  CFA

        .word   ustar
        .word   drop
        .word   semis

; ********************************************************************************
;
;                                       /MOD
;
; ********************************************************************************


l2589:  .byte   $84
        .text   '/MO'
        .byte   $c4

        .word   l2579                           ;  LFA
slmod:  .word   docol                           ;  CFA

        .word   tor
        .word   stod
        .word   rfrom
        .word   mslash
        .word   semis

; ********************************************************************************
;
;                                        /
;
; ********************************************************************************


l2601:  .byte   $81,$af

        .word   l2589                           ;  LFA
slash:  .word   docol                           ;  CFA

        .word   slmod
        .word   swap
        .word   drop
        .word   semis

; ********************************************************************************
;
;                                       MOD
;
; ********************************************************************************


l2612:  .byte   $83
        .text   'MO'
        .byte   $c4

        .word   l2601                           ;  LFA
mod:    .word   docol                           ;  CFA

        .word   slmod
        .word   drop
        .word   semis

; ********************************************************************************
;
;                                      */MOD
;
; ********************************************************************************


l2622:  .byte   $85
        .text   '*/MO'
        .byte   $c4

        .word   l2612                           ;  LFA
ssmod:  .word   docol                           ;  CFA

        .word   tor
        .word   mstar
        .word   rfrom
        .word   mslash
        .word   semis

; ********************************************************************************
;
;                                        */
;
; ********************************************************************************


l2634:  .byte   $82
        .text   '*'
        .byte   $af

        .word   l2622                           ;  LFA
sslash: .word   docol                           ;  CFA

        .word   ssmod
        .word   swap
        .word   drop
        .word   semis

; ********************************************************************************
;
;                                      M/MOD
;
; ********************************************************************************


l2645:  .byte   $85
        .text   'M/MO'
        .byte   $c4

        .word   l2634                           ;  LFA
msmod:  .word   docol                           ;  CFA

        .word   tor
        .word   zero
        .word   r
        .word   uslash
        .word   rfrom
        .word   swap
        .word   tor
        .word   uslash
        .word   rfrom
        .word   semis

; ********************************************************************************
;
;                                       USE
;
; ********************************************************************************


l2662:  .byte   $83
        .text   'US'
        .byte   $c5

        .word   l2645                           ;  LFA
use:    .word   dovar                           ;  CFA

        .word   darea

; ********************************************************************************
;
;                                       PREV
;
; ********************************************************************************


l2670:  .byte   $84
        .text   'PRE'
        .byte   $d6

        .word   l2662                           ;  LFA
prev:   .word   dovar

        .word   darea

; ********************************************************************************
;
;                                       +BUF
;
; ********************************************************************************


l2678:  .byte   $84
        .text   '+BU'
        .byte   $c6

        .word   l2670                           ;  LFA
pbuf:   .word   docol                           ;  CFA

        .word   lit
        .word   ssize+4
        .word   plus
        .word   dup
        .word   limit
        .word   equals
        .word   zbran
        .word   6
        .word   drop
        .word   first
        .word   dup
        .word   prev
        .word   at
        .word   sub
        .word   semis

; ********************************************************************************
;
;                                      UPDATE
;
; ********************************************************************************


l2700:  .byte   $86
        .text   'UPDAT'
        .byte   $c5

        .word   l2678                           ;  LFA
update: .word   docol                           ;  CFA

        .word   prev
        .word   at
        .word   at
        .word   lit
        .word   $8000
        .word   or
        .word   prev
        .word   at
        .word   store
        .word   semis

; ********************************************************************************
;
;                                      FLUSH
;
; ********************************************************************************


l2705:  .byte   $85
        .text   'FLUS'
        .byte   $c8

        .word   l2700                           ;  LFA
        .word   docol                           ;  CFA

        .word   limit
        .word   first
        .word   sub
        .word   bbuf
        .word   clit
        .byte   4
        .word   plus
        .word   slash
        .word   onep
        .word   zero
        .word   pdo
        .word   lit
        .word   $7fff
        .word   buffer
        .word   drop
        .word   ploop
        .word   (-10 & $ffff)
        .word   semis

; ********************************************************************************
;
;                                  EMPTY-BUFFERS
;
; ********************************************************************************


l2716:  .byte   $8d
        .text   'EMPTY-BUFFER'
        .byte   $d3

        .word   l2705                           ;  LFA
        .word   docol                           ;  CFA

        .word   first
        .word   limit
        .word   over
        .word   sub
        .word   erase
        .word   semis

; ********************************************************************************
;
;                                      BUFFER
;
; ********************************************************************************


l2751:  .byte   $86
        .text   'BUFFE'
        .byte   $d2

        .word   l2716                           ;  LFA
buffer: .word   docol                           ;  CFA

        .word   use
        .word   at
        .word   dup
        .word   tor
        .word   pbuf
        .word   zbran
        .word   (-4 & $ffff)
        .word   use
        .word   store
        .word   r
        .word   at
        .word   zless
        .word   zbran
        .word   20
        .word   r
        .word   twop
        .word   r
        .word   at
        .word   lit
        .word   $7fff
        .word   andd
        .word   zero
        .word   r
        .word   store
        .word   r
        .word   prev
        .word   store
        .word   rfrom
        .word   twop
        .word   semis

; ********************************************************************************
;
;                                      BLOCK
;
; ********************************************************************************


l2788:  .byte   $85
        .text   'BLOC'
        .byte   $cb

        .word   l2751                           ;  LFA
block:  .word   docol                           ;  CFA

        .word   offset
        .word   at
        .word   plus
        .word   tor
        .word   prev
        .word   at
        .word   dup
        .word   at
        .word   r
        .word   sub
        .word   dup
        .word   plus
        .word   zbran
        .word   52
        .word   pbuf
        .word   zequ
        .word   zbran
        .word   20
        .word   drop
        .word   r
        .word   buffer
        .word   dup
        .word   r
        .word   one
        .word   two
        .word   sub
        .word   dup
        .word   at
        .word   r
        .word   sub
        .word   dup
        .word   plus
        .word   zequ
        .word   zbran
        .word   $ffd6
        .word   dup
        .word   prev
        .word   store
        .word   rfrom
        .word   drop
        .word   twop
        .word   semis

; ********************************************************************************
;
;                                      (LINE)
;
; ********************************************************************************


l2838:  .byte   $86
        .text   '(LINE'
        .byte   $a9

        .word   l2788                           ;  LFA
pline:  .word   docol

        .word   tor
        .word   csll
        .word   bbuf
        .word   ssmod
        .word   rfrom
        .word   bscr
        .word   star
        .word   plus
        .word   block
        .word   plus
        .word   csll
        .word   semis

; ********************************************************************************
;
;                                      .LINE
;
; ********************************************************************************


l2857:  .byte   $85
        .text   '.LIN'
        .byte   $c5

        .word   l2838                           ;  LFA
dline:  .word   docol                           ;  CFA

        .word   pline
        .word   dtrai
        .word   type_w
        .word   semis

; ********************************************************************************
;
;                                     MESSAGE
;
; ********************************************************************************


l2868:  .byte   $87
        .text   'MESSAG'
        .byte   $c5

        .word   l2857                           ;  LFA
mess:   .word   docol                           ;  CFA

        .word   warn
        .word   at
        .word   zbran
        .word   27
        .word   ddup
        .word   zbran
        .word   17
        .word   clit
        .byte   4
        .word   offset
        .word   at
        .word   bscr
        .word   slash
        .word   sub
        .word   dline
        .word   branch
        .word   13
        .word   pdotq
        .byte   6
        .text   'MSG # '
        .word   dot
        .word   semis

; ********************************************************************************
;
;                                       LOAD
;
; ********************************************************************************


l2896:  .byte   $84
        .text   'LOA'
        .byte   $c4

        .word   l2868                           ;  LFA
load:   .word   docol                           ;  CFA

        .word   blk
        .word   at
        .word   tor
        .word   in
        .word   at
        .word   tor
        .word   zero
        .word   in
        .word   store
        .word   bscr
        .word   star
        .word   blk
        .word   store
        .word   inter
        .word   rfrom
        .word   in
        .word   store
        .word   rfrom
        .word   blk
        .word   store
        .word   semis

; ********************************************************************************
;
;                                       -->
;
; ********************************************************************************


l2924:  .byte   $c3
        .text   '--'
        .byte   $be

        .word   l2896                           ;  LFA
        .word   docol                           ;  CFA

        .word   qload
        .word   zero
        .word   in
        .word   store
        .word   bscr
        .word   blk
        .word   at
        .word   over
        .word   mod
        .word   sub
        .word   blk
        .word   pstore
        .word   semis

xemit:  tya                                     ;  writes 1
        sec                                     ;  ASCII
        ldy     #$1a                            ;  char to
        adc     (up),y                          ;  terminal
        sta     (up),y
        iny                                     ;  bump OUT
        lda     #0
        adc     (up),y
        sta     (up),y
        lda     0,x                             ;  fetch char
        and     #$7f
        stx     xsave
        jsr     oswrch                          ;  display it
        ldx     xsave
        jmp     pop

; ********************************************************************************
;
;                                       >VDU
;
; ********************************************************************************


l3000:  .byte   $84
        .text   '>VD'
        .byte   $d5

        .word   l2924
        .word   *+2

        lda     0,x
        jsr     oswrch
        jmp     pop


xkey:   stx     xsave                           ;  reads one keystroke
        jsr     osrdch
        bit     $ff                             ;  MJR
        bpl     noesc                           ;  MJR
        lda     $7e                             ;  MJR
        jsr     osbyte                          ;  MJR
        lda     $ff                             ;  MJR
        and     #127                            ;  MJR
        sta     $ff                             ;  MJR
        jmp     reentr                          ;  MJR

noesc:  ldx     xsave
        jmp     push0a


xqter:  lda     #0
        jmp     push0a                          ;  dummied


xcr:    stx     xsave                           ;  CRLF to terminal
        jsr     osnewl                          ;  monitor call
        ldx     xsave
        jmp     next

; ********************************************************************************
;
;                                       -BCD
;
; ********************************************************************************


l3050:  .byte   $84
        .text   '-BC'
        .byte   $c4

        .word   l3000                           ;  LFA
dbcd:   .word   docol                           ;  CFA

        .word   zero
        .word   clit
        .byte   10
        .word   uslash
        .word   clit
        .byte   16
        .word   star
        .word   or
        .word   semis


; ********************************************************************************
;
;                                     ' (TICK)
;
; ********************************************************************************


l3202:  .byte   $c1,$a7

        .word   l3050                           ;  LFA
tick:   .word   docol                           ;  CFA

        .word   dfind
        .word   zequ
        .word   zero
        .word   qerror
        .word   drop
        .word   liter
        .word   semis

; ********************************************************************************
;
;                                      FORGET
;
; ********************************************************************************


l3217:  .byte   $86
        .text   'FORGE'
        .byte   $d4

        .word   l3202                           ;  LFA
forget: .word   docol                           ;  CFA

        .word   tick
        .word   nfa
        .word   dup
        .word   fence
        .word   at
        .word   uless
        .word   clit
        .byte   $15
        .word   qerror
        .word   tor
        .word   voclnk
        .word   at
        .word   r
        .word   over
        .word   uless
        .word   zbran
        .word   l3225-*
        .word   forth
        .word   defin
        .word   at
        .word   dup
        .word   voclnk
        .word   store
        .word   branch
        .word   (-24 & $ffff)
l3225:  .word   dup
        .word   clit
        .byte   4
        .word   sub
        .word   pfa
        .word   lfa
        .word   at
        .word   dup
        .word   r
        .word   uless
        .word   zbran
        .word   (-14 & $ffff)
        .word   over
        .word   two
        .word   sub
        .word   store
        .word   at
        .word   ddup
        .word   zequ
        .word   zbran
        .word   (-39 & $ffff)
        .word   rfrom
        .word   dp
        .word   store
        .word   semis

; ********************************************************************************
;
;                                       BACK
;
; ********************************************************************************


l3250:  .byte   $84
        .text   'BAC'
        .byte   $cb

        .word   l3217                           ;  LFA
back:   .word   docol                           ;  CFA

        .word   here
        .word   sub
        .word   comma
        .word   semis

; ********************************************************************************
;
;                                      BEGIN
;
; ********************************************************************************


l3261:  .byte   $c5
        .text   'BEGI'
        .byte   $ce

        .word   l3250                           ;  LFA
        .word   docol                           ;  CFA

        .word   qcomp
        .word   here
        .word   one
        .word   semis

; ********************************************************************************
;
;                                      ENDIF
;
; ********************************************************************************


l3273:  .byte   $c5
        .text   'ENDI'
        .byte   $c6

        .word   l3261                           ;  LFA
endif:  .word   docol                           ;  CFA

        .word   qcomp
        .word   two
        .word   qpair
        .word   here
        .word   over
        .word   sub
        .word   swap
        .word   store
        .word   semis

; ********************************************************************************
;
;                             THEN *      ; (= ENDIF)
;
; ********************************************************************************


l3290:  .byte   $c4
        .text   'THE'
        .byte   $ce

        .word   l3273                           ;  LFA
        .word   docol                           ;  CFA

        .word   endif
        .word   semis

; ********************************************************************************
;
;                                        DO
;
; ********************************************************************************


l3300:  .byte   $c2
        .text   'D'
        .byte   $cf

        .word   l3290                           ;  LFA
        .word   docol                           ;  CFA

        .word   comp
        .word   pdo
        .word   here
        .word   three
        .word   semis

; ********************************************************************************
;
;                                       LOOP
;
; ********************************************************************************


l3313:  .byte   $c4
        .text   'LOO'
        .byte   $d0

        .word   l3300                           ;  LFA
        .word   docol                           ;  CFA

        .word   three
        .word   qpair
        .word   comp
        .word   ploop
        .word   back
        .word   semis

; ********************************************************************************
;
;                                      +LOOP
;
; ********************************************************************************


l3327:  .byte   $c5
        .text   '+LOO'
        .byte   $d0

        .word   l3313                           ;  LFA
        .word   docol                           ;  CFA

        .word   three
        .word   qpair
        .word   comp
        .word   pploo
        .word   back
        .word   semis

; ********************************************************************************
;
;                                      UNTIL
;
; ********************************************************************************


l3341:  .byte   $c5
        .text   'UNTI'
        .byte   $cc

        .word   l3327                           ;  LFA
until:  .word   docol                           ;  CFA

        .word   one
        .word   qpair
        .word   comp
        .word   zbran
        .word   back
        .word   semis

; ********************************************************************************
;
;                              END *      ; (=UNTIL)
;
; ********************************************************************************


l3355:  .byte   $c3
        .text   'EN'
        .byte   $c4

        .word   l3341                           ;  LFA
        .word   docol                           ;  CFA

        .word   until
        .word   semis

; ********************************************************************************
;
;                                      AGAIN
;
; ********************************************************************************


l3365:  .byte   $c5
        .text   'AGAI'
        .byte   $ce

        .word   l3355                           ;  LFA
again:  .word   docol                           ;  CFA

        .word   one
        .word   qpair
        .word   comp
        .word   branch
        .word   back
        .word   semis

; ********************************************************************************
;
;                                      REPEAT
;
; ********************************************************************************


l3379:  .byte   $c6
        .text   'REPEA'
        .byte   $d4

        .word   l3365                           ;  LFA
        .word   docol                           ;  CFA

        .word   tor
        .word   tor
        .word   again
        .word   rfrom
        .word   rfrom
        .word   two
        .word   sub
        .word   endif
        .word   semis

; ********************************************************************************
;
;                                        IF
;
; ********************************************************************************


l3396:  .byte   $c2
        .text   'I'
        .byte   $c6

        .word   l3379                           ;  LFA
if:     .word   docol                           ;  CFA

        .word   comp
        .word   zbran
        .word   here
        .word   zero
        .word   comma
        .word   two
        .word   semis

; ********************************************************************************
;
;                                       ELSE
;
; ********************************************************************************


l3411:  .byte   $c4
        .text   'ELS'
        .byte   $c5

        .word   l3396                           ;  LFA
        .word   docol                           ;  CFA

        .word   two
        .word   qpair
        .word   comp
        .word   branch
        .word   here
        .word   zero
        .word   comma
        .word   swap
        .word   two
        .word   endif
        .word   two
        .word   semis

; ********************************************************************************
;
;                                      WHILE
;
; ********************************************************************************


l3431:  .byte   $c5
        .text   'WHIL'
        .byte   $c5

        .word   l3411                           ;  LFA
        .word   docol                           ;  CFA
        .word   if
        .word   twop
        .word   semis

; ********************************************************************************
;
;                                      SPACES
;
; ********************************************************************************


l3442:  .byte   $86
        .text   'SPACE'
        .byte   $d3

        .word   l3431                           ;  LFA
spaces: .word   docol                           ;  CFA

        .word   zero
        .word   max
        .word   ddup
        .word   zbran
        .word   12
        .word   zero
        .word   pdo
        .word   space
        .word   ploop
        .word   (-4 & $ffff)
        .word   semis

; ********************************************************************************
;
;                                        <#
;
; ********************************************************************************


l3460:  .byte   $82
        .text   '<'
        .byte   $a3

        .word   l3442                           ;  LFA
bdigs:  .word   docol                           ;  CFA

        .word   pad
        .word   hld
        .word   store
        .word   semis

; ********************************************************************************
;
;                                        #>
;
; ********************************************************************************


l3471:  .byte   $82
        .text   '#'
        .byte   $be

        .word   l3460                           ;  LFA
edigs:  .word   docol                           ;  CFA

        .word   drop
        .word   drop
        .word   hld
        .word   at
        .word   pad
        .word   over
        .word   sub
        .word   semis

; ********************************************************************************
;
;                                       SIGN
;
; ********************************************************************************


l3486:  .byte   $84
        .text   'SIG'
        .byte   $ce

        .word   l3471                           ;  LFA
sign_w: .word   docol                           ;  CFA

        .word   rot
        .word   zless
        .word   zbran
        .word   7
        .word   clit
        .byte   45
        .word   hold
        .word   semis

; ********************************************************************************
;
;                                        #
;
; ********************************************************************************


l3501:  .byte   $81,$a3

        .word   l3486                           ;  LFA
dig:    .word   docol                           ;  CFA

        .word   base
        .word   at
        .word   msmod
        .word   rot
        .word   clit
        .byte   9
        .word   over
        .word   less
        .word   zbran
        .word   7
        .word   clit
        .byte   7
        .word   plus
        .word   clit
        .byte   48
        .word   plus
        .word   hold
        .word   semis

; ********************************************************************************
;
;                                        #S
;
; ********************************************************************************


l3526:  .byte   $82
        .text   '#'
        .byte   $d3

        .word   l3501                           ;  LFA
digs:   .word   docol                           ;  CFA

        .word   dig
        .word   over
        .word   over
        .word   or
        .word   zequ
        .word   zbran
        .word   (-12 & $ffff)
        .word   semis
        .word   semis

; ********************************************************************************
;
;                                       D.R
;
; ********************************************************************************


l3541:  .byte   $83
        .text   'D.'
        .byte   $d2

        .word   l3526                           ;  LFA
ddotr:  .word   docol                           ;  CFA

        .word   tor
        .word   swap
        .word   over
        .word   dabs_w
        .word   bdigs
        .word   digs
        .word   sign_w
        .word   edigs
        .word   rfrom
        .word   over
        .word   sub
        .word   spaces
        .word   type_w
        .word   semis

; ********************************************************************************
;
;                                        D.
;
; ********************************************************************************


l3562:  .byte   $82
        .text   'D'
        .byte   $ae

        .word   l3541                           ;  LFA
ddot:   .word   docol                           ;  CFA

        .word   zero
        .word   ddotr
        .word   space
        .word   semis

; ********************************************************************************
;
;                                        .R
;
; ********************************************************************************


l3567:  .byte   $82
        .text   '.'
        .byte   $d2

        .word   l3562                           ;  LFA
dotr:   .word   docol                           ;  CFA

        .word   tor
        .word   stod
        .word   rfrom
        .word   ddotr
        .word   semis

; ********************************************************************************
;
;                                        .
;
; ********************************************************************************


l3585:  .byte   $81,$ae

        .word   l3567                           ;  LFA
dot:    .word   docol                           ;  CFA

        .word   stod
        .word   ddot
        .word   semis

; ********************************************************************************
;
;                                        ?
;
; ********************************************************************************


l3595:  .byte   $81,$bf

        .word   l3585                           ;  LFA
ques:   .word   docol                           ;  CFA

        .word   at
        .word   dot
        .word   semis

; ********************************************************************************
;
;                                      LIST_W
;
; ********************************************************************************


l3605:  .byte   $84
        .text   'LIS'
        .byte   $d4

        .word   l3595                           ;  LFA
list_w: .word   docol                           ;  CFA

        .word   decim
        .word   cr
        .word   dup
        .word   scr
        .word   store
        .word   pdotq
        .byte   6
        .text   'SCR # '
        .word   dot
        .word   clit
        .byte   16
        .word   zero
        .word   pdo
        .word   cr
        .word   i
        .word   three
        .word   dotr
        .word   space
        .word   i
        .word   scr
        .word   at
        .word   dline
        .word   ploop
        .word   (-20 & $ffff)
        .word   cr
        .word   semis

; ********************************************************************************
;
;                                      INDEX
;
; ********************************************************************************


l3637:  .byte   $85
        .text   'INDE'
        .byte   $d8

        .word   l3605                           ;  LFA
        .word   docol                           ;  CFA

        .word   cr
        .word   onep
        .word   swap
        .word   pdo
        .word   cr
        .word   i
        .word   three
        .word   dotr
        .word   space
        .word   zero
        .word   i
        .word   dline
        .word   qterm
        .word   zbran
        .word   4
        .word   leave
        .word   ploop
        .word   (-26 & $ffff)
        .word   clit
        .byte   12                              ;  FF for printer
        .word   emit
        .word   semis

; ********************************************************************************
;
;                                      TRIAD
;
; ********************************************************************************


l3666:  .byte   $85
        .text   'TRIA'
        .byte   $c4

        .word   l3637                           ;  LFA
        .word   docol                           ;  CFA

        .word   three
        .word   slash
        .word   three
        .word   star
        .word   three
        .word   over
        .word   plus
        .word   swap
        .word   pdo
        .word   cr
        .word   i
        .word   list_w
        .word   ploop
        .word   (-8 & $ffff)
        .word   cr
        .word   clit
        .byte   15
        .word   mess
        .word   cr
        .word   clit
        .byte   12                              ;  FF for printer
        .word   emit
        .word   semis

; ********************************************************************************
;
;                                      VLIST
;
; ********************************************************************************


l3696:  .byte   $85
        .text   'VLIS'
        .byte   $d4

        .word   l3666                           ;  LFA
vlist_w:.word   docol                           ;  CFA

        .word   clit
        .byte   $80
        .word   out
        .word   store
        .word   con
        .word   at
        .word   at
        .word   out
        .word   at
        .word   csll
        .word   great
        .word   zbran
        .word   10
        .word   cr
        .word   zero
        .word   out
        .word   store
        .word   dup
        .word   iddot
        .word   space
        .word   space
        .word   pfa
        .word   lfa
        .word   at
        .word   dup
        .word   zequ
        .word   qterm
        .word   or
        .word   zbran
        .word   $ffd4
        .word   drop
        .word   semis

; ********************************************************************************
;
;                                       MON
;
; ********************************************************************************


l4000:  .byte   $83
        .text   'MO'
        .byte   $ce

        .word   l3696                           ;  LFA
mon:    .word   *+2                             ;  CFA

        stx     xsave
        brk                                     ;  break out
        ldx     xsave                           ;  to monitor
        jmp     next                            ;  and reenter


ntop:   .byte   $84
        .text   'NOO'
        .byte   $d0

        .word   l4000                           ;  LFA
noop:   .word   docol                           ;  CFA

        .word   semis                           ;  NULL DEF'N

top:                                            ;  of dictionary
