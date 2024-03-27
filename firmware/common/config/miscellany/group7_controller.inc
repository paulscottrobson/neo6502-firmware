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
        if (GMPGetControllerCount() != 0) {
            *DPARAMS |= GMPReadDigitalController(0) & 0xFF;
        }

    DOCUMENTATION
        This reads the status of the base controller into \Param{0}, and is a compatibility
        API call.
        \newline

        The base controller is the keyboard keys (these are WASD+OPKL or Arrow Keys+ZXCV) or the 
        gamepad controller buttons. Either works.

        \newline
        \ParamBits{$FF04 - Controller Flags}{Y}{X}{B}{A}{Down}{Up}{Right}{Left}
