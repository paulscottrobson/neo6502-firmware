// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      group8_sound.inc
//      Authors :   Paul Robson (paul@robsons.org.uk)
//                  Bill Auger
//      Date :      22nd November 2023
//      Reviewed :  No
//      Purpose :   Sound dispatcher
//
// ***************************************************************************************
// ***************************************************************************************

// ***************************************************************************************
//
//                                  Group 8 (Sound)
//
// ***************************************************************************************

GROUP 8 Sound
    FUNCTION 1 Reset Sound
        SNDResetAll();
    DOCUMENTATION
        Reset the sound system.
        This empties all channel queues and silences all channels immediately.

    FUNCTION 2 Reset Channel
        *DERROR = SNDResetChannel(*DPARAMS);
    DOCUMENTATION
        Reset the sound channel specified in Parameter:0.

    FUNCTION 3 Beep
        SNDStartup();
    DOCUMENTATION
        Play the startup beep immediately.

    FUNCTION 4 Queue Sound
        su.frequency = DSPGetInt16(DCOMMAND,5);
        su.timeCS = DSPGetInt16(DCOMMAND,7);
        su.slide = DSPGetInt16(DCOMMAND,9);
        *DERROR = SNDPlay(DPARAMS[0],&su);
    DOCUMENTATION
        Queue a sound. Refer to Section \#\ref{sound} "Sound" for details.

        The parameters are : Channel, Frequency Low, Frequency High,
        Duration Low, Duration High, Slide Low, Slide High and Source.

    FUNCTION 5 Play Sound
        *DERROR = SFXPlay(DPARAMS[0],DPARAMS[1]);
    DOCUMENTATION
        Play the sound effect specified in Parameter:1
        on the channel specified in Parameter:0 immediately, clearing the channel queue.

    FUNCTION 6 Sound Status
        i1 = SNDGetNoteCount(DPARAMS[0]);
        DPARAMS[0] = i1 & 0xFF;
        *DERROR = (i1 < 0) ? 1 : 0;
    DOCUMENTATION
        Deposit in Parameter:0 the number of notes outstanding before silence
        in the queue of the channel specified in Parameter:0,
        including the current playing sound, if any.

