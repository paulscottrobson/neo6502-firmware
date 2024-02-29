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

GROUP 7 ,0 Controller
    FUNCTION 1 Read Controller
        *DPARAMS = KBDKeyboardController();
    DOCUMENTATION
        This reads the status of the default controller into \Param{0}
        \newline Initially, the Controller is the keyboard.
        This Function interprets key presses and releases as a joystick.
        The system maintains a bit-array of which keys are pressed.
        Currently, the keyboard is the only available Controller. 
        \newline   This function will become obsolete shortly as a more extensive API using USB
        controllers becomes available. However, it will maintain backward compatibility.
        \newline
        \ParamBits{$FF04 - Controller Flags}{0}{0}{B}{A}{Down}{Up}{Right}{Left}


