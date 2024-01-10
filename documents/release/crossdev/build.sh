#
#		Where executables are
#
BINDIR=../../../bin
#
#		Convert source.bsc to a tokenised program
#
python3 $BINDIR/makebasic.zip source.bsc -oprogram.bas
#
#		Run it in the emulator. Loads program.bas in to 'page' (where program code goes)
#		Then warm starts the interpreter. You can use exec to auto run it.
#
$BINDIR/neo	program.bas@page warm
