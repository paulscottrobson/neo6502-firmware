#
#		Where executables are
#
BINDIR=..
#
#		Convert graphics files and put in storage (which is the SD Card/USB stick in the emulator.)
#		This takes the 3 png files and creates graphics.gfx
# 
python $BINDIR/makeimg.zip
cp graphics.gfx storage
#
#		Convert source.bsc to a tokenised program
#
python3 $BINDIR/makebasic.zip source.bsc -oprogram.bas
#
#		Run it in the emulator. Loads program.bas in to 'page' (where program code goes)
#		Then start the interpreter and runs the program (warm just starts it)
#
$BINDIR/neo	program.bas@page exec
