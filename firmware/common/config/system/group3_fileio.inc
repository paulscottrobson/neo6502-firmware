// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      group3_fileio.inc (paul@robsons.org.uk)
//      Date :      22nd November 2023
//   Authors :      Paul Robson (paul@robsons.org.uk)
//                  Bill Auger
//                  David Given
//      Reviewed :  No
//      Purpose :   File I/O Group dispatcher.
//
// ***************************************************************************************
// ***************************************************************************************

// ***************************************************************************************
//
//                                  Group 3 (File I/O)
//
// ***************************************************************************************

GROUP 3 ,0,9 File I/O
    FUNCTION 1 List Directory
        FIODirectory("");
    DOCUMENTATION
        Display the file listing of the present directory.

    FUNCTION 2 Load File
        *DERROR = FIOReadFile(DSPGetStdString(DCOMMAND,4),DSPGetInt16(DCOMMAND,6),DCOMMAND);
    DOCUMENTATION
        Load a file by name, per the length-prefixed string stored at the memory location
        specified in \Param{0,1} into to the memory location specified in \Param{2,3}.
        When the command completes, an error/status code will be in \Param{0}.
        If the address in \Param{2,3} is \MonoSp{$FFFF}, the file will be loaded into the
        graphics working memory, used for sprites, tiles, images.

    FUNCTION 3 Store File
        *DERROR = FIOWriteFile(DSPGetStdString(DCOMMAND,4),DSPGetInt16(DCOMMAND,6),DSPGetInt16(DCOMMAND,8));
    DOCUMENTATION
        Save the data stored at the memory location specified in \Param{2,3} to storage,
        as a file with the specified filename, per the length-prefixed string
        stored at the memory location specified in \Param{0,1}.
        \Param{4,5} specify the number of bytes to store.
        When the command completes, an error/status code will be in \Param{0}.

    FUNCTION 4 File Open
        *DERROR = FIOOpenFileHandle(DPARAMS[0],DSPGetStdString(DPARAMS,1),DPARAMS[3]);
    DOCUMENTATION
        File Open
        \newline TODO: explain this function and any parameters

    FUNCTION 5 File Close
        *DERROR = FIOCloseFileHandle(DPARAMS[0]);
    DOCUMENTATION
        File Close
        \newline TODO: explain this function and any parameters

    FUNCTION 6 File Seek
        *DERROR = FIOSeekFileHandle(DPARAMS[0],DSPGetInt32(DPARAMS,1));
    DOCUMENTATION
        File Seek
        \newline TODO: explain this function and any parameters

    FUNCTION 7 File Tell
    {
        uint32_t pos;
        *DERROR = FIOTellFileHandle(DPARAMS[0],&pos);
        DSPSetInt32(DPARAMS,1,pos);
    }
    DOCUMENTATION
        Deposits the current seek position (byte-offset) into \Param{4}.

    FUNCTION 8 File Read
    {
        uint16_t size = DSPGetInt16(DPARAMS,3);
        *DERROR = FIOReadFileHandle(DPARAMS[0],DSPGetInt16(DPARAMS,1),&size);
        DSPSetInt16(DPARAMS,3,size);
    }
    DOCUMENTATION
        File Read
        \newline TODO: explain this function and any parameters

    FUNCTION 9 File Write
    {
        uint16_t size = DSPGetInt16(DPARAMS,3);
        *DERROR = FIOWriteFileHandle(DPARAMS[0],DSPGetInt16(DPARAMS,1),&size);
        DSPSetInt16(DPARAMS,3,size);
    }
    DOCUMENTATION
        File Write
        \newline TODO: explain this function and any parameters

    FUNCTION 10 File Size
    {
        uint32_t size;
        *DERROR = FIOGetSizeFileHandle(DPARAMS[0],&size);
        DSPSetInt32(DPARAMS,1,size);
    }
    DOCUMENTATION
        File Size
        \newline TODO: explain this function and any parameters

    FUNCTION 11 File Set Size
    {
        *DERROR = FIOSetSizeFileHandle(DPARAMS[0],DSPGetInt32(DPARAMS,1));
    }
    DOCUMENTATION
        File Set Size
        \newline TODO: explain this function and any parameters

    FUNCTION 12 File Rename
    {
        *DERROR = FIORenameFile(DSPGetStdString(DPARAMS, 0), DSPGetStdString(DPARAMS, 2));
    }
    DOCUMENTATION
        File Rename
        \newline TODO: explain this function and any parameters

    FUNCTION 13 Delete File
    {
        *DERROR = FIODeleteFile(DSPGetStdString(DPARAMS, 0));
    }
    DOCUMENTATION
        Delete File
        \newline TODO: explain this function and any parameters

    FUNCTION 14 Create Directory
    {
        *DERROR = FIOCreateDirectory(DSPGetStdString(DPARAMS, 0));
    }
    DOCUMENTATION
        Create Directory
        \newline TODO: explain this function and any parameters

    FUNCTION 15 Change Directory
        {
        *DERROR = FIOChangeDirectory(DSPGetStdString(DPARAMS, 0));
        }
    DOCUMENTATION
        Change Directory
        \newline TODO: explain this function and any parameters

    FUNCTION 16 Stat File
    {
        uint32_t length;
        uint8_t type;
        *DERROR = FIOStatFile(DSPGetStdString(DPARAMS, 0), &length, &type);
        DSPSetInt32(DPARAMS, 0, length);
        DPARAMS[4] = type;
    }
    DOCUMENTATION
        Stat File
        \newline TODO: explain this function and any parameters

    FUNCTION 17 Open Directory
    {
        *DERROR = FIOOpenDir(DSPGetStdString(DPARAMS, 0));
    }
    DOCUMENTATION
        Open Directory
        \newline TODO: explain this function and any parameters

    FUNCTION 18 Read Directory
        {
        std::string filename;
        uint32_t size;
        uint8_t attribs;
        *DERROR = FIOReadDir(filename, &size, &attribs);
        DSPSetStdString(DPARAMS, 0, filename);
        DSPSetInt32(DPARAMS, 2, size);
        DPARAMS[6] = attribs;
        }
    DOCUMENTATION
        Read Directory
        \newline TODO: explain this function and any parameters

    FUNCTION 19 Close Directory
        {
            *DERROR = FIOCloseDir();
        }
    DOCUMENTATION
        Close Directory
        \newline TODO: explain this function and any parameters

    FUNCTION 20 Copy File
        *DERROR = FIOCopyFile(DSPGetStdString(DPARAMS, 0), DSPGetStdString(DPARAMS, 2));
    DOCUMENTATION
        Copy File
        \newline TODO: explain this function and any parameters

    FUNCTION 32 List Filtered
        FIODirectory(DSPGetString(DCOMMAND,4));
    DOCUMENTATION
        Display the file listing of the present directory,
        filtered per the length-prefixed string
        stored at the memory location specified in \Param{0,1}.

