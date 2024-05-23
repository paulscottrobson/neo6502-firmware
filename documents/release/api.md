---
fontfamilyoptions: sfdefault
fontsize: 12pt
---

# Neo6502 Messaging API

The Neo6502 API is a messaging system. There are no methods to access
the hardware directly. Messages are passed via the block of memory from
$FF00 to $FF0F, as specified in the \"API Messaging Addresses\"
table on the following page.

The kernel include file documents/release/neo6502.inc specifies the
beginning of this address range as the identifier ControlPort, along
with the addresses of WaitMessage and SendMessage (described later),
various related kernel jump vectors, and some helper functions.

The application include files examples/assembly/neo6502.asm.inc and
examples/C/neo6502.h also specify the beginning of this address range
as the identifier ControlPort. The assembly include also specifies
ControlPort and the other controls as API_COMMAND, API_FUNCTION,
API_ERROR, API_STATUS, and API_PARAMETERS. The C header also
specifies ControlPort and the other controls as API_COMMAND_ADDR,
API_FUNCTION_ADDR, API_ERROR_ADDR, API_STATUS_ADDR, and
API_PARAMETERS_ADDR.

API Commands/Functions are grouped by functionality. For example, Group
1 are system-related, and Group 2 are console-I/O-related. 

Command/Function Parameters are notated in this document as Params[0]
through Params[7], or as a list or range (eg: Params[1,2],
Params[0..7]). Note that these are referring to a mapping to memory
locations. The numbers represent offsets from the Parameters base
address $FF04. Ie: the actual bytes are not necessarily all distinct
\"parameters\" in the conventional sense. Depending on the routine, a
logical parameter may be an individual byte, one or more bits of a byte
interpreted as a composite or bit-field, or multiple adjacent bytes
interpreted as 16 or 32 bit values. For example, the list Params[0,1]
would indicate a single logical parameter, comprised of the two adjacent
bytes $FF04 and $FF05. The range Params[4..7] would indicate a
single logical parameter, spanning consecutive bytes between $FF08 and
$FF0B.

**Note that strings referenced by Parameters are not ASCIIZ, but are
length-prefixed.** The first byte represents the length of the string
(not counting itself). The string begins at the second byte.
Consequently, strings must be 256 bytes or less (not counting the length
header).

\newpage

**API Messaging Addresses**\

| Address | Type       | Notes                                                        |
| ------- | ---------- | ------------------------------------------------------------ |
| FF00    | Group      | Group selector and status. Writing a non-zero value to this location triggers the routine specified in $FF01. The system will respond by setting the 'Error', 'Information', and 'Parameters' values appropriately. Upon completion, this memory location will be will cleared. |
| FF01    | Function   | A command or function within the selected Group. For example, Group 1 Function 0 writes a value to the console; and Group 1 Function 1 reads the keyboard. |
| FF02    | Error      | Return any error values, 0 = no error. \                     |
| FF03:7  | Status     | Set (1) if the ESCape key has been pressed. This is not automatically reset. |
| FF03:6  | *unused*   |                                                              |
| FF03:5  | *unused*   |                                                              |
| FF03:4  | *unused*   |                                                              |
| FF03:3  | *unused*   |                                                              |
| FF03:2  | *unused*   |                                                              |
| FF03:1  | *unused*   |                                                              |
| FF03:0  | *unused*   |                                                              |
| FF04..  | Parameters | This memory block is notated in this document as Params[0] through Params[7], or as a composite list or range (eg: Params[1,2], Params[0..7]). Some Functions require Parameters in these locations and some return values in these locations; yet others do neither. |


\newpage

## API Interfacing Protocol

Neo6502 application programmers should interact with the API per the
following algorithm:

1.  Wait for any pending command to complete. There is a subroutine
    WaitMessage which does this for the developer.

2.  Setup the Function code at $FF01; and any Parameters across
    $FF04..$FF0B. To print a character for example, set $FF01 to
    $06 and set $FF04 to the character's ASCII value. To draw a
    line, set $FF01 to $02 and set $FF04..$FF0B as four 16-bit
    pixel coordinates:

    **Params**

3.  Setup the command code at $FF00. This triggers the routine; so
    mind that the Function code and Parameters are setup sanely prior.
    On a technical point, both implementations process the message
    immediately on write.

4.  Optionally, wait for completion. Most commands (eg: writing to the
    console) do not require waiting, as any subsequent command will
    wait/queue as per step 1. Query commands (e.g. reading from the
    keyboard queue), return a value in a parameter. Programs must wait
    until the control address $FF00 has been cleared before reading
    the result of a query.

\newpage

There is a support function SendMessage, which in-lines the command
and function. E.g.: this code from the Kernel:

jsr KSendMessage ; send message for command 2,1 (read keyboard)

.byte 2,1

jsr KWaitMessage ; wait to receive the result message

lda DParameters ; read result

The instructions above are equivalent to the following explicit steps:

lda #1

sta DFunction

lda #2

sta DCommand ; trigger API function 2,1 (read keyboard)

Loop:

lda DCommand ; load the result - non-zero until the routine\'s
completion

bne Loop ; wait for API routine to complete

lda DParameters ; read result (a key-code)

\newpage

## Mathematical Interface

The mathematical interface of the API functions largely as a helper
system for the BASIC interpreted, but it is open to any developer who
wishes to avail themselves of the functionality. It is strongly advised
that function 32 of Group 4 (NumberToDecimal) is not used as this is
extremely specific and as such is not documented.

The interface is used in a stack environment, but is designed so it
could be used in either a stack environment or a fixed location
environment. The Neo6502 BASIC stack is also 'split', so elements are
not consecutive, though they can be.

Parameter 0 and 1 specify the address of the registers 1 and 2. Register
1 starts at this address, Register 2 starts at the next address.
Parameter 2 specifies the step to the next register. Therefore they are
interleaved by default at present.

So if Parameters 0 and 1 are 8100 and Parameter 2 is 4, the 5 byte
registers are

Register 1: 8100,8104,8108,810C,8110

Register 2: 8101,8105,8109,810D,8111

Bytes 1-4 of the 'register' are the number, which can be either an
integer (32 bit signed) or a standard 'C' float (e.g. the IEEE Single
Precision Float format). Bit 0 is the type byte, and the relevant bit is
bit 6, which is set to indicate bytes 1-4 are a float value, and is set
on return.

Binary functions that use int and float combined (one is int and one is
float) normally return a float.

\newpage

# Console Codes

The following are console key codes. They can be printed in BASIC
programs using chr$(n), and are also related to the character keys
returned by inkey$(). The key() function uses physical key numbers.
Some control codes do not have corresponding keyboard keys; and some
console outputs are not yet implemented.

Backspace (8), Tab (9), Enter/CR (13), Escape (27), and the
printable characters (32..127) are the standard ASCII set. Other typical
control keys (eg: Home and arrows) are mapped into the 0..31 range.

**Console Key Codes - Non-Printable**\

| Code | Ctrl | Key         | Function                      |
| ---- | ---- | ----------- | ----------------------------- |
| 1    | A    | Left Arrow  | Cursor Left                   |
| 4    | D    | Right Arrow | Cursor Right                  |
| 5    | E    | Insert      | Insertion Mode                |
| 6    | F    | Page Down   | Cursor Page Down              |
| 7    | G    | End         | Cursor Line End               |
| 8    | H    | Backspace   | Delete Character Left         |
| 9    | I    | Tab         | Tab Character                 |
| 10   | J    |             | Line Feed                     |
| 12   | L    |             | Clear Screen                  |
| 13   | M    | Enter       | Carriage Return (Accept Line) |
| 18   | R    | Page Up     | Cursor Page Up                |
| 19   | S    | Down        | Cursor Down                   |
| 20   | T    | Home        | Cursor Line Begin             |
| 22   | V    |             | Cursor Down (8 Lines)         |
| 23   | W    | Up          | Cursor Up                     |
| 24   | X    |             | Cursor Color Inverse          |
| 26   | Z    | Delete      | Delete Character Right        |
| 27   | \[   | Escape      | Exit                          |

**Console Key Codes - Printable**\

| Code  | Type      | Notes                     |
| ----- | --------- | ------------------------- |
| 20-7F | ASCII Set | Standard ASCII Characters |
| 80-8F |           | Set Foreground Color      |
| 90-9F |           | Set Background Color      |
| C0-FF |           | User-definable Characters |

\newpage

# Graphics

## Graphics Settings

Function 5,1 configures the global graphics system settings. Not all
Parameters are relevant for all graphics commands; but all Parameters
will be set by this command. So mind their values.

The actual color of each drawn pixel will be computed at runtime by
AND'ing the existing pixel color with the value specified in
Params[0], then XOR'ing the result with the value specified in
Params[1].

The value in Params[2] is a flag which determines the paint fill mode
for the Draw Rectangle and Draw Ellipse commands: reset (0) for
outline, set (1) for solid fill.

The value in Params[3] is the draw extent (window) for the Draw Image
command.

The value in Params[4] is a bit-field of flags for the Draw Image
command, which determine if the image will be inverted (flipped)
horizontally or vertically: bit-0 for horizontal, bit-1 for
vertical, reset (0) for normal, set (1) for inverted.

For the \"Draw Rectangle\" and \"Draw Ellipse\" commands, the given
order and position of the coordinates are not significant. To be
precise, one is \"a corner\" and the other is \"the opposite corner\".
For the \"Draw Ellipse\" command, these corners are referring to the
bounding-box. The coordinates for an ellipse will lie outside of the
ellipse itself.

\newpage

## Graphics Data

Graphics data files are conventionally named ending in the .gfx suffix;
though this is not mandatory. The format is quite simple.

**Graphics Data Format**\

| Offset | Data  | Notes                          |
| ------ | ----- | ------------------------------ |
| 0      | 1     | Graphics Data Format ID        |
| 1      | Count | Number of 16x16 tiles in use   |
| 2      | Count | Number of 16x16 sprites in use |
| 3      | Count | Number of 32x32 sprites in use |
| 4..255 |       | Reserved                       |
| 256    | Raw   | Sprites graphics data          |

The layout of sprites graphics data is all of the 16x16 tiles, followed
by all the 16x16 sprites, followed by all the 32x32 sprites, all in
their respective orders. As there is currently only about 20kB of
Graphics Memory, these should be used somewhat sparingly.

Each byte specifies 2 pixels. The upper 4 bits represent the first pixel
colour; and the lower 4 bits represent the second pixel colour. So tiles
and 16x16 sprites occupy 16x16/2 bytes (128 bytes) each. Each 32x32
sprite occupies 32x32/2 bytes (512 bytes). Colour 0 is transparent for
sprites (colour 9 should be used for a black pixel).

The release package includes Python scripts for creating graphics files,
which allow you to design graphics using your preferred editing tools
(eg: Gimp, Inkscape, Krita, etc). There is an example in the crossdev/
directory, which demonstrates how to get started importing graphics into
the Neo6502.

\newpage

## Pixel Colors

**Pixel Colors**\

| Pixel | Colour            |
| ----- | ----------------- |
| $80   | Black/Transparent |
| $81   | Red               |
| $82   | Green             |
| $83   | Yellow            |
| $84   | Blue              |
| $85   | Magenta           |
| $86   | Cyan              |
| $87   | White             |
| $88   | Black             |
| $89   | Dark Grey         |
| $8A   | Dark Green        |
| $8B   | Orange            |
| $8C   | Dark Orange       |
| $8D   | Brown             |
| $8E   | Pink              |
| $8F   | Light Grey        |


\newpage

# Tile Maps

A tile map occupies an area of user memory in 65C02. It is comprised of
three meta-data bytes, followed by one byte for each tile, which is it's
tile number in the graphic file (refer to the following section).

F0-FF are special reserved tile numbers, F0 is a transparent tile;
and F1-FF are a solid tile in the current palette colour. The format
is very simple.

**Tile Maps Format**\

| Offset | Data   | Notes                                       |
| ------ | ------ | ------------------------------------------- |
| 0      | 1      | Graphics Data Format ID                     |
| 1      | Width  | Width of tile-map (number of tiles)         |
| 2      | Height | Height of tile-map (number of tiles)        |
| 3..    | Raw    | Tiles graphics data (width \* height bytes) |

\newpage

# Sprites

The Neo6502 graphics system has one sprite layer (z-plane) in the
conventional sense. Technically, there is no \"sprite layer\", per-se.
The system uses palette manipulation to create, what is in practice, a
pair of 4-bit bit-planes. The sprite graphics are in the upper nibble,
the background is in the lower nibble, and the background is drawn only
if the sprite graphic layer is zero. It's this top nibble which is read
by Function 5,36 \"Read Sprite Pixel\".

Function 6,2 sets or updates a sprite. These parameters (eg: the X and Y
coordinates) cannot be set independently. To retain/reuse the current
value of a parameter for a subsequent call, set each of the associated
byte(s) to $80 (eg: $80,$80,$80,$80 for coordinates).

The 'Sprite' Parameter Params[0] specifies the index of the sprite in
the graphics system. Sprite 0 is the turtle sprite.

Params[1,2],Params[3,4] specifies the X and Y screen coordinates.

Bits 0-5 of the 'Image' Parameter Params[5] specify the index of a
specific graphic within the sprites data. Bit 6 of the 'Image' Parameter
specifies the sprite dimensions: reset (0) for 16x16, set (1) for
32x32. In practice, the index is the same as the sprite number
($80-$BF for 16x16 sprites, $C0-$FF for 32x32 sprites), but
without bit-7 set.

The value in Params[6] specifies a bit-field of flags, which
determines if the graphic will be inverted (flipped) horizontally or
vertically: bit-0 for horizontal, bit-1 for vertical, reset (0)
for normal, set (1) for inverted.

Params[7] specifies the anchor alignment. 

\newpage

## Sprite Anchors

The table below shows the valid anchor alignments for a sprite. The
anchor position is the origin of the relative coordinate given. That is,
coordinates 0,0 of the sprite will coincide with one of the positions
shown in the table below. The default anchor alignment is zero
(middle-center).

**Sprite Anchors**\

|      |      |      |
| ---- | ---- | ---- |
| 7    | 8    | 9    |
|      |      |      |
|      |      |      |
| 4    | 0/5  | 6    |
|      |      |      |
|      |      |      |
| 1    | 2    | 3    |

To the right are two examples. Assume this is a 32x32 sprite. In the
upper example, the anchor point is at 8, the top-center. Considering the
origin at the bottom-left, this sprite is drawn at 16,32, the midpoint
of the top of the square.

In the lower example, the anchor point is at 0; and this sprite is drawn
at 16,16 (the middle of the square). The anchor point should be
something like the centre point. So for a walking character, this might
be anchor point 2 (the bottom-center).

\newpage

# Sound

Function 8,4 queues a sound. Queued sounds are played sequentially, each
after the previous has completed, such that sounds within a channel
queue will not conflict, interrupt, or overlap.

Frequency is in units of Hertz. Duration is in units of 100ths of a
second. Slide is a gradual linear change in frequency, in units of Hz
per 100th of a second. Sound target type 0 is the beeper. Currently, the
beeper is the only available sound target.

**Queue Sound Parameters**

|         |          |           |            |             |           |            |        |
| ------- | -------- | --------- | ---------- | ----------- | --------- | ---------- | ------ |
| FF04    | FF05     | FF06      | FF07       | FF08        | FF09      | FF0A       | FF0B   |
| Channel | Freq Low | Freq High | Length Low | Length High | Slide Low | Slide High | Target |

\newpage

# Sound Effects

Function 8,5 plays a sound effect immediately. These will be synthesised
to the best ability of the available hardware. These are predefined as:

| ID   | Sound      |
| ---- | ---------- |
| 0    | positive   |
| 1    | negative   |
| 2    | error      |
| 3    | confirm    |
| 4    | reject     |
| 5    | sweep      |
| 6    | coin       |
| 7    | las70      |
| 8    | powerup    |
| 9    | victory    |
| 10   | defeat     |
| 11   | fanfare    |
| 12   | alarm 1    |
| 13   | alarm 2    |
| 14   | alarm 3    |
| 15   | ringtone 1 |
| 16   | ringtone 2 |
| 17   | ringtone 3 |
| 18   | danger     |
| 19   | expl100    |
| 20   | expl50     |
| 21   | expl20     |
| 22   | las30      |
| 23   | las10      |


\newpage

# API Functions

The following tables are a comprehensive list of all supported API
functions.

For the convenience of application programmers, the application include
files examples/C/neo6502.h and examples/assembly/neo6502.asm.inc
define macros for these groups, their functions, and common parameters
(colors, musical notes, etc).
