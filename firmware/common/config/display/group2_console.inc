// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      group2_console.inc
//      Authors :   Paul Robson (paul@robsons.org.uk)
//                  Bill Auger
//      Date :      22nd November 2023
//      Reviewed :  No
//      Purpose :   Console dispatcher.
//
// ***************************************************************************************
// ***************************************************************************************


// ***************************************************************************************
//
//                                  Group 2 (Console)
//
// ***************************************************************************************

GROUP 2 Console
    FUNCTION 0 Write Character
        CONWrite(*DPARAMS);
    DOCUMENTATION
        Console out (duplicate of Function 6 for backward compatibility).

    FUNCTION 1 Read Character
        *(DPARAMS) = KBDGetKey();
    DOCUMENTATION
        Read and remove a key press from the keyboard queue into \Param{0}.
        This is the ASCII value of the keystroke.
        If there are no key presses in the queue, \Param{0} will be zero.
        \newline
        Note that this Function is best for text input, but not for games.
        Function 7,1 is more optimal for games, as this only detects key presses,
        you cannot check whether the key is currently down or not.

    FUNCTION 2 Console Status
        *(DPARAMS) = KBDIsKeyAvailable() ? 0xFF: 0;
    DOCUMENTATION // Function 2 is key available in queue
        Check to see if the keyboard queue is empty.
        If it is, \Param{0} will be \MonoSp{$FF}, otherwise it will be \MonoSp{$00}.

    FUNCTION 3 Read Line
        CONGetScreenLine(DSPGetInt16(DCOMMAND,4));
    DOCUMENTATION
        Input the current line below the cursor into \Param{0,1} as a length-prefixed string;
        and move the cursor to the line below. Handles multiple-line input.

    FUNCTION 4 Define Hotkey
        *DERROR = KBDSetFunctionKey(*(DCOMMAND+4),DSPGetString(DCOMMAND,6));
    DOCUMENTATION
        Define the function key F1..F10 (\MonoSp{$01..$0A}) specified in \Param{0} to emit the
        length-prefixed string stored at the memory location specified in \Param{2,3}.
        For example, in a block of in-line assembly within a BASIC program,
        the string: \MonoSp{06,12,108,105,115,116,13} would clear the screen (\MonoSp{12}),
        then list the program (\MonoSp{108}='l',\MonoSp{105}='i',\MonoSp{115}='s',\MonoSp{116}='t',\MonoSp{13}='ENTER').
        \newline F11 and F12 cannot currently be defined.

    FUNCTION 5 Define Character
        *DERROR = CONUpdateUserFont(DPARAMS);
    DOCUMENTATION
        Define a font character specified in \Param{0} within the range of 192..255.
        Fill bits 0..5 (columns) of \Param{1..7} (rows) with the character bitmap.

    FUNCTION 6 Write Character
        CONWrite(*DPARAMS);
    DOCUMENTATION
        Write the character specified in \Param{0} to the console at the cursor position.
        Refer to Section \#\ref{console-codes} "Console Codes" for details.

    FUNCTION 7 Set Cursor Pos
        *DERROR = CONSetCursorPosition(DPARAMS[0],DPARAMS[1]);
    DOCUMENTATION
        Move the cursor to the screen character cell \Param{0} (X), \Param{1} (Y).

    FUNCTION 8 List Hotkeys
        KBDShowFunctionKeys();
    DOCUMENTATION
        Display the current function key definitions.

    FUNCTION 9 Screen Size
        CONGetScreenSizeChars(&DPARAMS[0],&DPARAMS[1]);
    DOCUMENTATION
        Returns the console size in characters, in \Param{0} (height) and \Param{1} (width).

    FUNCTION 10 Insert Line
        CONInsertLine(DPARAMS[0]);
    DOCUMENTATION
        This is a support function which inserts a blank line in the console and should not be used.

    FUNCTION 11 Delete Line
        CONDeleteLine(DPARAMS[0]);
    DOCUMENTATION
        This is a support function which deletes a line in the console and should not be used.

    FUNCTION 12 Clear Screen
        CONClearScreen();
    DOCUMENTATION
        Clears the screen.

    FUNCTION 13 Cursor Position
        CONGetCursorPosition(&DPARAMS[0],&DPARAMS[1]);
    DOCUMENTATION
        Returns the current screen character cell of the cursor
        in \Param{0} (X), \Param{1} (Y).

    FUNCTION 14 Clear Region
        CONClearArea(DPARAMS[0],DPARAMS[1],DPARAMS[2],DPARAMS[3]);
    DOCUMENTATION
        Erase all characters within the rectangular region specified
        in \Param{0,1} (begin X,Y) and \Param{2,3} (end X,Y).

    FUNCTION 15 Set Text Color
        CONSetForeBackColour(DPARAMS[0],DPARAMS[1]);
    DOCUMENTATION
        Sets the foreground colour to \Param{0} and the background colour to \Param{1}.

    FUNCTION 16 Cursor Inverse
        CONReverseCursorBlock();
    DOCUMENTATION
        Toggles the cursor colour between normal and inverse
        (ie: swaps FG and BG colors). This should not be used.

    FUNCTION 17 Tab() implementation
        CONGetCursorPosition(&u2,&u4);
        while (u2 < DPARAMS[0]) {
            CONWrite(' ');
            CONGetCursorPosition(&u2,&u4);
        }
    DOCUMENTATION
        Internal helper.
