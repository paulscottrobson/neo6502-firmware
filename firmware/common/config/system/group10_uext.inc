// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      group10_uext.inc
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      22nd November 2023
//      Reviewed :  No
//      Purpose :   UEXT function handlers.
//
// ***************************************************************************************
// ***************************************************************************************

// ***************************************************************************************
//
//                                  Group 10 (UExt)
//
// ***************************************************************************************

GROUP 10 UExt I/O
    FUNCTION 1 UExt Initialise
        IOInitialise();
    DOCUMENTATION
        Initialise the UExt I/O system.
        This resets the IO system to its default state, where all UEXT pins are I/O pins,
        inputs and enabled.

    FUNCTION 2 Write GPIO
        *DERROR = IOWrite(DPARAMS[0],DPARAMS[1] != 0);
    DOCUMENTATION
        This copies the value \Param{1} to the output latch for UEXT pin \Param{0}.
        This will only display on the output pin if it is enabled,
        and its direction is set to "Output" direction.

    FUNCTION 3 Read GPIO
        *DERROR = IORead(DPARAMS[0],&b1);
        DPARAMS[0] = b1 ? 0xFF:0;
    DOCUMENTATION
        If the pin is set to "Input" direction, reads the level on pin on UEXT port \Param{0}.
        If it is set to "Output" direction, reads the output latch for pin on UEXT port \Param{0}.
        If the read is successful, the result will be in \Param{0}.

    FUNCTION 4 Set Port Direction
        *DERROR = IOSetDirection(DPARAMS[0],DPARAMS[1]);
    DOCUMENTATION
        Set the port direction for UEXT Port \Param{0} to the value in \Param{1}.
        This can be $01 (Input), $02 (Output), or $03 (Analogue Input).

    FUNCTION 5 Write I2C
        *DERROR = IOI2CWriteRegister(DPARAMS[0],DPARAMS[1],DPARAMS[2]);
    DOCUMENTATION
        Write to I2C Device \Param{0}, Register \Param{1}, value \Param{2}.
        No error is flagged if the device is not present.

    FUNCTION 6 Read I2C
        *DERROR = IOI2CReadRegister(DPARAMS[0],DPARAMS[1],&u2);
        DPARAMS[0] = u2;
    DOCUMENTATION
        Read from I2C Device \Param{0}, Register \Param{1}.
        If the read is successful, the result will be in \Param{0}.
        If the device is not present, this will flag an error.
        Use FUNCTION 10,2 first, to check for its presence.

    FUNCTION 7 Read Analog
        *DERROR = IOReadAnalogue(DPARAMS[0],&u3);
        DPARAMS[0] = u3 & 0xFF;DPARAMS[1] = (u3 >> 8);
    DOCUMENTATION
        Read the analogue value on UEXT Pin \Param{0}.
        This has to be set to analogue type to work.
        Returns a value from 0..4095 stored in \Param{0,1},
        which represents an input value of 0 to 3.3 volts.

    FUNCTION 8 I2C Status
        DPARAMS[0] = (IOI2CReadBlock(DPARAMS[0],&u2,1) == 0) ? 1 : 0;
    DOCUMENTATION
        Try to read from I2C Device \Param{0}.
        If present, then \Param{0} will contain a non-zero value.

    FUNCTION 9 Read I2C Block
        *DERROR = IOI2CReadBlock(DPARAMS[0],cpuMemory+DSPGetInt16(DCOMMAND,5),DSPGetInt16(DCOMMAND,7));
    DOCUMENTATION
        Try to read a block of memory from I2C Device \Param{0} into memory
        at \Param{1,2}, length \Param{3,4}.

    FUNCTION 10 Write I2C Block
        *DERROR = IOI2CWriteBlock(DPARAMS[0],cpuMemory+DSPGetInt16(DCOMMAND,5),DSPGetInt16(DCOMMAND,7));
    DOCUMENTATION
        Try to write a block of memory to I2C Device \Param{0} from memory
        at \Param{1,2}, length \Param{3,4}.

    FUNCTION 11 Read SPI Block
        *DERROR = IOSPIReadBlock(cpuMemory+DSPGetInt16(DCOMMAND,5),DSPGetInt16(DCOMMAND,7));
    DOCUMENTATION
        Try to read a block of memory from SPI Device into memory
        at \Param{1,2}, length \Param{3,4}.

    FUNCTION 12 Write SPI Block
        *DERROR = IOSPIWriteBlock(cpuMemory+DSPGetInt16(DCOMMAND,5),DSPGetInt16(DCOMMAND,7));
    DOCUMENTATION
        Try to write a block of memory to SPI Device from memory
        at \Param{1,2}, length \Param{3,4}.

    FUNCTION 13 Read UART Block
        *DERROR = IOUARTReadBlock(cpuMemory+DSPGetInt16(DCOMMAND,5),DSPGetInt16(DCOMMAND,7));
    DOCUMENTATION
        Try to read a block of memory from UART into memory
        at \Param{1,2}, length \Param{3,4}. This can fail with a timeout.

    FUNCTION 14 Write UART Block
        *DERROR = IOUARTWriteBlock(cpuMemory+DSPGetInt16(DCOMMAND,5),DSPGetInt16(DCOMMAND,7));
    DOCUMENTATION
        Try to write a block of memory to UART from memory
        at \Param{1,2}, length \Param{3,4}.

    FUNCTION 15 Set UART Speed and Protocol
        *DERROR = 0;IOUARTInitialise(DSPGetInt16(DCOMMAND,4),DPARAMS[2]);
    DOCUMENTATION
        Set the Baud Rate and Serial Protocol for the UART interface. The baud rate is in \Param{0,1}
        and the protocol number is \Param{2}. Currently only 8N1 is supported, this is protocol 0.

    FUNCTION 16 Write byte to UART
        *DERROR = 0;SERWriteByte(DPARAMS[0]);
    DOCUMENTATION
        Write byte \Param{0} to the UART

    FUNCTION 17 Read byte from UART
        if (SERIsByteAvailable()) {
            *DERROR = 0;
            DPARAMS[0] = SERReadByte();
        } else {
            *DERROR = 1;
        }
    DOCUMENTATION
        Read a byte from the UART. It is returned in \Param{0}

    FUNCTION 18 Check if Byte Available
        DPARAMS[0] = SERIsByteAvailable() ? 1 : 0;
    DOCUMENTATION
        See if a byte is available in the UART input buffer. If available \Param{0} is non zero.