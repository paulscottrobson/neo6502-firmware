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

def convert(h,srcFile,targetName,loadAddress):
	b = bytearray(open(srcFile,"rb").read(-1))									# Read in file
	if loadAddress < 0:
		loadAddress = 0x10000-len(b)
	h.write("//\n//\tThis file is automatically created\n//\n") 				# Output header
	h.write("const uint8_t {0}_bin[] = {{\n".format(targetName.lower()))
	h.write(",".join([str(x) for x in b])) 										# Output data
	h.write("\n};\n")

	u = targetName.upper()
	h.write("#define {1}_LOAD (0x{0:04x})\n".format(loadAddress,u))				# Load and Size of file.
	h.write("#define {1}_SIZE (0x{0:04x})\n\n".format(len(b),u))

h = open(sys.argv[2],"w")
convert(h,sys.argv[1],sys.argv[3],int(sys.argv[4],16) if len(sys.argv) == 5 else -1)
h.close()