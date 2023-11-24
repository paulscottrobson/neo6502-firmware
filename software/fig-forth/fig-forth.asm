;
;                        Through the courtesy of
;
;                         FORTH INTEREST GROUP
;                            P.O. BOX  2154
;                         OAKLAND, CALIFORNIA
;                                94621
;
;
;                             Release 1.1
;
;                        with compiler security
;                                  and
;                        variable length names
;
;    Further distribution must include the above notice.
;    The FIG installation Manual is required as it contains
;    the model of FORTH and glossary of the system.
;    Available from FIG at the above address for **.** postpaid.
;
;    Translated from the FIG model by W.F. Ragsdale with input-
;    output given for the Rockwell System-65. Transportation to
;    other systems requires only the alteration of :
;                 XEMIT, XKEY, XQTER, XCR, AND RSLW
;
;    Equates giving memory assignments, machine
;    registers, and disk parameters.
;
SSIZE     =128           ; sector size in bytes
NBUF      =8             ; number of buffers desired in RAM
;                             (SSIZE*NBUF >= 1024 bytes)
SECTR     =800           ; sector per drive
;                              forcing high drive to zero
SECTL     =1600          ; sector limit for two drives
;                              of 800 per drive.
BMAG      =1056          ; total buffer magnitude, in bytes
;                              expressed by SSIZE+4*NBUF
;
BOS       =$20           ; bottom of data stack, in zero-page.
TOS       =$9E           ; top of data stack, in zero-page.
N         =TOS+8         ; scratch workspace.
IP        =N+8           ; interpretive pointer.
W         =IP+3          ; code field pointer.
UP        =W+2           ; user area pointer.
XSAVE     =UP+2          ; temporary for X register.
;
TIBX      =$0100         ; terminal input buffer of 84 bytes.
ORIG      =$0400         ; origin of FORTH's Dictionary.
MEM       =$4000         ; top of assigned memory+1 byte.
UAREA     =MEM-128       ; 128 bytes of user area
DAREA     =UAREA-BMAG    ; disk buffer space.
;
;         Monitor calls for terminal support
;
; OUTCH     =$D2C1         ; output one ASCII char. to term.
;
; INCH      =$D1DC         ; input one ASCII char. to term.
; TCR       =$D0F1         ; terminal return and line feed.

.include "neo6502.inc"

OUTCH = WriteCharacter
INCH = ReadCharacter

;
;    From DAREA downward to the top of the dictionary is free
;    space where the user's applications are compiled.
;
;    Boot up parameters. This area provides jump vectors
;    to Boot up  code, and parameters describing the system.
;
;
          * = ORIG
;
                         ; User cold entry point
ENTER     NOP            ; Vector to COLD entry
          JMP COLD+2     ;
REENTR    NOP            ; User Warm entry point
          JMP WARM       ; Vector to WARM entry
          .word $0004    ; 6502 in radix-36
          .word $5ED2    ;
          .word NTOP     ; Name address of MON
          .word $08      ; Backspace Character
          .word UAREA    ; Initial User Area
          .word TOS      ; Initial Top of Stack
          .word $1FF     ; Initial Top of Return Stack
          .word TIBX     ; Initial terminal input buffer
;
;
          .word 31       ; Initial name field width
          .word 0        ; 0=nod disk, 1=disk
          .word TOP      ; Initial fence address
          .word TOP      ; Initial top of dictionary
          .word VL0      ; Initial Vocabulary link ptr.
;
;    The following offset adjusts all code fields to avoid an
;    address ending $XXFF. This must be checked and altered on
;    any alteration , for the indirect jump at W-1 to operate !
;
;    Fixed on the 65C02
;
;          .fill     2

TCR:    pha
        lda     #13
        jsr     WriteCharacter
        pla
        rts
;
;
;                                       LIT
;                                       SCREEN 13 LINE 1
;
L22       .TEXT $83,'LI',$D4            ; <--- name field
;                          <----- link field
          .word 0        ; last link marked by zero
LIT       .word *+2      ; <----- code address field
          LDA (IP),Y     ; <----- start of parameter field
          PHA
          INC IP
          BNE L30
          INC IP+1
L30       LDA (IP),Y
L31       INC IP
          BNE PUSH
          INC IP+1
;
PUSH      DEX
          DEX
;
PUT       STA 1,X
          PLA
          STA 0,X
;
;    NEXT is the address interpreter that moves from machine
;    level word to word.
;
NEXT      LDY #1
          LDA (IP),Y     ; Fetch code field address pointed
          STA W+1        ; to by IP.
          DEY
          LDA (IP),Y
          STA W
;          JSR TRACE      ; Remove this when all is well
          CLC            ; Increment IP by two.
          LDA IP
          ADC #2
          STA IP
          BCC L54
          INC IP+1
L54       JMP W-1        ; Jump to an indirect jump (W) which
;                          vectors to code pointed to by a code
;                          field.
;
;    CLIT pushes the next inline byte to data stack
;
L35       .TEXT $84,'CLI',$D4
          .word L22      ; Link to LIT
CLIT      .word *+2
          LDA (IP),Y
          PHA
          TYA
          BEQ L31        ; a forced branch into LIT
;
;
;    This is a temporary trace routine, to be used until FORTH
;    is generally operating. Then NOP the terminal query
;    "JSR ONEKEY". This will allow user input to the text
;    interpreter. When crashes occur, the display shows IP, W,
;    and the word locations of the offending code. When all is
;    well, remove : TRACE, TCOLON, PRNAM, DECNP, and the
;    following monitor/register equates.
;
;
;
;    Monitor routines needed to trace.
;
XBLANK    =$D0AF         ; print one blank
CRLF      =$D0D2         ; print a carriage return and line feed.
HEX2      =$D2CE         ; print accum as two hex numbers
LETTER    =$D2C1         ; print accum as one ASCII character
ONEKEY    =$D1DC         ; wait for keystroke
XW        =$12           ; scratch reg. to next code field add
NP        =$14           ; scratch reg. pointing to name field
;
;
_TRACE     STX XSAVE
          JSR CRLF
          LDA IP+1
          JSR HEX2
          LDA IP
          JSR HEX2       ; print IP, the interpreter pointer
          JSR XBLANK
;
;
          LDA #0
          LDA (IP),Y
          STA XW
          STA NP         ; fetch the next code field pointer
          INY
          LDA (IP),Y
          STA XW+1
          STA NP+1
          JSR XPRNAM      ; print dictionary name
;
          LDA XW+1
          JSR HEX2       ; print code field address
          LDA XW
          JSR HEX2
          JSR XBLANK
;
          LDA XSAVE      ; print stack location in zero-page
          JSR HEX2
          JSR XBLANK
;
          LDA #1         ; print return stack bottom in page 1
          JSR HEX2
          TSX
          INX
          TXA
          JSR HEX2
          JSR XBLANK
;
          JSR ONEKEY     ; wait for operator keystroke
          LDX XSAVE      ; just to pinpoint early problems
          LDY #0
          RTS
;
;    TCOLON is called from DOCOLON to label each point
;    where FORTH 'nests' one level.
;
_TCOLON    STX XSAVE
          LDA W
          STA NP         ; locate the name of the called word
          LDA W+1
          STA NP+1
          JSR CRLF
          LDA #$3A       ; ':
          JSR LETTER
          JSR XBLANK
          JSR XPRNAM
          LDX XSAVE
          RTS
;
;    Print name by it's code field address in NP
;
XPRNAM    JSR XDECNP
          JSR XDECNP
          JSR XDECNP
          LDY #0
PN1       JSR XDECNP
          LDA (NP),Y     ; loop till D7 in name set
          BPL PN1
PN2       INY
          LDA (NP),Y
          JSR LETTER     ; print letters of name field
          LDA (NP),Y
          BPL PN2
          JSR XBLANK
          LDY #0
          RTS
;
;    Decrement name field pointer
;
XDECNP     LDA NP
          BNE DECNP1
          DEC NP+1
DECNP1    DEC NP
          RTS
;
;
SETUP     ASL A
          STA N-1
L63       LDA 0,X
          STA N,Y
          INX
          INY
          CPY N-1
          BNE L63
          LDY #0
          RTS
;
;                                       EXCECUTE
;                                       SCREEN 14 LINE 11
;
L75       .TEXT $87,'EXECUT',$C5
          .word L35      ; link to CLIT
EXEC      .word *+2
          LDA 0,X
          STA W
          LDA 1,X
          STA W+1
          INX
          INX
          JMP W-1        ; to JMP (W) in z-page
;
;                                       BRANCH
;                                       SCREEN 15 LINE 11
;
L89       .TEXT $86,'BRANC',$C8
          .word L75      ; link to EXCECUTE
BRAN      .word *+2
          CLC
          LDA (IP),Y
          ADC IP
          PHA
          INY
          LDA (IP),Y
          ADC IP+1
          STA IP+1
          PLA
          STA IP
          JMP NEXT +2
;
;                                       0BRANCH
;                                       SCREEN 15 LINE 6
;
L107      .TEXT $87,'0BRANC',$C8
          .word L89      ; link to BRANCH
ZBRAN     .word *+2
          INX
          INX
          LDA $FE,X
          ORA $FF,X
          BEQ BRAN+2
;
BUMP      CLC
          LDA IP
          ADC #2
          STA IP
          BCC L122
          INC IP+1
L122      JMP NEXT
;
;                                       (LOOP)
;                                       SCREEN 16 LINE 1
;
L127      .TEXT $86,'(LOOP',$A9
          .word L107     ; link to 0BRANCH
PLOOP     .word L130
L130      STX  XSAVE
          TSX
          INC $101,X
          BNE PL1
          INC $102,X
;
PL1       CLC
          LDA $103,X
          SBC $101,X
          LDA $104,X
          SBC $102,X
;
PL2       LDX XSAVE
          ASL A
          BCC BRAN+2
          PLA
          PLA
          PLA
          PLA
          JMP BUMP
;
;                                       (+LOOP)
;                                       SCREEN 16 LINE 8
;
L154      .TEXT $87,'(+LOOP',$A9
          .word L127     ; link to (loop)
PPLOO     .word *+2
          INX
          INX
          STX XSAVE
          LDA $FF,X
          PHA
          PHA
          LDA $FE,X
          TSX
          INX
          INX
          CLC
          ADC $101,X
          STA $101,X
          PLA
          ADC $102,X
          STA $102,X
          PLA
          BPL PL1
          CLC
          LDA $101,X
          SBC $103,X
          LDA $102,X
          SBC $104,X
          JMP PL2
;
;                                       (DO)
;                                       SCREEN 17 LINE 2
;
L185      .TEXT $84,'(DO',$A9
          .word L154     ; link to (+LOOP)
PDO       .word *+2
          LDA 3,X
          PHA
          LDA 2,X
          PHA
          LDA 1,X
          PHA
          LDA 0,X
          PHA
;
POPTWO    INX
          INX
;
;
;
POP       INX
          INX
          JMP NEXT
;
;                                       I
;                                       SCREEN 17 LINE 9
;
L207      .TEXT $81,$C9
          .word L185     ; link to (DO)
I         .word R+2      ; share the code for R
;
;                                       DIGIT
;                                       SCREEN 18 LINE 1
;
L214      .TEXT $85,'DIGI',$D4
          .word L207     ; link to I
DIGIT     .word *+2
          SEC
          LDA 2,X
          SBC #$30
          BMI L234
          CMP #$A
          BMI L227
          SEC
          SBC #7
          CMP #$A
          BMI L234
L227      CMP 0,X
          BPL L234
          STA 2,X
          LDA #1
          PHA
          TYA
          JMP PUT        ; exit true with converted value
L234      TYA
          PHA
          INX
          INX
          JMP PUT        ; exit false with bad conversion
;
;                                       (FIND)
;                                       SCREEN 19 LINE 1
;
L243      .TEXT $86,'(FIND',$A9
          .word L214   ; Link to DIGIT
PFIND     .word *+2
          LDA #2
          JSR SETUP
          STX XSAVE
L249      LDY #0
          LDA (N),Y
          EOR (N+2),Y
;
;
          AND #$3F
          BNE L281
L254      INY
          LDA (N),Y
          EOR (N+2),Y
          ASL A
          BNE L280
          BCC L254
          LDX XSAVE
          DEX
          DEX
          DEX
          DEX
          CLC
          TYA
          ADC #5
          ADC N
          STA 2,X
          LDY #0
          TYA
          ADC N+1
          STA 3,X
          STY 1,X
          LDA (N),Y
          STA 0,X
          LDA #1
          PHA
          JMP PUSH
L280      BCS L284
L281      INY
          LDA (N),Y
          BPL L281
L284      INY
          LDA (N),Y
          TAX
          INY
          LDA (N),Y
          STA N+1
          STX N
          ORA N
          BNE L249
          LDX XSAVE
          LDA #0
          PHA
          JMP PUSH       ; exit false upon reading null link
;
;                                       ENCLOSE
;                                       SCREEN 20 LINE 1
;
L301      .TEXT $87,'ENCLOS',$C5
          .word L243     ; link to (FIND)
ENCL      .word *+2
          LDA #2
          JSR SETUP
          TXA
          SEC
          SBC #8
          TAX
          STY 3,X
          STY 1,X
          DEY
L313      INY
          LDA (N+2),Y
          CMP N
          BEQ L313
          STY 4,X
L318      LDA (N+2),Y
          BNE L327
          STY 2,X
          STY 0,X
          TYA
          CMP 4,X
          BNE L326
          INC 2,X
L326      JMP NEXT
L327      STY 2,X
          INY
          CMP N
          BNE L318
          STY 0,X
          JMP NEXT
;
;                                       EMIT
;                                       SCREEN 21 LINE 5
;
L337      .TEXT $84,'EMI',$D4
          .word L301     ; link to ENCLOSE
EMIT      .word XEMIT    ; Vector to code for KEY
;
;                                       KEY
;                                       SCREEN 21 LINE 7
;
L344      .TEXT $83,'KE',$D9
          .word L337     ; link to EMIT
KEY       .word XKEY     ; Vector to code for KEY
;
;                                       ?TERMINAL
;                                       SCREEN 21 LINE 9
;
L351      .TEXT $89,'?TERMINA',$CC
          .word L344     ; link to KEY
QTERM     .word XQTER    ; Vector to code for ?TERMINAL
;
;
;
;
;
;                                       CR
;                                       SCREEN 21 LINE 11
;
L358      .TEXT $82,'C',$D2
          .word L351     ; link to ?TERMINAL
CR        .word XCR      ; Vector to code for CR
;
;                                       CMOVE
;                                       SCREEN 22 LINE 1
;
L365      .TEXT $85,'CMOV',$C5
          .word L358     ; link to CR
CMOVE     .word *+2
          LDA #3
          JSR SETUP
L370      CPY N
          BNE L375
          DEC N+1
          BPL L375
          JMP NEXT
L375      LDA (N+4),Y
          STA (N+2),Y
          INY
          BNE L370
          INC N+5
          INC N+3
          JMP L370
;
;                                       U*
;                                       SCREEN 23 LINE 1
;
L386      .TEXT $82,'U',$AA
          .word L365     ; link to CMOVE
USTAR     .word *+2
          LDA 2,X
          STA N
          STY 2,X
          LDA 3,X
          STA N+1
          STY 3,X
          LDY #16        ; for 16 bits
L396      ASL 2,X
          ROL 3,X
          ROL 0,X
          ROL 1,X
          BCC L411
          CLC
          LDA N
          ADC 2,X
          STA 2,X
          LDA N+1
          ADC 3,X
          STA 3,X
          LDA #0
          ADC 0,X
          STA 0,X

L411      DEY
          BNE L396
          JMP NEXT
;
;                                       U/
;                                       SCREEN 24 LINE 1
;
L418      .TEXT $82,'U',$AF
          .word L386     ; link to U*
USLAS     .word *+2
          LDA 4,X
          LDY 2,X
          STY 4,X
          ASL A
          STA 2,X
          LDA 5,X
          LDY 3,X
          STY 5,X
          ROL A
          STA 3,X
          LDA #16
          STA N
L433      ROL 4,X
          ROL 5,X
          SEC
          LDA 4,X
          SBC 0,X
          TAY
          LDA 5,X
          SBC 1,X
          BCC L444
          STY 4,X
          STA 5,X
L444      ROL 2,X
          ROL 3,X
          DEC N
          BNE L433
          JMP POP
;
;                                       AND
;                                       SCREEN 25 LINE 2
;
L453      .TEXT $83,'AN',$C4
          .word L418     ; link to U/
ANDD      .word *+2
          LDA 0,X
          AND 2,X
          PHA
          LDA 1,X
          AND 3,X
;
BINARY    INX
          INX
          JMP PUT
;
;                                       OR
;                                       SCREEN 25 LINE 7
;
L469      .TEXT $82,'O',$D2
          .word L453     ; link to AND
OR        .word *+2
          LDA 0,X
          ORA 2,X
          PHA
          LDA 1,X
          ORA 3,X
          INX
          INX
          JMP PUT
;
;                                       XOR
;                                       SCREEN 25 LINE 11
;
L484      .TEXT $83,'XO',$D2
          .word L469     ; link to OR
XOR       .word *+2
          LDA 0,X
          EOR 2,X
          PHA
          LDA 1,X
          EOR 3,X
          INX
          INX
          JMP PUT
;
;                                       SP@
;                                       SCREEN 26 LINE 1
;
L499      .TEXT $83,'SP',$C0
          .word L484     ; link  to XOR
SPAT      .word *+2
          TXA
;
PUSHOA    PHA
          LDA #0
          JMP PUSH
;
;                                       SP!
;                                       SCREEN 26 LINE 5
;
;
L511      .TEXT $83,'SP',$A1
          .word L499     ; link to SP@
SPSTO     .word *+2
          LDY #6
          LDA (UP),Y     ; load data stack pointer (X reg) from
          TAX            ; silent user variable S0.
          JMP NEXT
;
;                                       RP!
;                                       SCREEN 26 LINE 8
;
L522      .TEXT $83,'RP',$A1
          .word L511     ; link to SP!
RPSTO     .word *+2
          STX XSAVE      ; load return stack pointer (machine
          LDY #8         ; stack pointer) from silent user
          LDA (UP),Y     ; VARIABLE R0
          TAX
          TXS
          LDX XSAVE
          JMP NEXT
;
;                                       ;S
;                                       SCREEN 26 LINE 12
;
L536      .TEXT $82,';',$D3
          .word L522     ; link to RP!
SEMIS     .word *+2
          PLA
          STA IP
          PLA
          STA IP+1
          JMP NEXT
;
;                                       LEAVE
;                                       SCREEN 27 LINE  1
;
L548      .TEXT $85,'LEAV',$C5
          .word L536     ; link to ;S
LEAVE     .word *+2
          STX XSAVE
          TSX
          LDA $101,X
          STA $103,X
          LDA $102,X
          STA $104,X
          LDX XSAVE
          JMP NEXT
;
;                                       >R
;                                       SCREEN 27 LINE 5
;
L563      .TEXT $82,'>',$D2
          .word L548     ; link to LEAVE
TOR       .word *+2
          LDA 1,X        ; move high byte
          PHA
          LDA 0,X        ; then low byte
          PHA            ; to return stack
          INX
          INX            ; popping off data stack
          JMP NEXT
;
;                                       R>
;                                       SCREEN 27 LINE 8
;
L577      .TEXT $82,'R',$BE
          .word L563     ; link to >R
RFROM     .word *+2
          DEX            ; make room on data stack
          DEX
          PLA            ; high byte
          STA 0,X
          PLA            ; then low byte
          STA 1,X        ; restored to data stack
          JMP NEXT
;
;                                       R
;                                       SCREEN 27 LINE 11
;
L591      .TEXT $81,$D2
          .word L577     ; link to R>
R         .word *+2
          STX XSAVE
          TSX            ; address return stack
          LDA $101,X     ; copy bottom value
          PHA            ; to data stack
          LDA $102,X
          LDX XSAVE
          JMP PUSH
;
;                                       0=
;                                       SCREEN 28 LINE 2
;
L605      .TEXT $82,'0',$BD
          .word L591     ; link to R
ZEQU      .word *+2
          LDA 1,X        ; Corrected from FD3/2 p69
          STY 1,X
          ORA 0,X
          BNE L613
          INY
L613      STY 0,X
          JMP NEXT
;
;                                       0<
;                                       SCREEN 28 LINE 6
;
L619      .TEXT $82,'0',$BC
          .word L605     ; link to 0=
ZLESS     .word *+2
          ASL 1,X
          TYA
          ROL A
          STY 1,X
          STA 0,X
          JMP NEXT
;
;                                       +
;                                       SCREEN 29 LINE 1
;
L632      .TEXT $81,$AB
          .word L619     ; link to V-ADJ
PLUS      .word *+2
          CLC
          LDA 0,X
          ADC 2,X
          STA 2,X
          LDA 1,X
          ADC 3,X
          STA 3,X
          INX
          INX
          JMP NEXT
;
;                                       D+
;                                       SCREEN 29 LINE 4
;
L649      .TEXT $82,'D',$AB
          .word L632     ;    LINK TO +
DPLUS     .word *+2
          CLC
          LDA 2,X
          ADC 6,X
          STA 6,X
          LDA 3,X
          ADC 7,X
          STA 7,X
          LDA 0,X
          ADC 4,X
          STA 4,X
          LDA 1,X
          ADC 5,X
          STA 5,X
          JMP POPTWO
;
;                                       MINUS
;                                       SCREEN 29 LINE 9
;
L670      .TEXT $85,'MINU',$D3
          .word L649     ; link to D+
MINUS     .word *+2
          SEC
          TYA
          SBC 0,X
          STA 0,X
          TYA
          SBC 1,X
          STA 1,X
          JMP NEXT
;
;                                       DMINUS
;                                       SCREEN 29 LINE 12
;
L685      .TEXT $86,'DMINU',$D3
          .word L670     ; link to  MINUS
DMINU     .word *+2
          SEC
          TYA
          SBC 2,X
          STA 2,X
          TYA
          SBC 3,X
          STA 3,X
          JMP MINUS+3
;
;                                       OVER
;                                       SCREEN 30 LINE 1
;
L700      .TEXT $84,'OVE',$D2
          .word L685     ; link to DMINUS
OVER      .word *+2
          LDA 2,X
          PHA
          LDA 3,X
          JMP PUSH
;
;                                       DROP
;                                       SCREEN 30 LINE 4
;
L711      .TEXT $84,'DRO',$D0
          .word L700     ; link to OVER
DROP      .word POP
;
;                                       SWAP
;                                       SCREEN 30 LINE 8
;
L718      .TEXT $84,'SWA',$D0
          .word L711     ; link to DROP
SWAP      .word *+2
          LDA 2,X
          PHA
          LDA 0,X
          STA 2,X
          LDA 3,X
          LDY 1,X
          STY 3,X
          JMP PUT
;
;                                       DUP
;                                       SCREEN 30 LINE 21
;
L733      .TEXT $83,'DU',$D0
          .word L718     ; link to SWAP
DUP       .word *+2
          LDA 0,X
          PHA
          LDA 1,X
          JMP PUSH
;
;                                       +!
;                                       SCREEN 31 LINE 2
;
L744      .TEXT $82,'+',$A1
          .word L733     ; link to DUP
PSTOR     .word *+2
          CLC
          LDA (0,X)      ; fetch 16 bit value addressed by
          ADC 2,X        ; bottom of  stack, adding to
          STA (0,X)      ; second item on stack, and return
          INC 0,X        ; to memory
          BNE L754
          INC 1,X
L754      LDA (0,X)
          ADC 3,X
          STA (0,X)
          JMP POPTWO
;
;                                       TOGGLE
;                                       SCREEN 31 LINE 7
;
L762      .TEXT $81,'TOGGL',$C5
          .word L744     ; link to +!
TOGGL     .word *+2
          LDA (2,X)      ; complement bits in memory address
          EOR 0,X        ; second on stack, by pattern on
          STA (2,X)      ; bottom of stack.
          JMP POPTWO
;
;                                       @
;                                       SCREEN 32 LINE 1
;
L773      .TEXT $81,$C0
          .word L762     ; link to TOGGLE
AT        .word *+2
          LDA (0,X)
          PHA
          INC 0,X
          BNE L781
          INC 1,X
L781      LDA (0,X)
          JMP PUT
;
;                                       C@
;                                       SCREEN 32 LINE 5
;
L787      .TEXT $82,'C',$C0
          .word L773     ; link to @
CAT       .word *+2
          LDA (0,X)      ; fetch byte addressed by bottom of
          STA 0,X        ; stack to stack, zeroing the high
          STY 1,X        ; byte
          JMP NEXT
;
;                                       !
;                                       SCREEN 32 LINE 8
;
L798      .TEXT $81,$A1
          .word L787     ; link to C@
STORE     .word *+2
          LDA 2,X
          STA (0,X)      ; store second 16bit value on stack
          INC 0,X        ; to memory as addressed by bottom
          BNE L806       ; of stack.
          INC 1,X
L806      LDA 3,X
          STA (0,X)
          JMP POPTWO
;
;                                       C!
;                                       SCREEN 32 LINE 12
;
L813      .TEXT $82,'C',$A1
          .word L798     ; link to !
CSTOR     .word *+2
          LDA 2,X
          STA (0,X)
          JMP POPTWO
;
;                                       :
;                                       SCREEN 33 LINE 2
;
L823      .TEXT $C1,$BA
          .word L813     ; link to C!
COLON     .word DOCOL
          .word QEXEC
          .word SCSP
          .word CURR
          .word AT
          .word CON
          .word STORE
          .word CREAT
          .word RBRAC
          .word PSCOD
;
DOCOL     LDA IP+1
          PHA
          LDA IP
          PHA
          ; JSR TCOLON     ; mark the start of a traced : def.
          CLC
          LDA W
          ADC #2
          STA IP
          TYA
          ADC W+1
          STA IP+1
          JMP NEXT
;
;                                       ;
;                                       SCREEN 33 LINE 9
;
L853      .TEXT $C1,$BB
          .word L823     ; link to :
          .word DOCOL
          .word QCSP
          .word COMP
          .word SEMIS
          .word SMUDG
          .word LBRAC
          .word SEMIS
;
;                                       CONSTANT
;                                       SCREEN 34 LINE 1
;
L867      .TEXT $88,'CONSTAN',$D4
          .word L853     ; link to ;
CONST     .word DOCOL
          .word CREAT
          .word SMUDG
          .word COMMA
          .word PSCOD
;
DOCON     LDY #2
          LDA (W),Y
          PHA
          INY
          LDA (W),Y
          JMP PUSH
;
;                                       VARIABLE
;                                       SCREEN 34 LINE 5
;
L885      .TEXT $88,'VARIABL',$C5
          .word L867     ; link to CONSTANT
VAR       .word DOCOL
          .word CONST
          .word PSCOD
;
DOVAR     CLC
          LDA W
          ADC #2
          PHA
          TYA
          ADC W+1
          JMP PUSH
;
;                                       USER
;                                       SCREEN 34 LINE 10
;
L902      .TEXT $84,'USE',$D2
          .word L885     ; link to VARIABLE
USER      .word DOCOL
          .word CONST
          .word PSCOD
;
DOUSE     LDY #2
          CLC
          LDA (W),Y
          ADC UP
          PHA
          LDA #0
          ADC UP+1
          JMP PUSH
;
;                                       0
;                                       SCREEN 35 LINE 2
;
L920      .TEXT $81,$B0
          .word L902     ; link to USER
ZERO      .word DOCON
          .word 0
;
;                                       1
;                                       SCREEN 35 LINE 2
;
L928      .TEXT $81,$B1
          .word L920     ; link to 0
ONE       .word DOCON
          .word 1
;
;                                       2
;                                       SCREEN 35 LINE 3
;
L936      .TEXT $81,$B2
          .word L928     ; link to 1
TWO       .word DOCON
          .word 2
;
;                                       3
;                                       SCREEN 35 LINE 3
;
L944      .TEXT $81,$B3
          .word L936     ; link to 2
THREE     .word DOCON
          .word 3
;
;                                       BL
;                                       SCREEN 35 LINE 4
;
L952      .TEXT $82,'B',$CC
          .word L944     ; link to 3
BL        .word DOCON
          .word $20
;
;                                       C/L
;                                       SCREEN 35 LINE 5
;                                       Characters per line
L960      .TEXT $83,'C/',$CC
          .word L952     ; link to BL
CSLL      .word DOCON
          .word 64
;
;                                       FIRST
;                                       SCREEN 35 LINE 7
;
L968      .TEXT $85,'FIRS',$D4
          .word L960     ; link to C/L
FIRST     .word DOCON
          .word DAREA    ; bottom of disk buffer area
;
;                                       LIMIT
;                                       SCREEN 35 LINE 8
;
L976      .TEXT $85,'LIMI',$D4
          .word L968     ; link to FIRST
LIMIT     .word DOCON
          .word UAREA    ; buffers end at user area
;
;                                       B/BUF
;                                       SCREEN 35 LINE 9
;                                       Bytes per Buffer
;
L984      .TEXT $85,'B/BU',$C6
          .word L976     ; link to LIMIT
BBUF      .word DOCON
          .word SSIZE    ; sector size
;
;                                       B/SCR
;                                       SCREEN 35 LINE 10
;                                       Blocks per screen
;
L992      .TEXT $85,'B/SC',$D2
          .word L984     ; link to B/BUF
BSCR      .word DOCON
          .word 8        ; blocks to make one screen





;
;                                       +ORIGIN
;                                       SCREEN 35 LINE 12
;
L1000     .TEXT $87,'+ORIGI',$CE
          .word L992     ; link to B/SCR
PORIG     .word DOCOL
          .word LIT,ORIG
          .word PLUS
          .word SEMIS
;
;                                       TIB
;                                       SCREEN 36 LINE 4
;
L1010     .TEXT $83,'TI',$C2
          .word L1000    ; link to +ORIGIN
TIB       .word DOUSE
          .TEXT $A
;
;                                       WIDTH
;                                       SCREEN 36 LINE 5
;
L1018     .TEXT $85,'WIDT',$C8
          .word L1010    ; link to TIB
WIDTH     .word DOUSE
          .TEXT $C
;
;                                       WARNING
;                                       SCREEN 36 LINE 6
;
L1026     .TEXT $87,'WARNIN',$C7
          .word L1018    ; link to WIDTH
WARN      .word DOUSE
          .TEXT $E
;
;                                       FENCE
;                                       SCREEN 36 LINE 7
;
L1034     .TEXT $85,'FENC',$C5
          .word L1026    ; link to WARNING
FENCE     .word DOUSE
          .TEXT $10
;
;
;                                       DP
;                                       SCREEN 36 LINE 8
;
L1042     .TEXT $82,'D',$D0
          .word L1034    ; link to FENCE
DP        .word DOUSE
          .TEXT $12
;
;                                       VOC-LINK
;                                       SCREEN 36 LINE 9
;
L1050     .TEXT $88,'VOC-LIN',$CB
          .word L1042    ; link to DP
VOCL      .word DOUSE
          .TEXT $14
;
;                                       BLK
;                                       SCREEN 36 LINE 10
;
L1058     .TEXT $83,'BL',$CB
          .word L1050    ; link to VOC-LINK
BLK       .word DOUSE
          .TEXT $16
;
;                                       IN
;                                       SCREEN 36 LINE 11
;
L1066     .TEXT $82,'I',$CE
          .word L1058    ; link to BLK
IN        .word DOUSE
          .TEXT $18
;
;                                       OUT
;                                       SCREEN 36 LINE 12
;
L1074     .TEXT $83,'OU',$D4
          .word L1066    ; link to IN
OUT       .word DOUSE
          .TEXT $1A
;
;                                       SCR
;                                       SCREEN 36 LINE 13
;
L1082     .TEXT $83,'SC',$D2
          .word L1074    ; link to OUT
SCR       .word DOUSE
          .TEXT $1C
;
;                                       OFFSET
;                                       SCREEN 37 LINE 1
;
L1090     .TEXT $86,'OFFSE',$D4
          .word L1082    ; link to SCR
OFSET     .word DOUSE
          .TEXT $1E
;
;                                       CONTEXT
;                                       SCREEN 37 LINE 2
;
L1098     .TEXT $87,'CONTEX',$D4
          .word L1090    ; link to OFFSET
CON       .word DOUSE
          .TEXT $20
;
;                                       CURRENT
;                                       SCREEN 37 LINE 3
;
L1106     .TEXT $87,'CURREN',$D4
          .word L1098    ; link to CONTEXT
CURR      .word DOUSE
          .TEXT $22
;
;                                       STATE
;                                       SCREEN 37 LINE 4
;
L1114     .TEXT $85,'STAT',$C5
          .word L1106    ; link to CURRENT
STATE     .word DOUSE
          .TEXT $24
;
;                                       BASE
;                                       SCREEN 37 LINE 5
;
L1122     .TEXT $84,'BAS',$C5
          .word L1114    ; link to STATE
BASE      .word DOUSE
          .TEXT $26
;
;                                       DPL
;                                       SCREEN 37 LINE 6
;
L1130     .TEXT $83,'DP',$CC
          .word L1122    ; link to BASE
DPL       .word DOUSE
          .TEXT $28
;
;                                       FLD
;                                       SCREEN 37 LINE 7
;
L1138     .TEXT $83,'FL',$C4
          .word L1130    ; link to DPL
FLD       .word DOUSE
          .TEXT $2A
;
;
;
;                                       CSP
;                                       SCREEN 37 LINE 8
;
L1146     .TEXT $83,'CS',$D0
          .word L1138    ; link to FLD
CSP       .word DOUSE
          .TEXT $2C
;
;                                       R#
;                                       SCREEN 37  LINE 9
;
L1154     .TEXT $82,'R',$A3
          .word L1146    ; link to CSP
RNUM      .word DOUSE
          .TEXT $2E
;
;                                       HLD
;                                       SCREEN 37 LINE 10
;
L1162     .TEXT $83,'HL',$C4
          .word L1154    ; link to R#
HLD       .word DOUSE
          .TEXT $30
;
;                                       1+
;                                       SCREEN 38 LINE  1
;
L1170     .TEXT $82,'1',$AB
          .word L1162    ; link to HLD
ONEP      .word DOCOL
          .word ONE
          .word PLUS
          .word SEMIS
;
;                                       2+
;                                       SCREEN 38 LINE 2
;
L1180     .TEXT $82,'2',$AB
          .word L1170    ; link to 1+
TWOP      .word DOCOL
          .word TWO
          .word PLUS
          .word SEMIS
;
;                                       HERE
;                                       SCREEN 38 LINE 3
;
L1190     .TEXT $84,'HER',$C5
          .word L1180    ; link to 2+
HERE      .word DOCOL
          .word DP
          .word AT
          .word SEMIS
;
;                                       ALLOT
;                                       SCREEN 38 LINE 4
;
L1200     .TEXT $85,'ALLO',$D4
          .word L1190    ; link to HERE
ALLOT     .word DOCOL
          .word DP
          .word PSTOR
          .word SEMIS
;
;                                       ,
;                                       SCREEN 38 LINE 5
;
L1210     .TEXT $81,$AC
          .word L1200    ; link to ALLOT
COMMA     .word DOCOL
          .word HERE
          .word STORE
          .word TWO
          .word ALLOT
          .word SEMIS
;
;                                       C,
;                                       SCREEN 38 LINE 6
;
L1222     .TEXT $82,'C',$AC
          .word L1210    ; link to ,
CCOMM     .word DOCOL
          .word HERE
          .word CSTOR
          .word ONE
          .word ALLOT
          .word SEMIS
;
;                                       -
;                                       SCREEN 38 LINE 7
;
L1234     .TEXT $81,$AD
          .word L1222    ; link to C,
SUB       .word DOCOL
          .word MINUS
          .word PLUS
          .word SEMIS
;
;                                       =
;                                       SCREEN 38 LINE 8
;
L1244     .TEXT $81,$BD
          .word L1234    ; link to -
EQUAL     .word DOCOL
          .word SUB
          .word ZEQU
          .word SEMIS
;
;                                       U<
;                                       Unsigned less than
;
L1246     .TEXT $82,'U',$BC
          .word L1244    ; link to =
ULESS     .word DOCOL
          .word SUB      ; subtract two values
          .word ZLESS    ; test sign
          .word SEMIS
;
;                                       <
;                                       Altered from model
;                                       SCREEN 38 LINE 9
;
L1254     .TEXT $81,$BC
          .word L1246    ; link to U<
LESS      .word *+2
          SEC
          LDA 2,X
          SBC 0,X        ; subtract
          LDA 3,X
          SBC 1,X
          STY 3,X        ; zero high byte
          BVC L1258
          EOR #$80       ; correct overflow
L1258     BPL L1260
          INY            ; invert boolean
L1260     STY 2,X        ; leave boolean
          JMP POP
;
;                                       >
;                                       SCREEN 38 LINE 10
L1264     .TEXT $81,$BE
          .word L1254    ; link to <
GREAT     .word DOCOL
          .word SWAP
          .word LESS
          .word SEMIS
;
;                                       ROT
;                                       SCREEN 38 LINE 11
;
L1274     .TEXT $83,'RO',$D4
          .word L1264    ; link to >
ROT       .word DOCOL
          .word TOR
          .word SWAP
          .word RFROM
          .word SWAP
          .word SEMIS
;
;                                       SPACE
;                                       SCREEN 38 LINE 12
;
L1286     .TEXT $85,'SPAC',$C5
          .word L1274    ; link to ROT
SPACE     .word DOCOL
          .word BL
          .word EMIT
          .word SEMIS
;
;                                       -DUP
;                                       SCREEN 38 LINE 13
;
L1296     .TEXT $84,'-DU',$D0
          .word L1286    ; link to SPACE
DDUP      .word DOCOL
          .word DUP
          .word ZBRAN
L1301     .word $4       ; L1303-L1301
          .word DUP
L1303     .word SEMIS
;
;                                       TRAVERSE
;                                       SCREEN 39 LINE 14
;
L1308     .TEXT $88,'TRAVERS',$C5
          .word L1296    ; link to -DUP
TRAV      .word DOCOL
          .word SWAP
L1312     .word OVER
          .word PLUS
          .word CLIT
          .TEXT $7F
          .word OVER
          .word CAT
          .word LESS
          .word ZBRAN
L1320     .word $FFF1    ; L1312-L1320
          .word SWAP
          .word DROP
          .word SEMIS
;
;                                       LATEST
;                                       SCREEN 39 LINE 6
;
L1328     .TEXT $86,'LATES',$D4
          .word L1308    ; link to TRAVERSE
LATES     .word DOCOL
          .word CURR
          .word AT
          .word AT
          .word SEMIS
;
;
;                                       LFA
;                                       SCREEN 39 LINE 11
;
L1339     .TEXT $83,'LF',$C1
          .word L1328    ; link to LATEST
LFA       .word DOCOL
          .word CLIT
          .TEXT 4
          .word SUB
          .word SEMIS
;
;                                       CFA
;                                       SCREEN 39 LINE 12
;
L1350     .TEXT $83,'CF',$C1
          .word L1339    ; link to LFA
CFA       .word DOCOL
          .word TWO
          .word SUB
          .word SEMIS
;
;                                       NFA
;                                       SCREEN 39 LIINE 13
;
L1360     .TEXT $83,'NF',$C1
          .word L1350    ; link to CFA
NFA       .word DOCOL
          .word CLIT
          .TEXT $5
          .word SUB
          .word LIT,$FFFF
          .word TRAV
          .word SEMIS
;
;                                       PFA
;                                       SCREEN 39 LINE 14
;
L1373     .TEXT $83,'PF',$C1
          .word L1360    ; link to NFA
PFA       .word DOCOL
          .word ONE
          .word TRAV
          .word CLIT
          .TEXT 5
          .word PLUS
          .word SEMIS
;
;                                       !CSP
;                                       SCREEN 40 LINE 1
;
L1386     .TEXT $84,'!CS',$D0
          .word L1373    ; link to PFA
SCSP      .word DOCOL
          .word SPAT
          .word CSP
          .word STORE
          .word SEMIS
;
;                                       ?ERROR
;                                       SCREEN 40 LINE 3
;
L1397     .TEXT $86,'?ERRO',$D2
          .word L1386    ; link to !CSP
QERR      .word DOCOL
          .word SWAP
          .word ZBRAN
L1402     .word 8        ; L1406-L1402
          .word ERROR
          .word BRAN
L1405     .word 4        ; L1407-L1405
L1406     .word DROP
L1407     .word SEMIS
;
;                                       ?COMP
;                                       SCREEN 40 LINE 6
;
L1412     .TEXT $85,'?COM',$D0
          .word L1397    ; link to ?ERROR
QCOMP     .word DOCOL
          .word STATE
          .word AT
          .word ZEQU
          .word CLIT
          .TEXT $11
          .word QERR
          .word SEMIS
;
;                                       ?EXEC
;                                       SCREEN 40 LINE 8
;
L1426     .TEXT $85,'?EXE',$C3
          .word L1412    ; link to ?COMP
QEXEC     .word DOCOL
          .word STATE
          .word AT
          .word CLIT
          .TEXT $12
          .word QERR
          .word SEMIS
;
;                                       ?PAIRS
;                                       SCREEN 40 LINE 10
;
L1439     .TEXT $86,'?PAIR',$D3
          .word L1426    ; link to ?EXEC
QPAIR     .word DOCOL
          .word SUB
          .word CLIT
          .TEXT $13
          .word QERR
          .word SEMIS
;
;                                       ?CSP
;                                       SCREEN 40 LINE 12
;
L1451     .TEXT $84,'?CS',$D0
          .word L1439    ; link to ?PAIRS
QCSP      .word DOCOL
          .word SPAT
          .word CSP
          .word AT
          .word SUB
          .word CLIT
          .TEXT $14
          .word QERR
          .word SEMIS
;
;                                       ?LOADING
;                                       SCREEN 40 LINE 14
;
L1466     .TEXT $88,'?LOADIN',$C7
          .word L1451    ; link to ?CSP
QLOAD     .word DOCOL
          .word BLK
          .word AT
          .word ZEQU
          .word CLIT
          .TEXT $16
          .word QERR
          .word SEMIS
;
;                                       COMPILE
;                                       SCREEN 41 LINE 2
;
L1480     .TEXT $87,'COMPIL',$C5
          .word L1466    ; link to ?LOADING
COMP      .word DOCOL
          .word QCOMP
          .word RFROM
          .word DUP
          .word TWOP
          .word TOR
          .word AT
          .word COMMA
          .word SEMIS
;
;                                       [
;                                       SCREEN 41 LINE 5
;
L1495     .TEXT $C1,$DB
          .word L1480    ; link to COMPILE
LBRAC     .word DOCOL
          .word ZERO
          .word STATE
          .word STORE
          .word SEMIS
;
;                                       ]
;                                       SCREEN 41 LINE 7
;
L1507     .TEXT $81,$DD
          .word L1495    ; link to [
RBRAC     .word DOCOL
          .word CLIT
          .TEXT $C0
          .word STATE
          .word STORE
          .word SEMIS
;
;                                       SMUDGE
;                                       SCREEN 41 LINE 9
;
L1519     .TEXT $86,'SMUDG',$C5
          .word L1507    ; link to ]
SMUDG     .word DOCOL
          .word LATES
          .word CLIT
          .TEXT $20
          .word TOGGL
          .word SEMIS
;
;                                       HEX
;                                       SCREEN 41 LINE 11
;
L1531     .TEXT $83,'HE',$D8
          .word L1519    ; link to SMUDGE
HEX       .word DOCOL
          .word CLIT
          .TEXT 16
          .word BASE
          .word STORE
          .word SEMIS
;
;                                       DECIMAL
;                                       SCREEN 41 LINE 13
;
L1543     .TEXT $87,'DECIMA',$CC
          .word L1531    ; link to HEX
DECIM     .word DOCOL
          .word CLIT
          .TEXT 10
          .word BASE
          .word STORE
          .word SEMIS
;
;
;
;                                       (;CODE)
;                                       SCREEN 42 LINE 2
;
L1555     .TEXT $87,'(;CODE',$A9
          .word L1543    ; link to DECIMAL
PSCOD     .word DOCOL
          .word RFROM
          .word LATES
          .word PFA
          .word CFA
          .word STORE
          .word SEMIS
;
;                                       ;CODE
;                                       SCREEN 42 LINE 6
;
L1568     .TEXT $C5,';COD',$C5
          .word L1555    ; link to (;CODE)
          .word DOCOL
          .word QCSP
          .word COMP
          .word PSCOD
          .word LBRAC
          .word SMUDG
          .word SEMIS
;
;                                       <BUILDS
;                                       SCREEN 43 LINE 2
;
L1582     .TEXT $87,'<BUILD',$D3
          .word L1568    ; link to ;CODE
BUILD     .word DOCOL
          .word ZERO
          .word CONST
          .word SEMIS
;
;                                       DOES>
;                                       SCREEN 43 LINE 4
;
L1592     .TEXT $85,'DOES',$BE
          .word L1582    ; link to <BUILDS
DOES      .word DOCOL
          .word RFROM
          .word LATES
          .word PFA
          .word STORE
          .word PSCOD
;
DODOE     LDA IP+1
          PHA
          LDA IP
          PHA
          LDY #2
          LDA (W),Y
          STA IP
          INY
          LDA (W),Y
          STA IP+1
          CLC
          LDA W
          ADC #4
          PHA
          LDA W+1
          ADC #0
          JMP PUSH
;
;                                       COUNT
;                                       SCREEN 44 LINE 1
;
L1622     .TEXT $85,'COUN',$D4
          .word L1592    ; link to DOES>
COUNT     .word DOCOL
          .word DUP
          .word ONEP
          .word SWAP
          .word CAT
          .word SEMIS
;
;                                       TYPE_W
;                                       SCREEN 44 LINE 2
;
L1634     .TEXT $84,'TYP',$C5
          .word L1622    ; link to COUNT
TYPE_W      .word DOCOL
          .word DDUP
          .word ZBRAN
L1639     .word $18      ; L1651-L1639
          .word OVER
          .word PLUS
          .word SWAP
          .word PDO
L1644     .word I
          .word CAT
          .word EMIT
          .word PLOOP
L1648     .word $FFF8    ; L1644-L1648
          .word BRAN
L1650     .word $4       ; L1652-L1650
L1651     .word DROP
L1652     .word SEMIS
;
;                                       -TRAILING
;                                       SCREEN 44 LINE 5
;
L1657     .TEXT $89,'-TRAILIN',$C7
          .word L1634    ; link to TYPE_W
DTRAI     .word DOCOL
          .word DUP
          .word ZERO
          .word PDO
L1663     .word OVER
          .word OVER
          .word PLUS
          .word ONE
          .word SUB
          .word CAT
          .word BL
          .word SUB
          .word ZBRAN
L1672     .word 8        ; L1676-L1672
          .word LEAVE
          .word BRAN
L1675     .word 6        ; L1678-L1675
L1676     .word ONE
          .word SUB
L1678     .word PLOOP
L1679     .word $FFE0    ; L1663-L1679
          .word SEMIS
;
;                                       (.")
;                                       SCREEN 44 LINE 8
L1685     .TEXT $84,'(."',$A9
          .word L1657    ; link to -TRAILING
PDOTQ     .word DOCOL
          .word R
          .word COUNT
          .word DUP
          .word ONEP
          .word RFROM
          .word PLUS
          .word TOR
          .word TYPE_W
          .word SEMIS
;
;                                       ."
;                                       SCREEN 44 LINE12
;
L1701     .TEXT $C2,'.',$A2
          .word L1685    ; link to PDOTQ
          .word DOCOL
          .word CLIT
          .TEXT $22
          .word STATE
          .word AT
          .word ZBRAN
L1709     .word $14      ;L1719-L1709
          .word COMP
          .word PDOTQ
          .word WORD_W
          .word HERE
          .word CAT
          .word ONEP
          .word ALLOT
          .word BRAN
L1718     .word $A       ;L1723-L1718
L1719     .word WORD_W
          .word HERE
          .word COUNT
          .word TYPE_W
L1723     .word SEMIS
;
;                                       EXPECT
;                                       SCREEN 45 LINE 2
;
L1729     .TEXT $86,'EXPEC',$D4
          .word L1701    ; link to ."
EXPEC     .word DOCOL
          .word OVER
          .word PLUS
          .word OVER
          .word PDO
L1736     .word KEY
          .word DUP
          .word CLIT
          .TEXT $E
          .word PORIG
          .word AT
          .word EQUAL
          .word ZBRAN
L1744     .word $1F       ; L1760-L1744
          .word DROP
          .word CLIT
          .TEXT 8
          .word OVER
          .word I
          .word EQUAL
          .word DUP
          .word RFROM
          .word TWO
          .word SUB
          .word PLUS
          .word TOR
          .word SUB
          .word BRAN
L1759     .word $27       ; L1779-L1759
L1760     .word DUP
          .word CLIT
          .TEXT $0D
          .word EQUAL
          .word ZBRAN
L1765     .word $0E       ; L1772-L1765
          .word LEAVE
          .word DROP
          .word BL
          .word ZERO
          .word BRAN
L1771     .word 4         ; L1773-L1771
L1772     .word DUP
L1773     .word I
          .word CSTOR
          .word ZERO
          .word I
          .word ONEP
          .word STORE
L1779     .word EMIT
          .word PLOOP
L1781     .word $FFA9
          .word DROP      ; L1736-L1781
          .word SEMIS
;
;                                       QUERY
;                                       SCREEN 45 LINE 9
;
L1788     .TEXT $85,'QUER',$D9
          .word L1729    ; link to EXPECT
QUERY     .word DOCOL
          .word TIB
          .word AT
          .word CLIT
          .TEXT 80       ; 80 characters from terminal
          .word EXPEC
          .word ZERO
          .word IN
          .word STORE
          .word SEMIS
;
;                                       X
;                                       SCREEN 45 LINE 11
;                                       Actually Ascii Null
;
L1804     .TEXT $C1,$80
          .word L1788    ; link to QUERY
          .word DOCOL
          .word BLK
          .word AT
          .word ZBRAN
L1810     .word $2A      ; L1830-l1810
          .word ONE
          .word BLK
          .word PSTOR
          .word ZERO
          .word IN
          .word STORE
          .word BLK
          .word AT
          .word ZERO,BSCR
          .word USLAS
          .word DROP     ; fixed from model
          .word ZEQU
          .word ZBRAN
L1824     .word 8        ; L1828-L1824
          .word QEXEC
          .word RFROM
          .word DROP
L1828     .word BRAN
L1829     .word 6        ; L1832-L1829
L1830     .word RFROM
          .word DROP
L1832     .word SEMIS
;
;                                       FILL
;                                       SCREEN 46 LINE 1
;
;
L1838     .TEXT $84,'FIL',$CC
          .word L1804    ; link to X
FILL      .word DOCOL
          .word SWAP
          .word TOR
          .word OVER
          .word CSTOR
          .word DUP
          .word ONEP
          .word RFROM
          .word ONE
          .word SUB
          .word CMOVE
          .word SEMIS
;
;                                       ERASE
;                                       SCREEN 46 LINE 4
;
L1856     .TEXT $85,'ERAS',$C5
          .word L1838    ; link to FILL
ERASE     .word DOCOL
          .word ZERO
          .word FILL
          .word SEMIS
;
;                                       BLANKS
;                                       SCREEN 46 LINE 7
;
L1866     .TEXT $86,'BLANK',$D3
          .word L1856    ; link to ERASE
BLANK     .word DOCOL
          .word BL
          .word FILL
          .word SEMIS
;
;                                       HOLD
;                                       SCREEN 46 LINE 10
;
L1876     .TEXT $84,'HOL',$C4
          .word L1866    ; link to BLANKS
HOLD      .word DOCOL
          .word LIT,$FFFF
          .word HLD
          .word PSTOR
          .word HLD
          .word AT
          .word CSTOR
          .word SEMIS
;
;                                       PAD
;                                       SCREEN 46 LINE 13
;
L1890     .TEXT $83,'PA',$C4
          .word L1876    ; link to HOLD
PAD       .word DOCOL
          .word HERE
          .word CLIT
          .TEXT 68       ; PAD is 68 bytes above here.
          .word PLUS
          .word SEMIS
;
;                                       WORD_W
;                                       SCREEN 47 LINE 1
;
L1902     .TEXT $84,'WOR',$C4
          .word L1890    ; link to PAD
WORD_W      .word DOCOL
          .word BLK
          .word AT
          .word ZBRAN
L1908     .word $C       ; L1914-L1908
          .word BLK
          .word AT
          .word BLOCK
          .word BRAN
L1913     .word $6       ; L1916-L1913
L1914     .word TIB
          .word AT
L1916     .word IN
          .word AT
          .word PLUS
          .word SWAP
          .word ENCL
          .word HERE
          .word CLIT
          .TEXT $22
          .word BLANK
          .word IN
          .word PSTOR
          .word OVER
          .word SUB
          .word TOR
          .word R
          .word HERE
          .word CSTOR
          .word PLUS
          .word HERE
          .word ONEP
          .word RFROM
          .word CMOVE
          .word SEMIS
;
;                                       UPPER
;                                       SCREEN 47 LINE 12
;
L1943     .TEXT $85,'UPPE',$D2
          .word L1902    ; link to WORD_W
UPPER     .word DOCOL
          .word OVER     ; This routine converts text to U case
          .word PLUS     ; It allows interpretation from a term.
          .word SWAP     ; without a shift-lock.
          .word PDO
L1950     .word I
          .word CAT
          .word CLIT
          .TEXT $5F
          .word GREAT
          .word ZBRAN
L1956     .word 9       ; L1961-L1956
          .word I
          .word CLIT
          .TEXT $20
          .word TOGGL
L1961     .word PLOOP
L1962     .word $FFEA    ; L1950-L1962
          .word SEMIS
;
;                                       (NUMBER)
;                                       SCREEN 48 LINE 1
;
L1968     .TEXT $88,'(NUMBER',$A9
          .word L1943    ; link to UPPER
PNUMB     .word DOCOL
L1971     .word ONEP
          .word DUP
          .word TOR
          .word CAT
          .word BASE
          .word AT
          .word DIGIT
          .word ZBRAN
L1979     .word $2C      ; L2001-L1979
          .word SWAP
          .word BASE
          .word AT
          .word USTAR
          .word DROP
          .word ROT
          .word BASE
          .word AT
          .word USTAR
          .word DPLUS
          .word DPL
          .word AT
          .word ONEP
          .word ZBRAN
L1994     .word 8        ; L1998-L1994
          .word ONE
          .word DPL
          .word PSTOR
L1998     .word RFROM
          .word BRAN
L2000     .word $FFC6    ; L1971-L2000
L2001     .word RFROM
          .word SEMIS
;
;                                       NUMBER
;                                       SCREEN 48 LINE 6
;
L2007     .TEXT $86,'NUMBE',$D2
          .word L1968    ; link to (NUMBER)
NUMBER    .word DOCOL
          .word ZERO
          .word ZERO
          .word ROT
          .word DUP
          .word ONEP
          .word CAT
          .word CLIT
          .TEXT $2D
          .word EQUAL
          .word DUP
          .word TOR
          .word PLUS
          .word LIT,$FFFF
L2023     .word DPL
          .word STORE
          .word PNUMB
          .word DUP
          .word CAT
          .word BL
          .word SUB
          .word ZBRAN
L2031     .word $15      ; L2042-L2031
          .word DUP
          .word CAT
          .word CLIT
          .TEXT $2E
          .word SUB
          .word ZERO
          .word QERR
          .word ZERO
          .word BRAN
L2041     .word $FFDD    ; L2023-L2041
L2042     .word DROP
          .word RFROM
          .word ZBRAN
L2045     .word 4        ; L2047-L2045
          .word DMINU
L2047     .word SEMIS
;
;                                       -FIND
;                                       SCREEN 48 LINE 12
;
L2052     .TEXT $85,'-FIN',$C4
          .word L2007    ; link to NUMBER
DFIND     .word DOCOL
          .word BL
          .word WORD_W
          .word HERE     ; )
          .word COUNT    ; |- Optional allowing free use of low
          .word UPPER    ; )  case from terminal
          .word HERE
          .word CON
          .word AT
          .word AT
          .word PFIND
          .word DUP
          .word ZEQU
          .word ZBRAN
L2068     .word $A       ; L2073-L2068
          .word DROP
          .word HERE
          .word LATES
          .word PFIND
L2073     .word SEMIS
;
;                                       (ABORT)
;                                       SCREEN 49 LINE 2
;
L2078     .TEXT $87,'(ABORT',$A9
          .word L2052    ; link to -FIND
PABOR     .word DOCOL
          .word ABORT
          .word SEMIS
;
;                                       ERROR
;                                       SCREEN 49 LINE 4
;
L2087     .TEXT $85,'ERRO',$D2
          .word L2078    ; link to (ABORT)
ERROR     .word DOCOL
          .word WARN
          .word AT
          .word ZLESS
          .word ZBRAN
L2094     .word $4       ; L2096-L2094
          .word PABOR
L2096     .word HERE
          .word COUNT
          .word TYPE_W
          .word PDOTQ
          .TEXT 4,'  ? '
          .word MESS
          .word SPSTO
          .word DROP,DROP; make room for 2 error values
          .word IN
          .word AT
          .word BLK
          .word AT
          .word QUIT
          .word SEMIS
;
;                                       ID.
;                                       SCREEN 49 LINE 9
;
L2113     .TEXT $83,'ID',$AE
          .word L2087    ; link to ERROR
IDDOT     .word DOCOL
          .word PAD
          .word CLIT
          .TEXT $20
          .word CLIT
          .TEXT $5F
          .word FILL
          .word DUP
          .word PFA
          .word LFA
          .word OVER
          .word SUB
          .word PAD
          .word SWAP
          .word CMOVE
          .word PAD
          .word COUNT
          .word CLIT
          .TEXT $1F
          .word ANDD
          .word TYPE_W
          .word SPACE
          .word SEMIS
;
;                                       CREATE
;                                       SCREEN 50 LINE 2
;
L2142     .TEXT $86,'CREAT',$C5
          .word L2113    ; link to ID
CREAT     .word DOCOL
          .word TIB      ;)
          .word HERE     ;|
          .word CLIT     ;|  6502 only, assures
          .TEXT $A0      ;|  room exists in dict.
          .word PLUS     ;|
          .word ULESS    ;|
          .word TWO      ;|
          .word QERR     ;)
          .word DFIND
          .word ZBRAN
L2155     .word $0F
          .word DROP
          .word NFA
          .word IDDOT
          .word CLIT
          .TEXT 4
          .word MESS
          .word SPACE
L2163     .word HERE
          .word DUP
          .word CAT
          .word WIDTH
          .word AT
          .word MIN
          .word ONEP
          .word ALLOT
          .word DP       ;)
          .word CAT      ;| 6502 only. The code field
          .word CLIT     ;| must not straddle page
          .TEXT $FD      ;| boundaries
          .word EQUAL    ;|
          .word ALLOT    ;)
          .word DUP
          .word CLIT
          .TEXT $A0
          .word TOGGL
          .word HERE
          .word ONE
          .word SUB
          .word CLIT
          .TEXT $80
          .word TOGGL
          .word LATES
          .word COMMA
          .word CURR
          .word AT
          .word STORE
          .word HERE
          .word TWOP
          .word COMMA
          .word SEMIS
;
;                                       [COMPILE]
;                                       SCREEN 51 LINE 2
;
L2200     .TEXT $C9,'[COMPILE',$DD
          .word L2142    ; link to CREATE
          .word DOCOL
          .word DFIND
          .word ZEQU
          .word ZERO
          .word QERR
          .word DROP
          .word CFA
          .word COMMA
          .word SEMIS
;
;                                       LITERAL
;                                       SCREEN 51 LINE 2
;
L2216     .TEXT $C7,'LITERA',$CC
          .word L2200    ; link to [COMPILE]
LITER     .word DOCOL
          .word STATE
          .word AT
          .word ZBRAN
L2222     .word 8        ; L2226-L2222
          .word COMP
          .word LIT
          .word COMMA
L2226     .word SEMIS
;
;                                       DLITERAL
;                                       SCREEN 51 LINE 8
;
L2232     .TEXT $C8,'DLITERA',$CC
          .word L2216    ; link to LITERAL
DLIT      .word DOCOL
          .word STATE
          .word AT
          .word ZBRAN
L2238     .word 8        ; L2242-L2238
          .word SWAP
          .word LITER
          .word LITER
L2242     .word SEMIS
;
;                                       ?STACK
;                                       SCREEN 51 LINE 13
;
L2248     .TEXT $86,'?STAC',$CB
          .word L2232    ; link to DLITERAL
QSTAC     .word DOCOL
          .word CLIT
          .TEXT TOS
          .word SPAT
          .word ULESS
          .word ONE
          .word QERR
          .word SPAT
          .word CLIT
          .TEXT BOS
          .word ULESS
          .word CLIT
          .TEXT 7
          .word QERR
          .word SEMIS
;
;                                       INTERPRET
;                                       SCREEN 52 LINE 2
;
L2269     .TEXT $89,'INTERPRE',$D4
          .word L2248    ; link to ?STACK
INTER     .word DOCOL
L2272     .word DFIND
          .word ZBRAN
L2274     .word $1E      ; L2289-L2274
          .word STATE
          .word AT
          .word LESS
          .word ZBRAN
L2279     .word $A       ; L2284-L2279
          .word CFA
          .word COMMA
          .word BRAN
L2283     .word $6       ; L2286-L2283
L2284     .word CFA
          .word EXEC
L2286     .word QSTAC
          .word BRAN
L2288     .word $1C      ; L2302-L2288
L2289     .word HERE
          .word NUMBER
          .word DPL
          .word AT
          .word ONEP
          .word ZBRAN
L2295     .word 8        ; L2299-L2295
          .word DLIT
          .word BRAN
L2298     .word $6       ; L2301-L2298
L2299     .word DROP
          .word LITER
L2301     .word QSTAC
L2302     .word BRAN
L2303     .word $FFC2    ; L2272-L2303
;
;                                       IMMEDIATE
;                                       SCREEN 53 LINE 1
;
L2309     .TEXT $89,'IMMEDIAT',$C5
          .word L2269;   ; link to INTERPRET
          .word DOCOL
          .word LATES
          .word CLIT
          .TEXT $40
          .word TOGGL
          .word SEMIS
;
;                                       VOCABULARY
;                                       SCREEN 53 LINE 4
;
L2321     .TEXT $8A,'VOCABULAR',$D9
          .word L2309    ; link to IMMEDIATE
          .word DOCOL
          .word BUILD
          .word LIT,$A081
          .word COMMA
          .word CURR
          .word AT
          .word CFA
          .word COMMA
          .word HERE
          .word VOCL
          .word AT
          .word COMMA
          .word VOCL
          .word STORE
          .word DOES
DOVOC     .word TWOP
          .word CON
          .word STORE
          .word SEMIS
;
;                                       FORTH
;                                       SCREEN 53 LINE 9
;
L2346     .TEXT $C5,'FORT',$C8
          .word L2321    ; link to VOCABULARY
FORTH     .word DODOE
          .word DOVOC
          .word $A081
XFOR      .word NTOP     ; points to top name in FORTH
VL0       .word 0        ; last vocab link ends at zero
;
;                                       DEFINITIONS
;                                       SCREEN 53 LINE 11
;
;
L2357     .TEXT $8B,'DEFINITION',$D3
          .word L2346    ; link to FORTH
DEFIN     .word DOCOL
          .word CON
          .word AT
          .word CURR
          .word STORE
          .word SEMIS
;
;                                       (
;                                       SCREEN 53 LINE 14
;
L2369     .TEXT $C1,$A8
          .word L2357    ; link to DEFINITIONS
          .word DOCOL
          .word CLIT
          .TEXT $29
          .word WORD_W
          .word SEMIS
;
;                                       QUIT
;                                       SCREEN 54 LINE 2
;
L2381     .TEXT $84,'QUI',$D4
          .word L2369    ; link to (
QUIT      .word DOCOL
          .word ZERO
          .word BLK
          .word STORE
          .word LBRAC
L2388     .word RPSTO
          .word CR
          .word QUERY
          .word INTER
          .word STATE
          .word AT
          .word ZEQU
          .word ZBRAN
L2396     .word 7        ; L2399-L2396
          .word PDOTQ
          .TEXT 2,'OK'
L2399     .word BRAN
L2400     .word $FFE7    ; L2388-L2400
          .word SEMIS
;
;                                       ABORT
;                                       SCREEN 54 LINE 7
;
L2406     .TEXT $85,'ABOR',$D4
          .word L2381    ; link to QUIT
ABORT     .word DOCOL
          .word SPSTO
          .word DECIM
          .word DR0
          .word CR
          .word PDOTQ
          .TEXT 14,'fig-FORTH  1.0'
          .word FORTH
          .word DEFIN
          .word QUIT
;
;                                       COLD
;                                       SCREEN 55 LINE 1
;
L2423     .TEXT $84,'COL',$C4
          .word L2406    ; link to ABORT
COLD      .word *+2

          LDA ORIG+$0C   ; from cold start area
          STA FORTH+6
          LDA ORIG+$0D
          STA FORTH+7
          LDY #$15
          BNE L2433
WARM      LDY #$0F
L2433     LDA ORIG+$10
          STA UP
          LDA ORIG+$11
          STA UP+1
L2437     LDA ORIG+$0C,Y
          STA (UP),Y
          DEY
          BPL  L2437
          LDA #>ABORT    ; actually #>(ABORT+2)
          STA IP+1
          LDA #<ABORT+2
          STA IP
          CLD
          LDA #$6C
          STA W-1
          JMP RPSTO+2    ; And off we go !
;
;                                       S->D
;                                       SCREEN 56 LINE 1
;
L2453     .TEXT $84,'S->',$C4
          .word L2423    ; link to COLD
STOD      .word DOCOL
          .word DUP
          .word ZLESS
          .word MINUS
          .word SEMIS
;
;                                       +-
;                                       SCREEN 56 LINE 4
;
L2464     .TEXT $82,'+',$AD
          .word L2453    ; link to S->D
PM        .word DOCOL
          .word ZLESS
          .word ZBRAN
L2469     .word 4
          .word MINUS
L2471     .word SEMIS
;
;                                       D+-
;                                       SCREEN 56 LINE 6
;
L2476     .TEXT $83,'D+',$AD
          .word L2464    ; link to +-
DPM       .word DOCOL
          .word ZLESS
          .word ZBRAN
L2481     .word 4        ; L2483-L2481
          .word DMINU
L2483     .word SEMIS
;
;                                       ABS_W
;                                       SCREEN 56 LINE 9
;
L2488     .TEXT $83,'AB',$D3
          .word L2476    ; link to D+-
ABS_W       .word DOCOL
          .word DUP
          .word PM
          .word SEMIS
;
;                                       DABS_W
;                                       SCREEN 56 LINE 10
;
L2498     .TEXT $84,'DAB',$D3
          .word L2488    ; link to ABS_W
DABS_W      .word DOCOL
          .word DUP
          .word DPM
          .word SEMIS
;
;                                       MIN
;                                       SCREEN 56 LINE 12
;
L2508     .TEXT $83,'MI',$CE
          .word L2498    ; link to DABS_W
MIN       .word DOCOL
          .word OVER
          .word OVER
          .word GREAT
          .word ZBRAN
L2515     .word 4        ; L2517-L2515
          .word SWAP
L2517     .word DROP
          .word SEMIS
;
;                                       MAX
;                                       SCREEN 56 LINE 14
;
L2523     .TEXT $83,'MA',$D8
          .word L2508     ; link to MIN
MAX       .word DOCOL
          .word OVER
          .word OVER
          .word LESS
          .word ZBRAN
L2530     .word 4        ; L2532-L2530
          .word SWAP
L2532     .word DROP
          .word SEMIS
;
;                                       M*
;                                       SCREEN 57 LINE 1
;
L2538     .TEXT $82,'M',$AA
          .word L2523    ; link to MAX
MSTAR     .word DOCOL
          .word OVER
          .word OVER
          .word XOR
          .word TOR
          .word ABS_W
          .word SWAP
          .word ABS_W
          .word USTAR
          .word RFROM
          .word DPM
          .word SEMIS
;
;                                       M/
;                                       SCREEN 57 LINE 3
;
L2556     .TEXT $82,'M',$AF
          .word L2538    ; link to M*
MSLAS     .word DOCOL
          .word OVER
          .word TOR
          .word TOR
          .word DABS_W
          .word R
          .word ABS_W
          .word USLAS
          .word RFROM
          .word R
          .word XOR
          .word PM
          .word SWAP
          .word RFROM
          .word PM
          .word SWAP
          .word SEMIS
;
;                                       *
;                                       SCREEN 57 LINE 7
;
L2579     .TEXT $81,$AA
          .word L2556    ; link to M/
STAR      .word DOCOL
          .word USTAR
          .word DROP
          .word SEMIS
;
;                                       /MOD
;                                       SCREEN 57 LINE 8
;
L2589     .TEXT $84,'/MO',$C4
          .word L2579    ; link to *
SLMOD     .word DOCOL
          .word TOR
          .word STOD
          .word RFROM
          .word MSLAS
          .word SEMIS
;
;                                       /
;                                       SCREEN 57 LINE 9
;
L2601     .TEXT $81,$AF
          .word L2589    ; link to /MOD
SLASH     .word DOCOL
          .word SLMOD
          .word SWAP
          .word DROP
          .word SEMIS
;
;                                       MOD
;                                       SCREEN 57 LINE 10
;
L2612     .TEXT $83,'MO',$C4
          .word L2601    ; link to /
MOD       .word DOCOL
          .word SLMOD
          .word DROP
          .word SEMIS
;
;                                       */MOD
;                                       SCREEN 57 LINE 11
;
L2622     .TEXT $85,'*/MO',$C4
          .word L2612    ; link to MOD
SSMOD     .word DOCOL
          .word TOR
          .word MSTAR
          .word RFROM
          .word MSLAS
          .word SEMIS
;
;                                       */
;                                       SCREEN 57 LINE 13
;
L2634     .TEXT $82,'*',$AF
          .word L2622    ; link to */MOD
SSLAS     .word DOCOL
          .word SSMOD
          .word SWAP
          .word DROP
          .word SEMIS
;
;                                       M/MOD
;                                       SCREEN 57 LINE 14
;
L2645     .TEXT $85,'M/MO',$C4
          .word L2634    ; link to */
MSMOD     .word DOCOL
          .word TOR
          .word ZERO
          .word R
          .word USLAS
          .word RFROM
          .word SWAP
          .word TOR
          .word USLAS
          .word RFROM
          .word SEMIS
;
;                                       USE
;                                       SCREEN 58 LINE 1
;
L2662     .TEXT $83,'US',$C5
          .word L2645    ; link to M/MOD
USE       .word DOVAR
          .word DAREA
;
;                                       PREV
;                                       SCREEN 58 LINE 2
;
L2670     .TEXT $84,'PRE',$D6
          .word L2662    ; link to USE
PREV      .word DOVAR
          .word DAREA
;
;                                       +BUF
;                                       SCREEN 58 LINE 4
;
;
L2678     .TEXT $84,'+BU',$C6
          .word L2670    ; link to PREV
PBUF      .word DOCOL
          .word LIT
          .word SSIZE+4  ; hold block #, one sector two num
          .word PLUS
          .word DUP
          .word LIMIT
          .word EQUAL
          .word ZBRAN
L2688     .word 6        ; L2691-L2688
          .word DROP
          .word FIRST
L2691     .word DUP
          .word PREV
          .word AT
          .word SUB
          .word SEMIS
;
;                                       UPDATE
;                                       SCREEN 58 LINE 8
;
L2700     .TEXT $86,'UPDAT',$C5
          .word L2678    ; link to +BUF
UPDAT     .word DOCOL
          .word PREV
          .word AT
          .word AT
          .word LIT,$8000
          .word OR
          .word PREV
          .word AT
          .word STORE
          .word SEMIS
;
;                                       FLUSH
;
L2705     .TEXT $85,'FLUS',$C8
          .word L2700    ; link to UPDATE
          .word DOCOL
          .word LIMIT,FIRST,SUB
          .word BBUF,CLIT
          .TEXT 4
          .word PLUS,SLASH,ONEP
          .word ZERO,PDO
L2835     .word LIT,$7FFF,BUFFR
          .word DROP,PLOOP
L2839     .word $FFF6    ; L2835-L2839
          .word SEMIS
;
;                                       EMPTY-BUFFERS
;                                       SCREEN 58 LINE 11
;
L2716     .TEXT $8D,'EMPTY-BUFFER',$D3
          .word L2705    ; link to FLUSH
          .word DOCOL
          .word FIRST
          .word LIMIT
          .word OVER
          .word SUB
          .word ERASE
          .word SEMIS
;
;                                       DR0
;                                       SCREEN 58 LINE 14
;
L2729     .TEXT $83,'DR',$B0
          .word L2716    ; link to EMPTY-BUFFERS
DR0       .word DOCOL
          .word ZERO
          .word OFSET
          .word STORE
          .word SEMIS
;
;                                       DR1
;                                       SCREEN 58 LINE 15
;
L2740     .TEXT $83,'DR',$B1
          .word L2729    ; link to DR0
          .word DOCOL
          .word LIT,SECTR ; sectors per drive
          .word OFSET
          .word STORE
          .word SEMIS
;
;                                       BUFFER
;                                       SCREEN 59 LINE 1
;
L2751     .TEXT $86,'BUFFE',$D2
          .word L2740    ; link to DR1
BUFFR     .word DOCOL
          .word USE
          .word AT
          .word DUP
          .word TOR
L2758     .word PBUF
          .word ZBRAN
L2760     .word $FFFC    ; L2758-L2760
          .word USE
          .word STORE
          .word R
          .word AT
          .word ZLESS
          .word ZBRAN
L2767     .word $14      ; L2776-L2767
          .word R
          .word TWOP
          .word R
          .word AT
          .word LIT,$7FFF
          .word ANDD
          .word ZERO
          .word RSLW
L2776     .word R
          .word STORE
          .word R
          .word PREV
          .word STORE
          .word RFROM
          .word TWOP
          .word SEMIS
;
;                                       BLOCK
;                                       SCREEN 60 LINE 1
;
L2788     .TEXT $85,'BLOC',$CB
          .word L2751    ; link to BUFFER
BLOCK     .word DOCOL
          .word OFSET
          .word AT
          .word PLUS
          .word TOR
          .word PREV
          .word AT
          .word DUP
          .word AT
          .word R
          .word SUB
          .word DUP
          .word PLUS
          .word ZBRAN
L2804     .word $34      ; L2830-L2804
L2805     .word PBUF
          .word ZEQU
          .word ZBRAN
L2808     .word $14      ; L2818-L2808
          .word DROP
          .word R
          .word BUFFR
          .word DUP
          .word R
          .word ONE
          .word RSLW
          .word TWO
          .word SUB
L2818     .word DUP
          .word AT
          .word R
          .word SUB
          .word DUP
          .word PLUS
          .word ZEQU
          .word ZBRAN
L2826     .word $FFD6    ; L2805-L2826
          .word DUP
          .word PREV
          .word STORE
L2830     .word RFROM
          .word DROP
          .word TWOP
          .word SEMIS    ; end of BLOCK
;
;
;                                       (LINE)
;                                       SCREEN 61 LINE 2
;
L2838     .TEXT $86,'(LINE',$A9
          .word L2788    ; link to BLOCK
PLINE     .word DOCOL
          .word TOR
          .word CSLL
          .word BBUF
          .word SSMOD
          .word RFROM
          .word BSCR
          .word STAR
          .word PLUS
          .word BLOCK
          .word PLUS
          .word CSLL
          .word SEMIS
;
;                                       .LINE
;                                       SCREEN 61 LINE 6
;
L2857     .TEXT $85,'.LIN',$C5
          .word L2838    ; link to (LINE)
DLINE     .word DOCOL
          .word PLINE
          .word DTRAI
          .word TYPE_W
          .word SEMIS
;
;                                       MESSAGE
;                                       SCREEN 61 LINE 9
;
L2868     .TEXT $87,'MESSAG',$C5
          .word L2857    ; link to .LINE
MESS      .word DOCOL
          .word WARN
          .word AT
          .word ZBRAN
L2874     .word $1B      ; L2888-L2874
          .word DDUP
          .word ZBRAN
L2877     .word $11      ; L2886-L2877
          .word CLIT
          .TEXT 4
          .word OFSET
          .word AT
          .word BSCR
          .word SLASH
          .word SUB
          .word DLINE
L2886     .word BRAN
L2887     .word 13       ; L2891-L2887
L2888     .word PDOTQ
          .TEXT 6,'MSG # '
          .word DOT
L2891     .word SEMIS
;
;                                       LOAD
;                                       SCREEN 62 LINE 2
;
L2896     .TEXT $84,'LOA',$C4
          .word L2868    ; link to MESSAGE
LOAD      .word DOCOL
          .word BLK
          .word AT
          .word TOR
          .word IN
          .word AT
          .word TOR
          .word ZERO
          .word IN
          .word STORE
          .word BSCR
          .word STAR
          .word BLK
          .word STORE
          .word INTER
          .word RFROM
          .word IN
          .word STORE
          .word RFROM
          .word BLK
          .word STORE
          .word SEMIS
;
;                                       -->
;                                       SCREEN 62 LINE 6
;
L2924     .TEXT $C3,'--',$BE
          .word L2896    ; link to LOAD
          .word DOCOL
          .word QLOAD
          .word ZERO
          .word IN
          .word STORE
          .word BSCR
          .word BLK
          .word AT
          .word OVER
          .word MOD
          .word SUB
          .word BLK
          .word PSTOR
          .word SEMIS
;
;    XEMIT writes one ascii character to terminal
;
;
XEMIT     TYA
          SEC
          LDY #$1A
          ADC (UP),Y
          STA (UP),Y
          INY            ; bump user varaible OUT
          LDA #0
          ADC (UP),Y
          STA (UP),Y
          LDA 0,X        ; fetch character to output
          STX XSAVE
          JSR OUTCH      ; and display it
          LDX XSAVE
          JMP POP
;
;         XKEY reads one terminal keystroke to stack
;
;
XKEY      STX XSAVE
          JSR INCH       ; might otherwise clobber it while
          LDX XSAVE      ; inputting a char to accumulator
          JMP PUSHOA
;
;         XQTER leaves a boolean representing terminal break
;
;
XQTER     LDA $C000      ; system depend port test
          CMP $C001
          AND #1
          JMP PUSHOA
;
;         XCR displays a CR and LF to terminal
;
;
XCR       STX XSAVE
          JSR TCR        ; use monitor call
          LDX XSAVE
          JMP NEXT
;
;                                       -DISC
;                                       machine level sector R/W
;
L3030     .TEXT $85,'-DIS',$C3
          .word L2924    ; link to -->
DDISC     .word *+2
          LDA 0,X
          STA $C60C
          STA $C60D      ; store sector number
          LDA 2,X
          STA $C60A
          STA $C60B      ; store track number
          LDA 4,X
          STA $C4CD
          STA $C4CE      ; store drive number
          STX XSAVE
          LDA $C4DA      ; sense read or write
          BNE L3032
          JSR $E1FE
          JMP L3040
L3032     JSR $E262
L3040     JSR $E3EF      ; head up motor off
          LDX XSAVE
          LDA $C4E1      ; report error code
          STA 4,X
          JMP POPTWO
;
;                                       -BCD
;                             Convert binary value to BCD
;
L3050     .TEXT $84,'-BC',$C4
          .word L3030    ; link to -DISC
DBCD      .word DOCOL
          .word ZERO,CLIT
          .TEXT 10
          .word USLAS,CLIT
          .TEXT 16
          .word STAR,OR,SEMIS
;
;                                       R/W
;                              Read or write one sector
;
L3060     .TEXT $83,'R/',$D7
          .word L3050    ; link to -BCD
RSLW      .word DOCOL
          .word ZEQU,LIT,$C4DA,CSTOR
          .word SWAP,ZERO,STORE
          .word ZERO,OVER,GREAT,OVER
          .word LIT,SECTL-1,GREAT,OR,CLIT
          .TEXT 6
          .word QERR
          .word ZERO,LIT,SECTR,USLAS,ONEP
          .word SWAP,ZERO,CLIT
          .TEXT $12
          .word USLAS,DBCD,SWAP,ONEP
          .word DBCD,DDISC,CLIT
          .TEXT 8
          .word QERR
          .word SEMIS
;
;
;
          .word SEMIS
;
;                                       '
;                                       SCREEN 72 LINE 2
;
L3202     .TEXT $C1,$A7
          .word L3060    ; link to R/W
TICK      .word DOCOL
          .word DFIND
          .word ZEQU
          .word ZERO
          .word QERR
          .word DROP
          .word LITER
          .word SEMIS
;
;                                       FORGET
;                                       Altered from model
;                                       SCREEN 72 LINE 6
;
L3217     .TEXT $86,'FORGE',$D4
          .word L3202    ; link to ' TICK
FORG      .word DOCOL
          .word TICK,NFA,DUP
          .word FENCE,AT,ULESS,CLIT
          .TEXT $15
          .word QERR,TOR,VOCL,AT
L3220     .word R,OVER,ULESS
          .word ZBRAN,L3225-*
          .word FORTH,DEFIN,AT,DUP
          .word VOCL,STORE
          .word BRAN,$FFFF-24+1 ; L3220-*
L3225     .word DUP,CLIT
          .TEXT 4
          .word SUB
L3228     .word PFA,LFA,AT
          .word DUP
          .word R
          .word ULESS
          .word ZBRAN,$FFFF-14+1 ; L3228-*
          .word OVER,TWO,SUB,STORE
          .word AT,DDUP,ZEQU
          .word ZBRAN,$FFFF-39+1 ; L3225-*
          .word RFROM,DP,STORE
          .word SEMIS
;
;                                       BACK
;                                       SCREEN 73 LINE 1
;
L3250     .TEXT $84,'BAC',$CB
          .word L3217    ; link to FORGET
BACK      .word DOCOL
          .word HERE
          .word SUB
          .word COMMA
          .word SEMIS
;
;                                       BEGIN
;                                       SCREEN 73 LINE 3
;
L3261     .TEXT $C5,'BEGI',$CE
          .word L3250    ; link to BACK
          .word DOCOL
          .word QCOMP
          .word HERE
          .word ONE
          .word SEMIS
;
;                                       ENDIF
;                                       SCREEN 73 LINE 5
;
L3273     .TEXT $C5,'ENDI',$C6
          .word L3261    ; link to BEGIN
ENDIF     .word DOCOL
          .word QCOMP
          .word TWO
          .word QPAIR
          .word HERE
          .word OVER
          .word SUB
          .word SWAP
          .word STORE
          .word SEMIS
;
;                                       THEN
;                                       SCREEN 73 LINE 7
;
L3290     .TEXT $C4,'THE',$CE
          .word L3273    ; link to ENDIF
          .word DOCOL
          .word ENDIF
          .word SEMIS
;
;                                       DO
;                                       SCREEN 73 LINE 9
;
L3300     .TEXT $C2,'D',$CF
          .word L3290    ; link to THEN
          .word DOCOL
          .word COMP
          .word PDO
          .word HERE
          .word THREE
          .word SEMIS
;
;                                       LOOP
;                                       SCREEN 73 LINE 11
;
;
L3313     .TEXT $C4,'LOO',$D0
          .word L3300    ; link to DO
          .word DOCOL
          .word THREE
          .word QPAIR
          .word COMP
          .word PLOOP
          .word BACK
          .word SEMIS
;
;                                       +LOOP
;                                       SCREEN 73 LINE 13
;
L3327     .TEXT $C5,'+LOO',$D0
          .word L3313    ; link to LOOP
          .word DOCOL
          .word THREE
          .word QPAIR
          .word COMP
          .word PPLOO
          .word BACK
          .word SEMIS
;
;                                       UNTIL
;                                       SCREEN 73 LINE 15
;
L3341     .TEXT $C5,'UNTI',$CC
          .word L3327    ; link to +LOOP
UNTIL     .word DOCOL
          .word ONE
          .word QPAIR
          .word COMP
          .word ZBRAN
          .word BACK
          .word SEMIS
;
;                                       END
;                                       SCREEN 74 LINE 1
;
L3355     .TEXT $C3,'EN',$C4
          .word L3341    ; link to UNTIL
          .word DOCOL
          .word UNTIL
          .word SEMIS
;
;                                       AGAIN
;                                       SCREEN 74 LINE 3
;
L3365     .TEXT $C5,'AGAI',$CE
          .word L3355    ; link to END
AGAIN     .word DOCOL
          .word ONE
          .word QPAIR
          .word COMP
          .word BRAN
          .word BACK
          .word SEMIS
;
;                                       REPEAT
;                                       SCREEN 74 LINE 5
;
L3379     .TEXT $C6,'REPEA',$D4
          .word L3365    ; link to AGAIN
          .word DOCOL
          .word TOR
          .word TOR
          .word AGAIN
          .word RFROM
          .word RFROM
          .word TWO
          .word SUB
          .word ENDIF
          .word SEMIS
;
;                                       IF
;                                       SCREEN 74 LINE 8
;
L3396     .TEXT $C2,'I',$C6
          .word L3379    ; link to REPEAT
IF        .word DOCOL
          .word COMP
          .word ZBRAN
          .word HERE
          .word ZERO
          .word COMMA
          .word TWO
          .word SEMIS
;
;                                       ELSE
;                                       SCREEN 74 LINE 10
;
L3411     .TEXT $C4,'ELS',$C5
          .word L3396    ; link to IF
          .word DOCOL
          .word TWO
          .word QPAIR
          .word COMP
          .word BRAN
          .word HERE
          .word ZERO
          .word COMMA
          .word SWAP
          .word TWO
          .word ENDIF
          .word TWO
          .word SEMIS
;
;                                       WHILE
;                                       SCREEN 74 LINE 13
;
L3431     .TEXT $C5,'WHIL',$C5
          .word L3411    ; link to ELSE
          .word DOCOL
          .word IF
          .word TWOP
          .word SEMIS
;
;                                       SPACES
;                                       SCREEN 75 LINE 1
;
L3442     .TEXT $86,'SPACE',$D3
          .word L3431    ; link to WHILE
SPACS     .word DOCOL
          .word ZERO
          .word MAX
          .word DDUP
          .word ZBRAN
L3449     .word $0C      ; L3455-L3449
          .word ZERO
          .word PDO
L3452     .word SPACE
          .word PLOOP
L3454     .word $FFFC    ; L3452-L3454
L3455     .word SEMIS
;
;                                       <#
;                                       SCREEN 75 LINE 3
;
L3460     .TEXT $82,'<',$A3
          .word L3442    ; link to SPACES
BDIGS     .word DOCOL
          .word PAD
          .word HLD
          .word STORE
          .word SEMIS
;
;                                       #>
;                                       SCREEN 75 LINE 5
;
L3471     .TEXT $82,'#',$BE
          .word L3460    ; link to <#
EDIGS     .word DOCOL
          .word DROP
          .word DROP
          .word HLD
          .word AT
          .word PAD
          .word OVER
          .word SUB
          .word SEMIS
;
;                                       SIGN_W
;                                       SCREEN 75 LINE 7
;
L3486     .TEXT $84,'SIG',$CE
          .word L3471    ; link to #>
SIGN_W      .word DOCOL
          .word ROT
          .word ZLESS
          .word ZBRAN
L3492     .word $7       ; L3496-L3492
          .word CLIT
          .TEXT $2D
          .word HOLD
L3496     .word SEMIS
;
;                                       #
;                                       SCREEN 75 LINE 9
;
L3501     .TEXT $81,$A3
          .word L3486    ; link to SIGN_W
DIG       .word DOCOL
          .word BASE
          .word AT
          .word MSMOD
          .word ROT
          .word CLIT
          .TEXT 9
          .word OVER
          .word LESS
          .word ZBRAN
L3513     .word 7        ; L3517-L3513
          .word CLIT
          .TEXT 7
          .word PLUS
L3517     .word CLIT
          .TEXT $30
          .word PLUS
          .word HOLD
          .word SEMIS
;
;                                       #S
;                                       SCREEN 75 LINE 12
;
L3526     .TEXT $82,'#',$D3
          .word L3501    ; link to #
DIGS      .word DOCOL
L3529     .word DIG
          .word OVER
          .word OVER
          .word OR
          .word ZEQU
          .word ZBRAN
L3535     .word $FFF4    ; L3529-L3535
          .word SEMIS
;
;                                       D.R
;                                       SCREEN 76 LINE 1
;
L3541     .TEXT $83,'D.',$D2
          .word L3526    ; link to #S
DDOTR     .word DOCOL
          .word TOR
          .word SWAP
          .word OVER
          .word DABS_W
          .word BDIGS
          .word DIGS
          .word SIGN_W
          .word EDIGS
          .word RFROM
          .word OVER
          .word SUB
          .word SPACS
          .word TYPE_W
          .word SEMIS
;
;                                       D.
;                                       SCREEN 76 LINE 5
;
L3562     .TEXT $82,'D',$AE
          .word L3541    ; link to D.R
DDOT      .word DOCOL
          .word ZERO
          .word DDOTR
          .word SPACE
          .word SEMIS
;
;                                       .R
;                                       SCREEN 76 LINE 7
;
L3573     .TEXT $82,'.',$D2
          .word L3562     ; link to D.
DOTR      .word DOCOL
          .word TOR
          .word STOD
          .word RFROM
          .word DDOTR
          .word SEMIS
;
;                                       .
;                                       SCREEN 76  LINE  9
;
L3585     .TEXT $81,$AE
          .word L3573    ; link to .R
DOT       .word DOCOL
          .word STOD
          .word DDOT
          .word SEMIS
;
;                                       ?
;                                       SCREEN 76 LINE 11
;
L3595     .TEXT $81,$BF
          .word L3585    ; link to .
QUES      .word DOCOL
          .word AT
          .word DOT
          .word SEMIS
;
;                                       LIST_W
;                                       SCREEN 77 LINE 2
;
L3605     .TEXT $84,'LIS',$D4
          .word L3595    ; link to ?
LIST_W      .word DOCOL
          .word DECIM
          .word CR
          .word DUP
          .word SCR
          .word STORE
          .word PDOTQ
          .TEXT 6,'SCR # '
          .word DOT
          .word CLIT
          .TEXT 16
          .word ZERO
          .word PDO
L3620     .word CR
          .word I
          .word THREE
          .word DOTR
          .word SPACE
          .word I
          .word SCR
          .word AT
          .word DLINE
          .word PLOOP
L3630     .word $FFEC
          .word CR
          .word SEMIS
;
;                                       INDEX
;                                       SCREEN 77 LINE 7
;
L3637     .TEXT $85,'INDE',$D8
          .word L3605    ; link to LIST_W
          .word DOCOL
          .word CR
          .word ONEP
          .word SWAP
          .word PDO
L3647     .word CR
          .word I
          .word THREE
          .word DOTR
          .word SPACE
          .word ZERO
          .word I
          .word DLINE
          .word QTERM
          .word ZBRAN
L3657     .word 4        ; L3659-L3657
          .word LEAVE
L3659     .word PLOOP
L3660     .word $FFE6    ; L3647-L3660
          .word CLIT
          .TEXT $0C      ; form feed for printer
          .word EMIT
          .word SEMIS
;
;                                       TRIAD
;                                       SCREEN 77 LINE 12
;
L3666     .TEXT $85,'TRIA',$C4
          .word L3637    ; link to INDEX
          .word DOCOL
          .word THREE
          .word SLASH
          .word THREE
          .word STAR
          .word THREE
          .word OVER
          .word PLUS
          .word SWAP
          .word PDO
L3681     .word CR
          .word I
          .word LIST_W
          .word PLOOP
L3685     .word $FFF8    ; L3681-L3685
          .word CR
          .word CLIT
          .TEXT $F
          .word MESS
          .word CR
          .word CLIT
          .TEXT $0C      ;  form feed for printer
          .word EMIT
          .word SEMIS
;
;                                       VLIST_W
;                                       SCREEN 78 LINE 2
;
;
L3696     .TEXT $85,'VLIS',$D4
          .word L3666    ; link to TRIAD
VLIST_W     .word DOCOL
          .word CLIT
          .TEXT $80
          .word OUT
          .word STORE
          .word CON
          .word AT
          .word AT
L3706     .word OUT
          .word AT
          .word CSLL
          .word GREAT
          .word ZBRAN
L3711     .word $A       ; L3716-L3711
          .word CR
          .word ZERO
          .word OUT
          .word STORE
L3716     .word DUP
          .word IDDOT
          .word SPACE
          .word SPACE
          .word PFA
          .word LFA
          .word AT
          .word DUP
          .word ZEQU
          .word QTERM
          .word OR
          .word ZBRAN
L3728     .word $FFD4    ; L3706-L3728
          .word DROP
          .word SEMIS
;
;                                       MON
;                                       SCREEN 79 LINE 3
;
NTOP      .TEXT $83,'MO',$CE
          .word L3696    ; link to VLIST_W
MON       .word *+2
          STX XSAVE
          BRK       ; break to monitor which is assumed
          LDX XSAVE ; to save this as reentry point
          JMP NEXT
;
TOP       .END           ; end of listing
