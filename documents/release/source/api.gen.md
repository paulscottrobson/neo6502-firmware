---

fontfamilyoptions: sfdefault

fontsize: 12pt

---



# Group 1 : System

## Function 0 : DSP Reset

Resets the messaging system and component systems.
Normally, should not be used.

## Function 1 : Timer

Deposit the value (32-bits) of the 100Hz system timer into Parameters:0..3.

## Function 2 : Key Status

Deposits the state of the specified keyboard key into Parameter:0.
State of keyboard modifiers (Shift/Ctrl/Alt/Meta) is returned in Parameter:1.
The key which to query is specified in Parameter:0.

## Function 3 : Basic

Loads and allows the execution of BASIC via a indirect jump through address zero.

## Function 4 : Credits

Print the Neo6502 project contributors (stored in flash memory).

## Function 5 : Serial Status

Check the serial port to see if there is a data transmission.

## Function 6 : Locale

Set the locale code specified in Parameters:0,1 as upper-case ASCII letters.
Parameter:0 takes the first letter and Parameter:1 takes the second letter.
For example: French (FR) would require Parametr 0 being $46 and Parameter 1 being $52

## Function 7 : System Reset

System Reset. This is a full hardware reset. It resets the RP2040 using the Watchdog timer, and
this also resets the 65C02.

## Function 8 : MOS

Do a MOS command (a '* command') these are specified in the Wiki as they will be steadily expanded.

## Function 9 : Sweet 16 Virtual Machine

Execute Sweet 16 VM Code with the register set provided. The error return value is actually a re-entrant
call for Windows emulation ; if it returns non-zero it means the VM has been executed, if it returns zero
then the emulation can update as at the end of a normal frame.

\newpage

# Group 2 : Console

## Function 0 : Write Character

Console out (duplicate of Function 6 for backward compatibility).

## Function 1 : Read Character

Read and remove a key press from the keyboard queue into Parameter:0.
This is the ASCII value of the keystroke.
If there are no key presses in the queue, Parameter:0 will be zero.
Note that this Function is best for text input, but not for games.
Function 7,1 is more optimal for games, as this only detects key presses,
you cannot check whether the key is currently down or not.

## Function 2 : Console Status

Check to see if the keyboard queue is empty.
If it is, Parameter:0 will be $FF, otherwise it will be $00

## Function 3 : Read Line

Input the current line below the cursor into Parameters:0,1 as a length-prefixed string;
and move the cursor to the line below. Handles multiple-line input.

## Function 4 : Define Hotkey

Define the function key F1..F10 specified in Parameter:0 as 1..10 to emit the
length-prefixed string stored at the memory location specified in Parameters:2,3.
F11 and F12 cannot currently be defined.

## Function 5 : Define Character

Define a font character specified in Parameter:0 within the range of 192..255.
Fill bits 0..5 (columns) of Parameters:1..7 (rows) with the character bitmap.

## Function 6 : Write Character

Write the character specified in Parameter:0 to the console at the cursor position.
Refer to Section "Console Codes" for details.

## Function 7 : Set Cursor Pos

Move the cursor to the screen character cell Parameter:0 (X), Parameter:1 (Y).

## Function 8 : List Hotkeys

Display the current function key definitions.

## Function 9 : Screen Size

Returns the console size in characters, in Parameter:0 (height) and Parameter:1 (width).

## Function 10 : Insert Line

This is a support function which inserts a blank line in the console and should not be used.

## Function 11 : Delete Line

This is a support function which deletes a line in the console and should not be used.

## Function 12 : Clear Screen

Clears the screen.

## Function 13 : Cursor Position

Returns the current screen character cell of the cursor
in Parameter:0 (X), Parameter:1 (Y).

## Function 14 : Clear Region

Erase all characters within the rectangular region specified
in Parameters:0,1 (begin X,Y) and Parameters:2,3 (end X,Y).

## Function 15 : Set Text Color

Sets the foreground colour to Parameter:0 and the background colour to Parameter:1.

## Function 16 : Cursor Inverse

Toggles the cursor colour between normal and inverse
(ie: swaps FG and BG colors). This should not be used.

## Function 17 : Tab() implementation

Internal helper.

\newpage

# Group 3 : File I/O

## Function 1 : List Directory

Display the file listing of the present directory.

## Function 2 : Load File

Load a file by name into memory. On input:
Parameters:0,1 points to the length-prefixed filename string;
Parameters:2,3 contains the location to write the data to. If the
address is $FFFF, the file will instead be loaded into the
graphics working memory, used for sprites, tiles, images.
On output:
Error location contains an error/status code.

## Function 3 : Store File

Saves data in memory to a file. On input:
Parameters:0,1 points to the length-prefixed filename string;
Parameters:2,3 contains the location to read data from;
Parameters:4,5 specified the number of bytes to store.
On output:
Error location contains an error/status code.

## Function 4 : File Open

Opens a file into a specific channel. On input:
Parameter:0 contains the file channel to open;
Parameters:1,2 points to the length-prefixed filename string;
Parameter:3 contains the open mode. See below.
Valid open modes are:
0 opens the file for read-only access;
1 opens the file for write-only access;
2 opens the file for read-write access;
3 creates the file if it doesn't already exist, truncates it if it
does, and opens the file for read-write access.
Modes 0 to 2 will fail if the file does not already exist. If the
channel is already open, the call fails. Opening the same file more than
once on different channels has undefined behaviour, and is not
recommended.

## Function 5 : File Close

Closes a particular channel. On input:
Parameter:0 contains the file channel to close.

## Function 6 : File Seek

Seeks the file opened on a particular channel to a location. On input:
Parameter:0 contains the file channel to operate on;
Parameters:1..4 contains the file location.
You can seek beyond the end of a file to extend the file.
However, whether the file size changes when the seek happens,
or when you perform the write is undefined behavior.

## Function 7 : File Tell

Returns the current seek location for the file opened on a particular channel. On input:
Parameter:0 contains the file channel to operate on.
On output:
Parameters:1..4 contains the seek location within the file.

## Function 8 : File Read

Reads data from an opened file. On input:
Parameter:0 contains the file channel to operate on.
Parameters:1,2 points to the destination in memory,
or $FFFF to read into graphics memory.
Parameters:2,3 contains the amount of data to read.
On output:
Parameters:3,4 is updated to contain the amount of data actually read.
Data is read from the current seek position, which is advanced after the read.

## Function 9 : File Write

Writes data to an opened file. On input:
Parameter:0 contains the file channel to operate on;
Parameters:1,2 points to the data in memory;
Parameters:3,4 contains the amount of data to write.
On output:
Parameters:3,4 is updated to contain the amount of data actually written.
Data is written to the current seek position, which is advanced after the write.

## Function 10 : File Size

Returns the current size of an opened file. On input:
Parameter:0 contains the file channel to operate on.
On output:
Parameters:1..4 contains the size of the file.
This call should be used on open files,
and takes into account any buffered data which has not yet been written to disk.
Consequently, this may return a different size than Function 3,16 "File Stat".

## Function 11 : File Set Size

Extends or truncates an opened file to a particular size. On input:
Parameter:0 contains the file channel to operate on;
Parameters:1..4 contains the new size of the file.

## Function 12 : File Rename

Renames a file. On input:
Parameters:0,1 points to the length-prefixed string for the old name;
Parameters:2,3 points to the length-prefixed string for the new name.
Files may be renamed across directories.

## Function 13 : File Delete

Deletes a file or directory. On input:
Parameters:0,1 points to the length-prefixed filename string.
Deleting a file which is open has undefined behaviour. Directories may
only be deleted if they are empty.

## Function 14 : Create Directory

Creates a new directory. On input:
Parameters:0,1 points to the length-prefixed filename string.

## Function 15 : Change Directory

Changes the current working directory. On input:
Parameters:0,1 points to the length-prefixed path string.

## Function 16 : File Stat

Retrieves information about a file by name. On input:
Parameters:0,1 points to the length-prefixed filename string.
Parameters:0..3 contains the length of the file;
Parameter:4 contains the attributes bit-field of the file.
If the file is open for writing, this may not return the correct size
due to buffered data not having been flushed to disk.
File attributes are a bitfield as follows: 0,0,0,Hidden, Read Only, Archive,
System, Directory.

## Function 17 : Open Directory

Opens a directory for enumeration. On input:
Parameters:0,1 points to the length-prefixed filename string.
Only one directory at a time may be opened. If a directory is already
open when this call is made, it is automatically closed. However, an
open directory may make it impossible to delete the directory; so
closing the directory after use is good practice.

## Function 18 : Read Directory

Reads an item from the currently open directory. On input:
Parameters:0,1 points to a length-prefixed buffer for returning the filename.
Parameters:0,1 is unchanged, but the buffer is updated to contain the
length-prefixed filename (without any leading path);
Parameters:2..5 contains the length of the file;
Parameter:6 contains the file attributes, as described by Function 3,16 "File Stat".
If there are no more items to read, this call fails and an error is flagged.

## Function 19 : Close Directory

Closes any directory opened previously by Function 3,17 "Open Directory".

## Function 20 : Copy File

Copies a file. On input:
Parameters:0,1 points to the length-prefixed old filename;
Parameters:2,3 points to the length-prefixed new filename.
Only single files may be copied, not directories.

## Function 21 : Set file attributes

Sets the attributes for a file. On input:
Parameters:0,1 points to the length-prefixed filename;
Parameter:2 is the attribute bitfield. (See Stat File for details.)
The directory bit cannot be changed. Obviously.

## Function 32 : List Filtered

Prints a filtered file listing of the current directory to the console. On input:
Parameters:0,1 points to the filename search string.
Files will only be shown if the name contains the search string (ie: a
substring match).

\newpage

# Group 4 : Mathematics

## Function 0 : Addition

Register1 := Register 1 + Register2

## Function 1 : Subtraction

Register1 := Register 1 - Register2

## Function 2 : Multiplication

Register1 := Register 1 * Register2

## Function 3 : Decimal Division

Register1 := Register 1 / Register2 (floating point)

## Function 4 : Integer Division

Register1 := Register 1 / Register2 (integer result)

## Function 5 : Integer Modulus

Register1 := Register 1 mod Register2

## Function 6 : Compare

Parameter:0 := Register 1 compare Register2 : returns $FF, 0, 1 for less equal and greater.
Note: float comparison is approximate because of rounding.

## Function 16 : Negate

Register1 :=  -Register 1

## Function 17 : Floor

Register1 := floor(Register 1)

## Function 18 : Square Root

Register1 := square root(Register 1)

## Function 19 : Sine

Register1 := sine(Register 1) angles in degrees/radians

## Function 20 : Cosine

Register1 := cosine(Register 1) angles in degrees/radians

## Function 21 : Tangent

Register1 := tangent(Register 1) angles in degrees/radians

## Function 22 : Arctangent

Register1 := arctangent(Register 1) angles in degrees/radians

## Function 23 : Exponent

Register1 :=  e to the power of Register 1

## Function 24 : Logarithm

Register1 := log(Register 1) natural logarithm

## Function 25 : Absolute Value

Register1 := absolute value(Register 1)

## Function 26 : Sign

Register1 := sign(Register 1), returns -1 0 or 1

## Function 27 : Random Decimal

Register1 := random float from 0-1

## Function 28 : Random Integer

Register1 := random integer from 0 to (Register 1-1)

## Function 32 : Number to Decimal

Helper function for tokeniser, do not use.

## Function 33 : String to Number

Convert the length prefixed string at Parameters:4,5 to a constant in Register1.

## Function 34 : Number to String

Convert the constant in Register1 to a length prefixed string which is stored at Parameters:4,5

## Function 35 : Set Degree/Radian Mode

Sets the use of degrees (the default) when non zero, radians when zero.

\newpage

# Group 5 : Graphics

## Function 1 : Set Defaults

Configure the global graphics system settings.
Not all parameters are relevant for all graphics commands;
but all parameters will be set by this command. So mind their values.
Refer to Section "Graphics Settings" for details.
The parameters are And, Or, Fill Flag, Extent, and Flip. Bit 0 of flip
sets the horizontal flip, Bit 1 sets the vertical flip.

## Function 2 : Draw Line

Draw a line between the screen coordinates specified
in  Parameters:0,1,Parameters:2,3 (begin X,Y)
and Parameters:4,5,Parameters:6,7 (end X,Y).

## Function 3 : Draw Rectangle

Draw a rectangle spanning the screen coordinates specified
in  Parameters:0,1,Parameters:2,3 (corner X,Y)
and Parameters:4,5,Parameters:6,7 (opposite corner X,Y).

## Function 4 : Draw Ellipse

Draw an ellipse spanning the screen coordinates specified
in  Parameters:0,1,Parameters:2,3 (corner  X,Y)
and Parameters:4,5,Parameters:6,7 (opposite corner X,Y).

## Function 5 : Draw Pixel

Draw a single pixel at the screen coordinates specified
in Parameters:0,1,Parameters:2,3 (X,Y).

## Function 6 : Draw Text

Draw the length-prefixed string of text stored
at the memory location specified in Parameters:4,5
at the screen character cell specified in Parameters:0,1,Parameters:2,3 (X,Y).

## Function 7 : Draw Image

Draw the image with image ID in Parameter:4
at the screen coordinates Parameters:0,1,Parameters:2,3 (X,Y).
The extent and flip settings influence this command.

## Function 8 : Draw Tilemap

Draw the current tilemap at the screen coordinates specified
in Parameters:0,1,Parameters:2,3 (top-left X,Y)
and Parameters:4,5,Parameters:6,7 (bottom-right X,Y)
using current graphics settings.

## Function 32 : Set Palette

Set the palette colour at the index spcified in Parameter:0
to the values in Parameter:1,Parameter:2,Parameter:3 (RGB).

## Function 33 : Read Pixel

Read a single pixel at the screen coordinates specified
in Parameters:0,1,Parameters:2,3 (X,Y).
When the routine completes, the result will be in Parameter:0. If sprites are in use, this will be the
background only (0..15), if sprites are not in use it may return (0..255)

## Function 34 : Reset Palette

Reset the palette to the defaults.

## Function 35 : Set Tilemap

Set the current tilemap.
Parameters:0,1 is the memory address of the tilemap,
and Parameters:2,3,Parameters:4,5 (X,Y) specifies the offset into the tilemap,
in units of pixels, of the top-left pixel of the tile.

## Function 36 : Read Sprite Pixel

Read Pixel from the sprite layer at the screen coordinates
specified in Parameters:0,1,Parameters:2,3 (X,Y).
When the routine completes, the result will be in Parameter:0.
Refer to Section "Pixel Colors" for details.

## Function 37 : Frame Count

Deposit into Parameters:0..3,
the number of v-blanks (full screen redraws) which have occurred since power-on.
This is updated at the start of each v-blank period.

## Function 38 : Get Palette

Get the palette colour at the index spcified in Parameter:0.
Values are returned in Parameter:1,Parameter:2,Parameter:3 (RGB).

## Function 39 : Write Pixel

Write Pixel index Parameter:4 to the screen coordinate specified in
Parameters:0,1,Parameters:2,3 (X,Y).

## Function 64 : Set Color

Set Color
Sets the current drawing colour to Parameter:0

## Function 65 : Set Solid Flag

Set Solid Flag
Sets the solid flag to Parameter:0, which indicates either solid fill (for shapes) or solid background (for images and fonts)

## Function 66 : Set Draw Size

Set Draw Size
Sets the drawing scale for images and fonts to Parameter:0

## Function 67 : Set Flip Bits

Set Flip Bits
Sets the flip bits for drawing images. Bit 0 set causes a horizontal flip, bit 1 set causes a vertical flip.

\newpage

# Group 6 : Sprites

## Function 1 : Sprite Reset

Reset the sprite system.

## Function 2 : Sprite Set

Set or update the sprite specified in Parameter:0.
The parameters are : Sprite Number, X Low, X High, Y Low, Y High, Image, Flip and Anchor and Flags
Bit 0 of flags specifies 32 bit sprites.
Values that are $80 or $8080 are not updated.

## Function 3 : Sprite Hide

Hide the sprite specified in Parameter:0.

## Function 4 : Sprite Collision

Parameter:0 is non-zero if the distance is less than or equal to Parameter:2
between the center of the sprite with index specified in Parameter:0
and the center of the sprite with index specified in Parameter:1 .

## Function 5 : Sprite Position

Deposit into Parameters:1..4, the screen coordinates of the sprite
with the index specified in Parameter:0.

\newpage

# Group 7 : Controller

## Function 1 : Read Default Controller

This reads the status of the base controller into Parameter:0, and is a compatibility
API call.
\newline

The base controller is the keyboard keys (these are WASD+OPKL or Arrow Keys+ZXCV) or the
gamepad controller buttons. Either works.
The 8 bits of the returned byte are the following buttons, most significant first :
Y X B A Down Up Right Left

## Function 2 : Read Controller Count

This returns the number of game controllers plugged in to the USB System into Parameter:0. This does not include the
keyboard based controller, only physical controller hardware.

## Function 3 : Read Controller

This returns a specific controller status. Controller 0 is the keyboard controller, Controllers 1 upwards are
those physical USB devices.
This returns a 32 bit value in Parameters:0..3 which currently is compatible with function 1, but allows for expansion.
The 8 bits of the returned byte are the following buttons, most significant first :
Y X B A Down Up Right Left

\newpage

# Group 8 : Sound

## Function 1 : Reset Sound

Reset the sound system.
This empties all channel queues and silences all channels immediately.

## Function 2 : Reset Channel

Reset the sound channel specified in Parameter:0.

## Function 3 : Beep

Play the startup beep immediately.

## Function 4 : Queue Sound

Queue a sound. Refer to Section \#\ref{sound} "Sound" for details.
The parameters are : Channel, Frequency Low, Frequency High,
Duration Low, Duration High, Slide Low, Slide High and Source.

## Function 5 : Play Sound

Play the sound effect specified in Parameter:1
on the channel specified in Parameter:0 immediately, clearing the channel queue.

## Function 6 : Sound Status

Deposit in Parameter:0 the number of notes outstanding before silence
in the queue of the channel specified in Parameter:0,
including the current playing sound, if any.

\newpage

# Group 9 : Turtle Graphics

## Function 1 : Turtle Initialise

Initialise the turtle graphics system.
Parameter:0 is the sprite number to use for the turtle,
as the turtle graphics system “adopts” one of the sprites.
The icon is not currently re-definable, and initially the turtle is hidden.

## Function 2 : Turtle Turn

Turn the turtle right by Parameter:0,1 degrees. Show if hidden. To turn left, turn by a negative amount.

## Function 3 : Turtle Move

Move the turtle forward by Parameter:0,1 degrees, drawing in colour Parameter:2,
pen down flag in Parameter:3. Show if hidden.

## Function 4 : Turtle Hide

Hide the turtle.

## Function 5 : Turtle Home

Move the turtle to the home position (in the center, pointing upward).

\newpage

# Group 10 : UExt I/O

## Function 1 : UExt Initialise

Initialise the UExt I/O system.
This resets the IO system to its default state, where all UEXT pins are I/O pins,
inputs and enabled.

## Function 2 : Write GPIO

This copies the value Parameter:1 to the output latch for UEXT pin Parameter:0.
This will only display on the output pin if it is enabled,
and its direction is set to "Output" direction.

## Function 3 : Read GPIO

If the pin is set to "Input" direction, reads the level on pin on UEXT port Parameter:0.
If it is set to "Output" direction, reads the output latch for pin on UEXT port Parameter:0.
If the read is successful, the result will be in Parameter:0.

## Function 4 : Set Port Direction

Set the port direction for UEXT Port Parameter:0 to the value in Parameter:1.
This can be $01 (Input), $02 (Output), or $03 (Analogue Input).

## Function 5 : Write I2C

Write to I2C Device Parameter:0, Register Parameter:1, value Parameter:2.
No error is flagged if the device is not present.

## Function 6 : Read I2C

Read from I2C Device Parameter:0, Register Parameter:1.
If the read is successful, the result will be in Parameter:0.
If the device is not present, this will flag an error.
Use FUNCTION 10,2 first, to check for its presence.

## Function 7 : Read Analog

Read the analogue value on UEXT Pin Parameter:0.
This has to be set to analogue type to work.
Returns a value from 0..4095 stored in Parameters:0,1,
which represents an input value of 0 to 3.3 volts.

## Function 8 : I2C Status

Try to read from I2C Device Parameter:0.
If present, then Parameter:0 will contain a non-zero value.

## Function 9 : Read I2C Block

Try to read a block of memory from I2C Device Parameter:0 into memory
at Parameters:1,2, length Parameters:3,4.

## Function 10 : Write I2C Block

Try to write a block of memory to I2C Device Parameter:0 from memory
at Parameters:1,2, length Parameters:3,4.

## Function 11 : Read SPI Block

Try to read a block of memory from SPI Device into memory
at Parameters:1,2, length Parameters:3,4.

## Function 12 : Write SPI Block

Try to write a block of memory to SPI Device from memory
at Parameters:1,2, length Parameters:3,4.

## Function 13 : Read UART Block

Try to read a block of memory from UART into memory
at Parameters:1,2, length Parameters:3,4. This can fail with a timeout.

## Function 14 : Write UART Block

Try to write a block of memory to UART from memory
at Parameters:1,2, length Parameters:3,4.

## Function 15 : Set UART Speed and Protocol

Set the Baud Rate and Serial Protocol for the UART interface. The baud rate is in Parameters:0..3
and the protocol number is Parameter:4. Currently only 8N1 is supported, this is protocol 0.

## Function 16 : Write byte to UART

Write byte Parameter:0 to the UART

## Function 17 : Read byte from UART

Read a byte from the UART. It is returned in Parameter:0

## Function 18 : Check if Byte Available

See if a byte is available in the UART input buffer. If available Parameter:0 is non zero.

\newpage

# Group 11 : Mouse

## Function 1 : Move display cursor

Positions the display cursor at Parameters:0,1,Parameters:2,3

## Function 2 : Set mouse display cursor on/off

Shows or hides the mouse cursor depending on the Parameter:0

## Function 3 : Get mouse state

Returns the mouse position (screen pixel, unsigned) in x Parameters:0,1 and y Parameters:2,3,
buttonstate in Parameter:4 (button 1 is 0x1, button 2 0x2 etc., set when pressed),
scrollwheelstate in Parameter:5 as uint8 which changes according to scrolls.

## Function 4 : Test mouse present

Returns non zero if a mouse is plugged in in Parameter:0

## Function 5 : Select mouse Cursor

Select a mouse cursor in Parameter:0 ; returns error status if the cursor is not available.

\newpage

# Group 12 : Blitter

## Function 1 : Blitter Busy

Returns a non zero value in Parameter:1 if the blitter/DMA system is currently transferring data, used to check availability
and transfer completion.

## Function 2 : Simple Blit Copy

Copy Parameters:6,7 bytes of internal memory from Parameter:0:Parameters:1,2 to Parameter:3:Parameters:4,5. Sets error flag if the transfer is not
possible (e.g. illegal write addresses). The upper 8 bits of the address are : 6502 RAM (00) VideoRAM (80,81) Graphics RAM(90)

## Function 3 : Complex Blit Copy

Copy a source rectangular area to a destination rectangular area.
It's oriented toward copying graphics data, but can be used as a more general-purpose memory mover.
The source and target areas may be different formats, and the copy will convert the data on the fly.
For example, you can expand 4bpp source graphics (two pixels per byte) into the 1 pixel per byte framebuffer.
However, the blitting is byte-oriented. So the source width is always rounded down to the nearest full byte.
Parameter  (0) is the blit action:
0 = copy
1 = copymasked - copy, but only where src is not the transparent value.
2 = solidmasked - set target to constant solid value, but only where src is not the transparent value.
See below for transparent/solid values.
Parameters (1,2) address of the source rectangle data.
Parameters (3,4) address of the target rectangle data.
The source and target rectangle data is laid out in memory as follows:
0-2     24 bit address to copy from/to (address is address:page:0)
3       pad byte (must be zero)
4-5     Stride, in bytes. This is the value to add to the address to get from one line to the next.
Used for both source and target.
For example:
- if blitting to the screen, a stride of screen width (320) would get to the next line.
- a zero source stride would repeat a single line for the whole copy.
- A negative target stride would draw from the bottom upward.
6       data format
0: bytes. Supported for both source and target.
1: pairs of 4-bit values (nibbles). Source only.
2: 8 single-bit values. Source only.
3: high nibble. Target only.
4: low nibble. Target only.
7       A constant to use as the "transparent" value for BLTACT_MASK and BLTACT_SOLID. Source only. Not used in target.
8       A constant to use as the "solid" value for BLTACT_SOLID. Source only. Not used in target.
9       Height. The number of lines to copy.
Source only. Not used in target.
The copy is driven by the source height.
10-11   Width. The number of values to copy for each line.
Source only. Not used in target.
The copy is driven by the source width.

## Function 4 : Blit Image

Blits an image from memory onto the screen. The image will be clipped,
so it's safe to blit partly (or fully) offscreen-images.
Parameter  (0) is the blit action (see function 3, Complex Blit):
Parameters (1,2) address of the source rectangle data.
Parameters (3,4) x pixel coordinate on screen (signed 16 bit)
Parameters (5,6) y pixel coordinate on screen (signed 16 bit)
Parameter  (7) destination format, determines how framebuffer will be written:
0: write to whole byte.
1: unsupported
2: unsupported
3: write to high nibble only.
4: write to low nibble only.
NOTE: clipping operates at byte resolution on the source data. So, for example, if you blit a 1-bit image (format 2) to an x-position of -2, then the whole first byte will be skipped leaving 6 empty pixels on the left. Same happens on the right - either the whole source byte is used, or it'll be skipped.

\newpage

# Group 13 : Editor

## Function 1 : Initialise Editor

Initialises the editor

## Function 2 : Reenter the Editor

Re-enters the system editor. Returns the function required for call out, the editors
sort of 'call backs' - see editor specification.

\newpage

