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
// Group 5 (Graphics)
//
// ***************************************************************************************

GROUP 5 ,0,5 Graphics
    FUNCTION 1 Set Defaults
        GFXSetDefaults(DCOMMAND);
    DOCUMENTATION
        Configure the global graphics system settings.
        Not all parameters are relevant for all graphics commands;
        but all parameters will be set by this command. So mind their values.
        Refer to Section \#\ref{subsec:graphics-settings} "Graphics Settings" for details.
        \newline
        \ParamsBytes{Graphics Settings}{AND}{XOR}{Fill}{Extent}
                                       {Flip}{unused}{\textit{unused}}{\textit{unused}}
        \newline
        \ParamBits{$FF08 - Flip Axis Flags}{0}{0}{0}{0}{0}{0}{Vertical}{Horizontal}

    FUNCTION 2 Draw Line
        GFXGraphicsCommand(2,DCOMMAND);
    DOCUMENTATION
        Draw a line between the screen coordinates specified
        in  \Param{0,1},\Param{2,3} (begin X,Y)
        and \Param{4,5},\Param{6,7} (end X,Y).
        \newline
        \ParamsBytes{Draw Line Parameters}{X  lo}{X  hi}{Y  lo}{Y  hi}
                                          {X' lo}{X' hi}{Y' lo}{Y' hi}

    FUNCTION 3 Draw Rectangle
        GFXGraphicsCommand(3,DCOMMAND);
    DOCUMENTATION
        Draw a rectangle spanning the screen coordinates specified
        in  \Param{0,1},\Param{2,3} (corner X,Y)
        and \Param{4,5},\Param{6,7} (opposite corner X,Y).

    FUNCTION 4 Draw Elipse
        GFXGraphicsCommand(4,DCOMMAND);
    DOCUMENTATION
        Draw an ellipse spanning the screen coordinates specified
        in  \Param{0,1},\Param{2,3} (corner  X,Y)
        and \Param{4,5},\Param{6,7} (opposite corner X,Y).

    FUNCTION 5 Draw Pixel
        GFXGraphicsCommand(5,DCOMMAND);
    DOCUMENTATION
        Draw a single pixel at the screen coordinates specified
        in \Param{0,1},\Param{2,3} (X,Y).

    FUNCTION 6 Draw Text
        GFXGraphicsCommand(6,DCOMMAND);
    DOCUMENTATION
        Draw the length-prefixed string of text stored
        at the memory location specified in \Param{4,5}
        at the screen character cell specified in \Param{0,1},\Param{2,3} (X,Y).

    FUNCTION 7 Draw Image
        GFXGraphicsCommand(7,DCOMMAND);
    DOCUMENTATION
        Draw the image with image ID in \Param{4}
        at the screen coordinates \Param{0,1},\Param{2,3} (X,Y).
        The extent and flip settings influence this command.

    FUNCTION 8 Draw Tilemap
        *DERROR = TMPDrawTileMap(DCOMMAND);
    DOCUMENTATION
        Draw the current tilemap at the screen coordinates specified
        in \Param{0,1},\Param{2,3} (top-left X,Y)
        and \Param{4,5},\Param{6,7} (bottom-right X,Y)
        using current graphics settings.

    FUNCTION 32 Set Palette
        gMode.setPalette(DPARAMS[0],DPARAMS[1],DPARAMS[2],DPARAMS[3]);
    DOCUMENTATION
        Set the palette colour at the index spcified in \Param{0}
        to the values in \Param{1},\Param{2},\Param{3} (RGB).

    FUNCTION 33 Read Pixel
        GFXGraphicsCommand(33,DCOMMAND);
    DOCUMENTATION
        Read a single pixel at the screen coordinates specified
        in \Param{0,1},\Param{2,3} (X,Y).
        When the routine completes, the result will be in \Param{0}.
// Q17: these are guesses - the current api.pdf does not document Function 5,33

    FUNCTION 34 Reset Palette
        GFXDefaultPalette();
    DOCUMENTATION
        Reset the palette to the defaults.

    FUNCTION 35 Set Tilemap
        TMPSelectTileMap(cpuMemory+DSPGetInt16(DCOMMAND,4),DSPGetInt16(DCOMMAND,6),DSPGetInt16(DCOMMAND,8));
    DOCUMENTATION
        Set the current tilemap.
        \Param{0,1} is the memory address of the tilemap,
        and \Param{2,3},\Param{4,5} (X,Y) specifies the offset into the tilemap,
        in units of pixels, of the top-left pixel of the tile.

    FUNCTION 36 Read Sprite Pixel
        GFXGraphicsCommand(36,DCOMMAND);
    DOCUMENTATION
        Read Pixel from the sprite layer at the screen coordinates
        specified in \Param{0,1},\Param{2,3} (X,Y).
        When the routine completes, the result will be in \Param{0}.
        Refer to Section \#\ref{graphics-colors} "Pixel Colors" for details.

    FUNCTION 37 Frame Count
        *((uint32_t *)DPARAMS) = RNDGetFrameCount();
    DOCUMENTATION
        Deposit into \Param{0..3},
        the number of v-blanks (full screen redraws) which have occurred since power-on.
        This is updated at the start of each v-blank period.

    FUNCTION 64 Set Color
        GFXSetDrawColour(DPARAMS[0]);
    DOCUMENTATION
        Set Color
        \newline TODO: explain this function and any parameters

    FUNCTION 65 Set Solid Flag
        GFXSetSolidFlag(DPARAMS[0]);
    DOCUMENTATION
        Set Solid Flag
        \newline TODO: explain this function and any parameters

    FUNCTION 66 Set Draw Size
        GFXSetDrawSize(DPARAMS[0]);
    DOCUMENTATION
        Set Draw Size
        \newline TODO: explain this function and any parameters

    FUNCTION 67 Set Flip Bits
        GFXSetFlipBits(DPARAMS[0]);
    DOCUMENTATION
        Set Flip Bits
        \newline TODO: explain this function and any parameters

