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

b = [x for x in open(sys.argv[1],"rb").read(-1)]								# Read in file

print("//\n//\tThis file is automatically created\n//") 						# Output header
print("const uint8_t kernel_bin[] = {")
print(",".join([str(x) for x in b])) 											# Output data
print("};\n")

print("#define KERNEL_LOAD (0x{0:04x})".format(0x10000-len(b)))					# Load and Size of file.
print("#define KERNEL_SIZE (0x{0:04x})".format(len(b)))