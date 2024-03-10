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

GROUP 3 File I/O
    FUNCTION 1 List Directory
        FIODirectory("");
    DOCUMENTATION
        Display the file listing of the present directory.

    FUNCTION 2 Load File
        *DERROR = FIOReadFile(DSPGetStdString(DCOMMAND,4),DSPGetInt16(DCOMMAND,6),DCOMMAND);
    DOCUMENTATION
        Load a file by name into memory. On input:

        \begin{compactitem}
        \item \Param{0,1} points to the length-prefixed filename string;
        \item \Param{2,3} contains the location to write the data to. If the
        address is \MonoSp{$FFFF}, the file will instead be loaded into the
        graphics working memory, used for sprites, tiles, images.
        \end{compactitem}

        On output:

        \begin{compactitem}
        \item \Param{0} contains an error/status code.
        \end{compactitem}

    FUNCTION 3 Store File
        *DERROR = FIOWriteFile(DSPGetStdString(DCOMMAND,4),DSPGetInt16(DCOMMAND,6),DSPGetInt16(DCOMMAND,8));
    DOCUMENTATION
        Saves data in memory to a file. On input:

        \begin{compactitem}
        \item \Param{0,1} points to the length-prefixed filename string;
        \item \Param{2,3} contains the location to read data from;
        \item \Param{4,5} specified the number of bytes to store.
        \end{compactitem}

        On output:

        \begin{compactitem}
        \item \Param{0} contains an error/status code.
        \end{compactitem}

    FUNCTION 4 File Open
        *DERROR = FIOOpenFileHandle(DPARAMS[0],DSPGetStdString(DPARAMS,1),DPARAMS[3]);
    DOCUMENTATION
        Opens a file into a specific channel. On input:

        \begin{compactitem}
        \item \Param{0} contains the file channel to open;
        \item \Param{1,2} points to the length-prefixed filename string;
        \item \Param{3} contains the open mode. See below.
        \end{compactitem}

        Valid open modes are:

        \begin{compactitem}
        \item 0 opens the file for read-only access;
        \item 1 opens the file for write-only access;
        \item 2 opens the file for read-write access;
        \item 3 creates the file if it doesn't already exist, truncates it if it
        does, and opens the file for read-write access.
        \end{compactitem}

        Modes 0 to 2 will fail if the file does not already exist. If the
        channel is already open, the call fails. Opening the same file more than
        once on different channels has undefined behaviour, and is not
        recommended.

    FUNCTION 5 File Close
        *DERROR = FIOCloseFileHandle(DPARAMS[0]);
    DOCUMENTATION
        Closes a particular channel. On input:

        \begin{compactitem}
        \item \Param{0} contains the file channel to close.
        \end{compactitem}

    FUNCTION 6 File Seek
        *DERROR = FIOSeekFileHandle(DPARAMS[0],DSPGetInt32(DPARAMS,1));
    DOCUMENTATION
        Seeks the file opened on a particular channel to a location. On input:

        \begin{compactitem}
        \item \Param{0} contains the file channel to operate on;
        \item \Param{1,2,3,4} contains the file location.
        \end{compactitem}

        You can seek beyond the end of a file to extend the file.
        However, whether the file size changes when the seek happens,
        or when you perform the write is undefined behavior.

    FUNCTION 7 File Tell
    {
        uint32_t pos;
        *DERROR = FIOTellFileHandle(DPARAMS[0],&pos);
        DSPSetInt32(DPARAMS,1,pos);
    }
    DOCUMENTATION
        Returns the current seek location for the file opened on a particular channel. On input:

        \begin{compactitem}
        \item \Param{0} contains the file channel to operate on.
        \end{compactitem}

        On output:

        \begin{compactitem}
        \item \Param{1,2,3,4} contains the seek location within the file.
        \end{compactitem}

    FUNCTION 8 File Read
    {
        uint16_t size = DSPGetInt16(DPARAMS,3);
        *DERROR = FIOReadFileHandle(DPARAMS[0],DSPGetInt16(DPARAMS,1),&size);
        DSPSetInt16(DPARAMS,3,size);
    }
    DOCUMENTATION
        Reads data from an opened file. On input:

        \begin{compactitem}
        \item \Param{0} contains the file channel to operate on.
        \item \Param{1,2} points to the destination in memory,
              or \MonoSp{$FFFF} to read into graphics memory.
        \item \Param{2,3} contains the amount of data to read.
        \end{compactitem}

        On output:

        \begin{compactitem}
        \item \Param{3,4} is updated to contain the amount of data actually read.
        \end{compactitem}

        Data is read from the current seek position, which is advanced after the read.

    FUNCTION 9 File Write
    {
        uint16_t size = DSPGetInt16(DPARAMS,3);
        *DERROR = FIOWriteFileHandle(DPARAMS[0],DSPGetInt16(DPARAMS,1),&size);
        DSPSetInt16(DPARAMS,3,size);
    }
    DOCUMENTATION
        Writes data to an opened file. On input:

        \begin{compactitem}
        \item \Param{0} contains the file channel to operate on;
        \item \Param{1,2} points to the data in memory;
        \item \Param{3,4} contains the amount of data to write.
        \end{compactitem}

        On output:

        \begin{compactitem}
        \item \Param{3,4} is updated to contain the amount of data actually written.
        \end{compactitem}

        Data is written to the current seek position, which is advanced after the write.

    FUNCTION 10 File Size
    {
        uint32_t size;
        *DERROR = FIOGetSizeFileHandle(DPARAMS[0],&size);
        DSPSetInt32(DPARAMS,1,size);
    }
    DOCUMENTATION
        Returns the current size of an opened file. On input:

        \begin{compactitem}
        \item \Param{0} contains the file channel to operate on.
        \end{compactitem}

        On output:

        \begin{compactitem}
        \item \Param{1,2,3,4} contains the size of the file.
        \end{compactitem}

        This call should be used on open files,
        and takes into account any buffered data which has not yet been written to disk.
        Consequently, this may return a different size than Function 3,16 "File Stat".

    FUNCTION 11 File Set Size
    {
        *DERROR = FIOSetSizeFileHandle(DPARAMS[0],DSPGetInt32(DPARAMS,1));
    }
    DOCUMENTATION
        Extends or truncates an opened file to a particular size. On input:

        \begin{compactitem}
        \item \Param{0} contains the file channel to operate on;
        \item \Param{1,2,3,4} contains the new size of the file.
        \end{compactitem}

    FUNCTION 12 File Rename
    {
        *DERROR = FIORenameFile(DSPGetStdString(DPARAMS, 0), DSPGetStdString(DPARAMS, 2));
    }
    DOCUMENTATION
        Renames a file. On input:

        \begin{compactitem}
        \item \Param{0,1} points to the length-prefixed string for the old name;
        \item \Param{2,3} points to the length-prefixed string for the new name.
        \end{compactitem}

        Files may be renamed across directories.

    FUNCTION 13 File Delete
    {
        *DERROR = FIODeleteFile(DSPGetStdString(DPARAMS, 0));
    }
    DOCUMENTATION
        Deletes a file or directory. On input:

        \begin{compactitem}
        \item \Param{0,1} points to the length-prefixed filename string.
        \end{compactitem}

        Deleting a file which is open has undefined behaviour. Directories may
        only be deleted if they are empty.

    FUNCTION 14 Create Directory
    {
        *DERROR = FIOCreateDirectory(DSPGetStdString(DPARAMS, 0));
    }
    DOCUMENTATION
        Creates a new directory. On input:

        \begin{compactitem}
        \item \Param{0,1} points to the length-prefixed filename string.
        \end{compactitem}

    FUNCTION 15 Change Directory
        {
        *DERROR = FIOChangeDirectory(DSPGetStdString(DPARAMS, 0));
        }
    DOCUMENTATION
        Changes the current working directory. On input:

        \begin{compactitem}
        \item \Param{0,1} points to the length-prefixed path string.
        \end{compactitem}

    FUNCTION 16 File Stat
    {
        uint32_t length;
        uint8_t type;
        *DERROR = FIOStatFile(DSPGetStdString(DPARAMS, 0), &length, &type);
        DSPSetInt32(DPARAMS, 0, length);
        DPARAMS[4] = type;
    }
    DOCUMENTATION
        Retrieves information about a file by name. On input:

        \begin{compactitem}
        \item \Param{0,1} points to the length-prefixed filename string.
        \end{compactitem}

        On output:

        \begin{compactitem}
        \item \Param{0,1,2,3} contains the length of the file;
        \item \Param{4} contains the attributes bit-field of the file.
        \end{compactitem}

        If the file is open for writing, this may not return the correct size
        due to buffered data not having been flushed to disk.

        File attributes are a bitfield as follows:

        \newline
        \ParamBits{File attributes}{0}{0}{0}{Hidden}{Read only}{Archive}{System}{Directory}

    FUNCTION 17 Open Directory
    {
        *DERROR = FIOOpenDir(DSPGetStdString(DPARAMS, 0));
    }
    DOCUMENTATION
        Opens a directory for enumeration. On input:

        \begin{compactitem}
        \item \Param{0,1} points to the length-prefixed filename string.
        \end{compactitem}

        Only one directory at a time may be opened. If a directory is already
        open when this call is made, it is automatically closed. However, an
        open directory may make it impossible to delete the directory; so
        closing the directory after use is good practice.

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
        Reads an item from the currently open directory. On input:

        \begin{compactitem}
        \item \Param{0,1} points to a length-prefixed buffer for returning the filename.
        \end{compactitem}

        \begin{compactitem}
        \item \Param{0,1} is unchanged, but the buffer is updated to contain the
        length-prefixed filename (without any leading path);
        \item \Param{2,3,4,5} contains the length of the file;
        \item \Param{6} contains the file attributes, as described by Function 3,16 "File Stat".
        \end{compactitem}

        If there are no more items to read, this call fails and an error is flagged.

    FUNCTION 19 Close Directory
        {
            *DERROR = FIOCloseDir();
        }
    DOCUMENTATION
        Closes any directory opened previously by Function 3,17 "Open Directory".

    FUNCTION 20 Copy File
        *DERROR = FIOCopyFile(DSPGetStdString(DPARAMS, 0), DSPGetStdString(DPARAMS, 2));
    DOCUMENTATION
        Copies a file. On input:

        \begin{compactitem}
        \item \Param{0,1} points to the length-prefixed old filename;
        \item \Param{2,3} points to the length-prefixed new filename.
        \end{compactitem}

        Only single files may be copied, not directories.

    FUNCTION 21 Set file attributes
        *DERROR = FIOSetFileAttributes(DSPGetStdString(DPARAMS, 0), DPARAMS[2]);
    DOCUMENTATION
        Sets the attributes for a file. On input:

        \begin{compactitem}
        \item \Param{0,1} points to the length-prefixed filename;
        \item \Param{2} is the attribute bitfield. (See Stat File for details.)
        \end{compactitem}

        The directory bit cannot be changed. Obviously.

    FUNCTION 32 List Filtered
        FIODirectory(DSPGetString(DCOMMAND,4));
    DOCUMENTATION
        Prints a filtered file listing of the current directory to the console. On input:

        \begin{compactitem}
        \item \Param{0,1} points to the filename search string.
        \end{compactitem}

        Files will only be shown if the name contains the search string (ie: a
        substring match).
