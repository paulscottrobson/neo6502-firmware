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
        Returns a non zero value in \Param{1} if the blitter/DMA system is currently transferring data, used to check availability
        and transfer completion.

    FUNCTION 2 Simple Blit Copy
        *DERROR = BLTSimpleCopy(DPARAMS[0],DSPGetInt16(DCOMMAND,5),DPARAMS[3],DSPGetInt16(DCOMMAND,8),DSPGetInt16(DCOMMAND,10));
    DOCUMENTATION
        Copy \Param{6,7} bytes of internal memory from \Param{0}:\Param{1,2} to \Param{3}:\Param{4,5}. Sets error flag if the transfer is not
        possible (e.g. illegal write addresses). The upper 8 bits of the address are : 6502 RAM (00) VideoRAM (80,81) Graphics RAM(90)

        
