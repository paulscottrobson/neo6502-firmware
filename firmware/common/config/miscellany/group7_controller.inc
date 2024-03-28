// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      group7_controller.inc
//      Authors :   Paul Robson (paul@robsons.org.uk)
//                  Bill Auger
//      Date :      22nd November 2023
//      Reviewed :  No
//      Purpose :   Dispatcher for controllers.
//
// ***************************************************************************************
// ***************************************************************************************

// ***************************************************************************************
//
//                                  Group 7 (Controller)
//
// ***************************************************************************************

GROUP 7 Controller

    FUNCTION 1 Read Default Controller
        *DPARAMS = KBDKeyboardController();
        for (int n = 0;n < GMPGetControllerCount();n++) {
            *DPARAMS |= GMPReadDigitalController(n) & 0xFF;
        }

    DOCUMENTATION
        This reads the status of the base controller into \Param{0}, and is a compatibility
        API call.
        \newline

        The base controller is the keyboard keys (these are WASD+OPKL or Arrow Keys+ZXCV) or the 
        gamepad controller buttons. Either works.

        \newline
        \ParamBits{$FF04 - Controller Flags}{Y}{X}{B}{A}{Down}{Up}{Right}{Left}

    FUNCTION 2 Read Controller Count
        *DPARAMS = GMPGetControllerCount();

    DOCUMENTATION
        This returns the number of game controllers plugged in to the USB System into \Param{0}. This does not include the
        keyboard based controller, only physical controller hardware.

    FUNCTION 3 Read Controller
        *DERROR = DPARAMS[0] > GMPGetControllerCount();
        if (*DERROR == 0) {
            if (DPARAMS[0] == 0) {
                *((uint32_t *)DPARAMS) = KBDKeyboardController();
            } else {
                *((uint32_t *)DPARAMS) = GMPReadDigitalController(DPARAMS[0]-1);
            }
        }
        
    DOCUMENTATION
        This returns a specific controller status. Controller 0 is the keyboard controller, Controllers 1 upwards are
        those physical USB devices.

        This returns a 32 bit value in \Param{0..3} which currently is compatible with function 1, but allows for expansion.

        \newline
        \ParamBits{$FF04 - Controller Flags}{Y}{X}{B}{A}{Down}{Up}{Right}{Left}

