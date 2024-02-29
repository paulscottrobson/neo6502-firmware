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
// Group 10 (UExt)
//
// ***************************************************************************************

GROUP 10 UExt I/O
    FUNCTION 1 UExt Initialise
        IOInitialise();
    DOCUMENTATION
        Initialise the UExt I/O system
        \newline TODO: explain this function and any parameters

    FUNCTION 2 Write GPIO
        *DERROR = IOWrite(DPARAMS[0],DPARAMS[1] != 0);
    DOCUMENTATION
        (P0,P1)
        \newline TODO: explain this function and any parameters

    FUNCTION 3 Read GPIO
        *DERROR = IORead(DPARAMS[0],&b1);
        DPARAMS[0] = b1 ? 0xFF:0;
    DOCUMENTATION
        P0 = GPIO(P0)
        \newline TODO: explain this function and any parameters

    FUNCTION 4 Set Port Direction
        *DERROR = IOSetDirection(DPARAMS[0],DPARAMS[1]);
    DOCUMENTATION
        P0 to P1
        \newline TODO: explain this function and any parameters

    FUNCTION 5 Write I2C
        *DERROR = IOI2CWriteRegister(DPARAMS[0],DPARAMS[1],DPARAMS[2]);
    DOCUMENTATION
        (P0,P1,P2)
        \newline TODO: explain this function and any parameters

    FUNCTION 6 Read I2C
        *DERROR = IOI2CReadRegister(DPARAMS[0],DPARAMS[1],&u2);
        DPARAMS[0] = u2;
    DOCUMENTATION
        P0 = (P0,P1)
        \newline TODO: explain this function and any parameters

    FUNCTION 7 Read GPIO Analog
        *DERROR = IOReadAnalogue(DPARAMS[0],&u3);
        DPARAMS[0] = u3 & 0xFF;DPARAMS[1] = (u3 >> 8);
    DOCUMENTATION
        P1P0 = GPIOAnalogue(P0)
        \newline TODO: explain this function and any parameters
