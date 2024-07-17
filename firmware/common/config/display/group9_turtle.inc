// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      group9_turtle.inc
//      Authors :   Paul Robson (paul@robsons.org.uk)
//                  Bill Auger
//      Date :      22nd November 2023
//      Reviewed :  No
//      Purpose :   Turtle graphics dispatcher
//
// ***************************************************************************************
// ***************************************************************************************

// ***************************************************************************************
//
//                          Group 9 (Turtle Graphics)
//
// ***************************************************************************************

GROUP 9 Turtle Graphics

    FUNCTION 1 Turtle Initialise
        TTLInitialise(DPARAMS[0]);
    DOCUMENTATION
        Initialise the turtle graphics system.
        Parameter:0 is the sprite number to use for the turtle,as the turtle graphics system “adopts” one of the sprites.
        The icon is not currently re-definable, and initially the turtle is hidden.

    FUNCTION 2 Turtle Turn
        TTLRotate(DSPGetInt16(DCOMMAND,4));
    DOCUMENTATION
        Turn the turtle right by Parameter:0,1 degrees. Show if hidden. To turn left, turn by a negative amount.

    FUNCTION 3 Turtle Move
        TTLForward(DSPGetInt16(DCOMMAND,4),DCOMMAND[6],DCOMMAND[7]);
    DOCUMENTATION
        Move the turtle forward by Parameter:0,1 degrees, drawing in colour Parameter:2 if Parameter:3 is non-zero.

    FUNCTION 4 Turtle Hide
        TTLSetVisible(false);
    DOCUMENTATION
        Hide the turtle.

    FUNCTION 5 Turtle Home
        TTLHome();
    DOCUMENTATION
        Move the turtle to the home position (in the center, pointing upward).

    FUNCTION 6 Turtle Show
        TTLSetVisible(true);
    DOCUMENTATION
        Show the turtle.
