// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      dispatch.config
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      22nd November 2023
//      Reviewed :  No
//      Purpose :   Dispatched for commands
//
// ***************************************************************************************
// ***************************************************************************************


// ***************************************************************************************
//
// Group 2 (Console)
//
// ***************************************************************************************

GROUP 2 ,0,8 Console
    FUNCTION 0 Write Character
        CONWrite(*DPARAMS);
    DOCUMENTATION
      Function 0 is console out (duplicate of Function 6 for backward compatibility).

    FUNCTION 1 Read Character
        *(DPARAMS) = KBDGetKey();
    DOCUMENTATION
        Read and remove a key press from the keyboard queue into \Param{0}.
        This is the ASCII value of the keystroke.
        If there are no key presses in the queue, \Param{0} will be zero.
        \newline
        Note that this Function is best for text input, but not for games.
        Function 7,1 is more optimal for games.
// Q14: is this the intention? "Use Function 7,1 for games"

    FUNCTION 2 Console Status
        *(DPARAMS) = KBDIsKeyAvailable() ? 0xFF: 0;
    DOCUMENTATION // Function 2 is key available in queue
        Check to see if the keyboard queue is empty.
        If it is, \Param{0} will be \MonoSp{$FF}, otherwise it will be \MonoSp{$00}.

    FUNCTION 3 Read Line
        CONGetScreenLine(DSPGetInt16(DCOMMAND,4));
    DOCUMENTATION
        Input the current line below the cursor into YX as a length-prefixed string;
        and move the cursor to the line below. Handles multiple-line input.
// Q15: what is YX?

    FUNCTION 4 Define Hotkey
        *DERROR = KBDSetFunctionKey(*(DCOMMAND+4),DSPGetString(DCOMMAND,6));
    DOCUMENTATION
        Define the function key F1..F10 (\MonoSp{$01..$0A}) specified in \Param{0} to emit the
        length-prefixed string stored at the memory location specified in \Param{2,3}.
        For example, in a block of in-line assembly within a BASIC program,
        the string: \MonoSp{06,12,108,105,115,116,13} would clear the screen (\MonoSp{12}),
        then list the program (\MonoSp{108}='l',\MonoSp{105}='i',\MonoSp{115}='s',\MonoSp{116}='t',\MonoSp{13}='ENTER').
// Q16: so $01 is F1 key, $0A is F10 key? - why not also F11 and F12?

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
        Move the cursor to the screen character cell \Param{0}\textless-X, \Param{1}\textless-Y.

    FUNCTION 8 List Hotkeys
        KBDShowFunctionKeys();
    DOCUMENTATION
        Display the current function key definitions.

    FUNCTION 9 Screen Size
        CONGetScreenSizeChars(&DPARAMS[0],&DPARAMS[1]);
    DOCUMENTATION
        Fetches the screen size, in chars.
        \newline TODO: explain this function and any parameters

    FUNCTION 10 Insert Line
        CONInsertLine(DPARAMS[0]);
    DOCUMENTATION
        Insert Line
        \newline TODO: explain this function and any parameters

    FUNCTION 11 Delete Line
        CONDeleteLine(DPARAMS[0]);
    DOCUMENTATION
        Delete Line
        \newline TODO: explain this function and any parameters

    FUNCTION 12 Clear Screen
        CONClearScreen();
    DOCUMENTATION
        Clears the screen.

    FUNCTION 13 Cursor Position
        CONGetCursorPosition(&DPARAMS[0],&DPARAMS[1]);
    DOCUMENTATION
        Get Cursor Position
        \newline TODO: explain this function and any parameters

    FUNCTION 14 Clear Region
        CONClearArea(DPARAMS[0],DPARAMS[1],DPARAMS[2],DPARAMS[3]);
    DOCUMENTATION
        Erase all characters within the rectangular region specified
        in \Param{0,1} (begin X,Y) and \Param{2,3} (end X,Y).

    FUNCTION 15 Set Text Color
        CONSetForeBackColour(DPARAMS[0],DPARAMS[1]);
    DOCUMENTATION
        Set Text Color
        \newline TODO: explain this function and any parameters

    FUNCTION 16 Cursor Inverse
        CONReverseCursorBlock();
    DOCUMENTATION
        Toggles the cursor colour between normal and inverse
        (ie: swaps FG and BG colors).

