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
        MSESetPosition(DSPGetInt16(DCOMMAND,4),DSPGetInt16(DCOMMAND,6));
    DOCUMENTATION
        Positions the display cursor at Parameters:0,1,Parameters:2,3

    FUNCTION 2 Set mouse display cursor on/off
        MSESetVisible(DCOMMAND[4] != 0);
    DOCUMENTATION
        Shows or hides the mouse cursor depending on the Parameter:0

    FUNCTION 3 Get mouse state
        MSEGetState((uint16_t*)DPARAMS, (uint16_t*)(DPARAMS + 2), DPARAMS + 4, DPARAMS + 5);
    DOCUMENTATION
        Returns the mouse position (screen pixel, unsigned) in x Parameters:0,1 and y Parameters:2,3,
        buttonstate in Parameter:4 (button 1 is 0x1, button 2 0x2 etc., set when pressed),
        scrollwheelstate in Parameter:5 as uint8 which changes according to scrolls.

    FUNCTION 4 Test mouse present
        *DPARAMS = MSEMousePresent() ? 1 : 0;
    DOCUMENTATION
        Returns non zero if a mouse is plugged in in Parameter:0
                
    FUNCTION 5 Select mouse Cursor
        *DERROR = CURSetCurrent(DPARAMS[0]);                
    DOCUMENTATION
        Select a mouse cursor in Parameter:0 ; returns error status if the cursor is not available.
        