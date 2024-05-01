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
//                                  Group 10 (UExt)
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
        Copy a rectangular area to a rectangular area (can be contiguous by size = amount of data, offset = 0, copy steps = 1)
            Parameter  (0) is zero.
            Parameters (1,2) point to the source rectangle data.
            Parameters (3,4) point to the target rectangle data.

            These are as follows
                0-3     24 bit address to copy from/to (address is address:page:0)
                4-7     Step size (e.g. if copying 32 pixels, this would be 32)
                8-11    Step offset (e.g. when doing screen lines this would be 320, the number of pixels per line)     
                12-15   Steps to copy.

            Activity halts when one of the steps to copy value is decremented to zero. To ignore the steps to copy in receive,
            set it to zero.

