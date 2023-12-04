# *******************************************************************************************
# *******************************************************************************************
#
#		Name : 		asmcheck.py
#		Purpose :	Check assembled code matches.
#		Date :		7th July 2023
#		Author : 	Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

import sys 

codeAt = 0x6000

memory = [x for x in open("../../memory.dump","rb").read(-1)]
asbin =  [x for x in open("asmtest.bin","rb").read(-1)]

count= 0
for x in range(0,len(asbin)):
	if memory[x+codeAt] != asbin[x]:
		print("${0:04x} BAS:${1:02x} TASS:${2:02x}".format(x+codeAt,memory[x+codeAt],asbin[x]))
		count += 1
if count == 0:
	print("All opcodes assembled correctly.")
sys.exit(0 if count == 0 else 1)