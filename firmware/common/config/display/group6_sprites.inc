// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      group6_sprites.inc
//      Authors :   Paul Robson (paul@robsons.org.uk)
//                  Bill Auger
//      Date :      22nd November 2023
//      Reviewed :  No
//      Purpose :   Sprite dispatcher.
//
// ***************************************************************************************
// ***************************************************************************************

// ***************************************************************************************
//
//                                    Group 6 (Sprites)
//
// ***************************************************************************************

GROUP 6 Sprites
    FUNCTION 1 Sprite Reset
        SPRReset();
    DOCUMENTATION
        Reset the sprite system.

    FUNCTION 2 Sprite Set
        *DERROR = SPRUpdate(DPARAMS);
    DOCUMENTATION
        Set or update the sprite specified in \Param{0}.
        \newline
        \ParamsBytes{Sprite Parameters}{Sprite}{X lo}{X hi}{Y lo}{Y hi}{Image}{Flip}{Anchor}
        \newline
        \SpriteParamBits{$FF09 - Image Parameters}{0}{32-bit}{Index}
        \newline
        \ParamBits{$FF0A - Flip Axis Flags}{0}{0}{0}{0}{0}{0}{Vertical}{Horizontal}
        \newline values that are $80 or $8080 are not updated.

    FUNCTION 3 Sprite Hide
        SPRHide(DPARAMS);
    DOCUMENTATION
        Hide the sprite specified in \Param{0}.

    FUNCTION 4 Sprite Collision
        DPARAMS[0] = SPRCollisionCheck(DERROR,DPARAMS[0],DPARAMS[1],DPARAMS[2]);
    DOCUMENTATION
        \Param{0} is non-zero if the distance is less than or equal to \Param{2}
        between the center of the sprite with index specified in \Param{0}
        and the center of the sprite with index specified in \Param{1} .

    FUNCTION 5 Sprite Position
        *DERROR = SPRGetSpriteData(DPARAMS);
    DOCUMENTATION
        Deposit into \Param{1..4}, the screen coordinates of the sprite
        with the index specified in \Param{0}.

