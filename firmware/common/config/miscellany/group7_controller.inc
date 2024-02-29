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
// Group 7 (Controller)
//
// ***************************************************************************************

GROUP 7 ,0 Controller
    FUNCTION 1 Read Controller
        *DPARAMS = KBDKeyboardController();
    DOCUMENTATION
        Read the state of keyboard, joystick, etc.
// Q12: read back from which address? $FF04?
        Initially, the Controller is the keyboard.
        This Function interprets key presses and releases as a joystick.
        The system maintains a bit-array of which keys are pressed.
        Currently, the keyboard is the only available Controller.
// Q13: is the keyboard the only available Controller?
        \newline
        \ParamBits{$FF04 - Controller Flags}{0}{0}{B}{A}{Down}{Up}{Right}{Left}


