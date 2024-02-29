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
// Group 9 (Turtle Graphics)
//
// ***************************************************************************************

GROUP 9 ,0 Turtle Graphics
    FUNCTION 1 Turtle Initialise
        TTLInitialise(DPARAMS[0]);
    DOCUMENTATION
        Initialise the turtle graphics system.
        \Param{0} is the sprite number to use for the turtle,
        as the turtle graphics system “adopts” one of the sprites.
        The icon is not currently re-definable, and initially the turtle is hidden.

    FUNCTION 2 Turtle Turn
        TTLRotate(DSPGetInt16(DCOMMAND,4));
    DOCUMENTATION
        Turn the turtle right by \Param{0}\Param{1} degrees. Show if hidden.

    FUNCTION 3 Turtle Move
        TTLMove(DSPGetInt16(DCOMMAND,4),DCOMMAND[6],DCOMMAND[7]);
    DOCUMENTATION
        Move the turtle forward by \Param{0}\Param{1} degrees, drawing in colour \Param{2},
        pen down flag in \Param{3}. Show if hidden.

    FUNCTION 4 Turtle Hide
        TTLHide();
    DOCUMENTATION
        Hide the turtle.

    FUNCTION 5 Turtle Home
        TTLHome();TTLUpdate();
    DOCUMENTATION
        Move the turtle to the home position (in the center, pointing upward).

