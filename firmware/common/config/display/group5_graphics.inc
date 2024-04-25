// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      group5_graphics.inc
//      Authors :   Paul Robson (paul@robsons.org.uk)
//                  Bill Auger
//      Date :      22nd November 2023
//      Reviewed :  No
//      Purpose :   Graphics dispatcher.
//
// ***************************************************************************************
// ***************************************************************************************

// ***************************************************************************************
//
// Group 5 (Graphics)
//
// ***************************************************************************************

GROUP 5 Graphics
    FUNCTION 1 Set Defaults
        GFXSetDefaults(DCOMMAND);
    DOCUMENTATION
        Configure the global graphics system settings.
        Not all parameters are relevant for all graphics commands;
        but all parameters will be set by this command. So mind their values.
        Refer to Section "Graphics Settings" for details.

        The parameters are And, Or, Fill Flag, Extent, and Flip. Bit 0 of flip
        sets the horizontal flip, Bit 1 sets the vertical flip.

    FUNCTION 2 Draw Line
        GFXGraphicsCommand(2,DCOMMAND);
    DOCUMENTATION
        Draw a line between the screen coordinates specified
        in  Parameters:0,1,Parameters:2,3 (begin X,Y)
        and Parameters:4,5,Parameters:6,7 (end X,Y).

    FUNCTION 3 Draw Rectangle
        GFXGraphicsCommand(3,DCOMMAND);
    DOCUMENTATION
        Draw a rectangle spanning the screen coordinates specified
        in  Parameters:0,1,Parameters:2,3 (corner X,Y)
        and Parameters:4,5,Parameters:6,7 (opposite corner X,Y).

    FUNCTION 4 Draw Ellipse
        GFXGraphicsCommand(4,DCOMMAND);
    DOCUMENTATION
        Draw an ellipse spanning the screen coordinates specified
        in  Parameters:0,1,Parameters:2,3 (corner  X,Y)
        and Parameters:4,5,Parameters:6,7 (opposite corner X,Y).

    FUNCTION 5 Draw Pixel
        GFXGraphicsCommand(5,DCOMMAND);
    DOCUMENTATION
        Draw a single pixel at the screen coordinates specified
        in Parameters:0,1,Parameters:2,3 (X,Y).

    FUNCTION 6 Draw Text
        GFXGraphicsCommand(6,DCOMMAND);
    DOCUMENTATION
        Draw the length-prefixed string of text stored
        at the memory location specified in Parameters:4,5
        at the screen character cell specified in Parameters:0,1,Parameters:2,3 (X,Y).

    FUNCTION 7 Draw Image
        GFXGraphicsCommand(7,DCOMMAND);
    DOCUMENTATION
        Draw the image with image ID in Parameter:4
        at the screen coordinates Parameters:0,1,Parameters:2,3 (X,Y).
        The extent and flip settings influence this command.

    FUNCTION 8 Draw Tilemap
        *DERROR = TMPDrawTileMap(DCOMMAND);
    DOCUMENTATION
        Draw the current tilemap at the screen coordinates specified
        in Parameters:0,1,Parameters:2,3 (top-left X,Y)
        and Parameters:4,5,Parameters:6,7 (bottom-right X,Y)
        using current graphics settings.

    FUNCTION 32 Set Palette
        GFXSetPalette(DPARAMS[0],DPARAMS[1],DPARAMS[2],DPARAMS[3]);
    DOCUMENTATION
        Set the palette colour at the index spcified in Parameter:0
        to the values in Parameter:1,Parameter:2,Parameter:3 (RGB).

    FUNCTION 33 Read Pixel
        GFXGraphicsCommand(33,DCOMMAND);
    DOCUMENTATION
        Read a single pixel at the screen coordinates specified
        in Parameters:0,1,Parameters:2,3 (X,Y).
        When the routine completes, the result will be in Parameter:0. If sprites are in use, this will be the 
        background only (0..15), if sprites are not in use it may return (0..255)

    FUNCTION 34 Reset Palette
        GFXDefaultPalette();
    DOCUMENTATION
        Reset the palette to the defaults.

    FUNCTION 35 Set Tilemap
        TMPSelectTileMap(cpuMemory+DSPGetInt16(DCOMMAND,4),DSPGetInt16(DCOMMAND,6),DSPGetInt16(DCOMMAND,8));
    DOCUMENTATION
        Set the current tilemap.
        Parameters:0,1 is the memory address of the tilemap,
        and Parameters:2,3,Parameters:4,5 (X,Y) specifies the offset into the tilemap,
        in units of pixels, of the top-left pixel of the tile.

    FUNCTION 36 Read Sprite Pixel
        GFXGraphicsCommand(36,DCOMMAND);
    DOCUMENTATION
        Read Pixel from the sprite layer at the screen coordinates
        specified in Parameters:0,1,Parameters:2,3 (X,Y).
        When the routine completes, the result will be in Parameter:0.
        Refer to Section "Pixel Colors" for details.

    FUNCTION 37 Frame Count
        *((uint32_t *)DPARAMS) = RNDGetFrameCount();
    DOCUMENTATION
        Deposit into Parameters:0..3,
        the number of v-blanks (full screen redraws) which have occurred since power-on.
        This is updated at the start of each v-blank period.

    FUNCTION 38 Get Palette
        GFXGetPalette(DPARAMS[0],&DPARAMS[1],&DPARAMS[2],&DPARAMS[3]);
    DOCUMENTATION
        Get the palette colour at the index spcified in Parameter:0.
        Values are returned in Parameter:1,Parameter:2,Parameter:3 (RGB).

    FUNCTION 39 Write Pixel
        GFXSetDrawColour(DPARAMS[4]);GFXGraphicsCommand(5,DCOMMAND);
    DOCUMENTATION
        Write Pixel index Parameter:4 to the screen coordinate specified in 
        Parameters:0,1,Parameters:2,3 (X,Y).

    FUNCTION 64 Set Color
        GFXSetDrawColour(DPARAMS[0]);
    DOCUMENTATION
        Set Color
        Sets the current drawing colour to Parameter:0

    FUNCTION 65 Set Solid Flag
        GFXSetSolidFlag(DPARAMS[0]);
    DOCUMENTATION
        Set Solid Flag

        Sets the solid flag to Parameter:0, which indicates either solid fill (for shapes) or solid background (for images and fonts)

    FUNCTION 66 Set Draw Size
        GFXSetDrawSize(DPARAMS[0]);
    DOCUMENTATION
        Set Draw Size

        Sets the drawing scale for images and fonts to Parameter:0

    FUNCTION 67 Set Flip Bits
        GFXSetFlipBits(DPARAMS[0]);
    DOCUMENTATION
        Set Flip Bits
        Sets the flip bits for drawing images. Bit 0 set causes a horizontal flip, bit 1 set causes a vertical flip.

