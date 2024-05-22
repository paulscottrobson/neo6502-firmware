// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      group12_blitter.inc
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      24th March 2024
//      Reviewed :  No
//      Purpose :   Blitter type functionality
//
// ***************************************************************************************
// ***************************************************************************************

// ***************************************************************************************
//
//                                  Group 12 (Blitter)
//
// ***************************************************************************************

GROUP 12 Blitter
    FUNCTION 1 Blitter Busy

        DPARAMS[0] = 0;
    DOCUMENTATION
        Returns a non zero value in Parameter:1 if the blitter/DMA system is currently transferring data, used to check availability
        and transfer completion.

    FUNCTION 2 Simple Blit Copy
        *DERROR = BLTSimpleCopy(DPARAMS[0],DSPGetInt16(DCOMMAND,5),DPARAMS[3],DSPGetInt16(DCOMMAND,8),DSPGetInt16(DCOMMAND,10));

    DOCUMENTATION
        Copy Parameters:6,7 bytes of internal memory from Parameter:0:Parameters:1,2 to Parameter:3:Parameters:4,5. Sets error flag if the transfer is not
        possible (e.g. illegal write addresses). The upper 8 bits of the address are : 6502 RAM (00) VideoRAM (80,81) Graphics RAM(90)

        
    FUNCTION 3 Complex Blit Copy
        *DERROR = BLTComplexCopy(DPARAMS[0],DSPGetInt16(DCOMMAND,5),DSPGetInt16(DCOMMAND,7));

    DOCUMENTATION
        Copy a source rectangular area to a destination rectangular area.
        It's oriented toward copying graphics data, but can be used as a more general-purpose memory mover.
        The source and target areas may be different formats, and the copy will convert the data on the fly.
        For example, you can expand 4bpp source graphics (two pixels per byte) into the 1 pixel per byte framebuffer.
        However, the blitting is byte-oriented. So the source width is always rounded down to the nearest full byte.

            Parameter  (0) is the blit action:

                           0 = copy
                           1 = copymasked - copy, but only where src is not the transparent value.
                           2 = solidmasked - set target to constant solid value, but only where src is not the transparent value.
                           See below for transparent/solid values.

            Parameters (1,2) address of the source rectangle data.

            Parameters (3,4) address of the target rectangle data.

            The source and target rectangle data is laid out in memory as follows:

                0-2     24 bit address to copy from/to (address is address:page:0)

                3       pad byte (must be zero)

                4-5     Stride, in bytes. This is the value to add to the address to get from one line to the next.
                        Used for both source and target.
                        For example:
                        - if blitting to the screen, a stride of screen width (320) would get to the next line.
                        - a zero source stride would repeat a single line for the whole copy.
                        - A negative target stride would draw from the bottom upward.

                6       data format
                        0: bytes. Supported for both source and target.
                        1: pairs of 4-bit values (nibbles). Source only.
                        2: 8 single-bit values. Source only.
                        3: high nibble. Target only.
                        4: low nibble. Target only.

                7       A constant to use as the "transparent" value for BLTACT_MASK and BLTACT_SOLID. Source only. Not used in target.

                8       A constant to use as the "solid" value for BLTACT_SOLID. Source only. Not used in target.

                9       Height. The number of lines to copy.
                        Source only. Not used in target.
                        The copy is driven by the source height.

                10-11   Width. The number of values to copy for each line.
                        Source only. Not used in target.
                        The copy is driven by the source width.

    FUNCTION 4 Blit Image
        *DERROR = BLTImage(DPARAMS[0], (DPARAMS[1] | DPARAMS[2] << 8),(DPARAMS[3] | DPARAMS[4] << 8), (DPARAMS[5] | DPARAMS[6] << 8), DPARAMS[7]);

    DOCUMENTATION
        Blits an image from memory onto the screen. The image will be clipped,
        so it's safe to blit partly (or fully) offscreen-images.

            Parameter  (0) is the blit action (see function 3, Complex Blit):

            Parameters (1,2) address of the source rectangle data.

            Parameters (3,4) x pixel coordinate on screen (signed 16 bit)

            Parameters (5,6) y pixel coordinate on screen (signed 16 bit)

            Parameter  (7) destination format, determines how framebuffer will be written:
                        0: write to whole byte.
                        1: unsupported
                        2: unsupported
                        3: write to high nibble only.
                        4: write to low nibble only.

        NOTE: clipping operates at byte resolution on the source data. So, for example, if you blit a 1-bit image (format 2) to an x-position of -2, then the whole first byte will be skipped leaving 6 empty pixels on the left. Same happens on the right - either the whole source byte is used, or it'll be skipped.

