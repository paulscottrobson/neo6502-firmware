# ***************************************************************************************
# ***************************************************************************************
#
#		Name : 		hconvert.py
#		Author :	Paul Robson (paul@robsons.org.uk)
#		Date : 		20th November 2023
#		Reviewed :	No
#		Purpose :	Converts a binary file to a loadable hex file.
#
# ***************************************************************************************
# ***************************************************************************************

import sys
b = [x for x in open(sys.argv[1],"rb").read(-1)]

print("const uint8_t test_bin[] = {")
print(",".join([str(x) for x in b]))
print("};\n")

print("#define TESTLOAD (0x{0:04x})".format(0xFC00))
print("#define TESTSIZE (0x{0:04x})".format(len(b)))