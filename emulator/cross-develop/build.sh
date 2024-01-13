IMPSRC="../import/sources/interface"
CORESRC="../src/core"
FRAMESRC="../framework"
SRC1="$CORESRC/sys_processor.cpp  $FRAMESRC/main.cpp $FRAMESRC/gfx.cpp $FRAMESRC/debugger.cpp"
SRC2="$CORESRC/sys_debugger.cpp $CORESRC/hardware.cpp"
SRC3="$IMPSRC/memory.c $IMPSRC/graphics.c $IMPSRC/console.c $IMPSRC/keyboard.c $IMPSRC/dispatch.c"
SRC4="$IMPSRC/maths.c  $IMPSRC/config.c $IMPSRC/gfxcommands.c $IMPSRC/efla.c $IMPSRC/ellipse.c"
SRC5="$IMPSRC/fileinterface.c $IMPSRC/sprites.c $IMPSRC/logo.c $IMPSRC/sprites_xor.c"
SRC6="$IMPSRC/sndmanager.c $IMPSRC/sfxmanager.c"

SOURCES="$SRC1 $SRC2 $SRC3 $SRC4 $SRC5 $SRC6"
INCLUDES="-I../include -I../import/include -I../framework -I.."

x86_64-w64-mingw32-g++ -Wall -D IBMPC -D INCLUDE_DEBUGGING_SUPPORT -o neo.exe $SOURCES $INCLUDES `x86_64-w64-mingw32/bin/sdl2-config --cflags --static-libs` -static-libgcc -static-libstdc++ 