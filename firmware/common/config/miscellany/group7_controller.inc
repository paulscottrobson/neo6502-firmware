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
        if (GMPGetControllerCount() == 0) {
            *DPARAMS = KBDKeyboardController();
        } else {
            *DPARAMS = GMPReadDigitalController(0) & 0xFF;
        }

    DOCUMENTATION
        This reads the status of the default controller into \Param{0}.
        \newline

        The default controller is the keyboard if no gamepad is inserted. Keys used
        are WASD+OPKL or Arrow Keys+ZXCV

        \newline
        \ParamBits{$FF04 - Controller Flags}{Y}{X}{B}{A}{Down}{Up}{Right}{Left}
