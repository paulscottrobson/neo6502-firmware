// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      group11_mouse.inc
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      21st March 2024
//      Reviewed :  No
//      Purpose :   Dispatcher for mouse/cursor functions
//
// ***************************************************************************************
// ***************************************************************************************

// ***************************************************************************************
//
//                                  Group 11 (Mouse)
//
// ***************************************************************************************

GROUP 11 Mouse
    FUNCTION 1 Move display cursor
        CURSetPosition(DSPGetInt16(DCOMMAND,4),DSPGetInt16(DCOMMAND,6));
    DOCUMENTATION
        Positions the display cursor at \Param{0,1},\Param{2,3}        

    FUNCTION 2 Set mouse display cursor on/off
        CURSetVisible(DCOMMAND[4] != 0);
    DOCUMENTATION
        Shows or hides the mouse cursor depending on the \Param{0}
