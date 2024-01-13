IMPSRC="../import/sources/interface"
CORESRC="../src/core"
FRAMESRC="../framework"
SRC1="$CORESRC/sys_processor.cpp  $FRAMESRC/main.cpp $FRAMESRC/gfx.cpp $FRAMESRC/debugger.cpp"
SRC2="$CORESRC/sys_debugger.cpp $CORESRC/hardware.cpp"
SRC3="$IMPSRC/memory.cpp $IMPSRC/graphics.cpp $IMPSRC/console.cpp $IMPSRC/keyboard.cpp $IMPSRC/dispatch.cpp"
SRC4="$IMPSRC/maths.cpp  $IMPSRC/config.cpp $IMPSRC/gfxcommands.cpp $IMPSRC/efla.cpp $IMPSRC/ellipse.cpp"
SRC5="$IMPSRC/fileinterface.cpp $IMPSRC/sprites.cpp $IMPSRC/logo.cpp $IMPSRC/sprites_xor.cpp"
SRC6="$IMPSRC/sndmanager.cpp $IMPSRC/sfxmanager.cpp"

SOURCES="$SRC1 $SRC2 $SRC3 $SRC4 $SRC5 $SRC6"
INCLUDES="-I../include -I../import/include -I../framework -I.."

x86_64-w64-mingw32-g++ -Wall -D IBMPC -D INCLUDE_DEBUGGING_SUPPORT -o neo.exe $SOURCES $INCLUDES `x86_64-w64-mingw32/bin/sdl2-config --cflags --static-libs` -static-libgcc -static-libstdc++ 

rm neowin.zip
zip neowin.zip neo.exe *.dll

