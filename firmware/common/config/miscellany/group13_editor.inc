// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      group13_editor.inc
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      29th March 2024
//      Reviewed :  No
//      Purpose :   Dispatcher for editor functions
//
// ***************************************************************************************
// ***************************************************************************************

// ***************************************************************************************
//
//                                  Group 13 (Editor)
//
// ***************************************************************************************

GROUP 13 Editor
    FUNCTION 1 Initialise Editor
        DCOMMAND[4] = EDITInitialise();
    DOCUMENTATION
        Initialises the editor

    FUNCTION 2 Reenter the Editor
        DCOMMAND[4] = EDITContinue();
    DOCUMENTATION
        Re-enters the system editor. Returns the function required for call out, the editors
        sort of 'call backs' - see editor specification.
