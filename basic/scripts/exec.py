# *******************************************************************************************
# *******************************************************************************************
#
#		Name : 		exec.py
#		Purpose :	Build (or list) an executable
#		Date :		18th February 2024
#		Author : 	Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

import os,re,sys 

from makeexec import *

try:
	xFile = Executable() 											# new executable
	assert len(sys.argv) > 1,"No parameters" 

	for s in sys.argv[1:]:
		if s == "exec": 											# exec = load BASIC program
			xFile.execBasic()
		elif s.startswith("-o"): 									# -o <filename>
			xFile.writeFile(s[2:])
		elif s.startswith("-d"):  									# -d <filename>
			xFile.dumpFile(s[2:])
		else:
			s = s.split("@") 										# then <file>@<location>
			assert len(s) == 2,"Bad command "+s 
			if s[1] == "page":  									# handle <file>@page
				xFile.addBasic(s[0])
			else:
				assert re.match("^[0-9A-Fa-f]+$",s[1]) is not None,"Bad address "+s[1]
				address = int(s[1],16)  							# hex address
				if s[0] == "run": 									# handle run@<address>
					xFile.execFrom(address)
				elif address == 0xFFFF:  							# handle <file>@FFFF
					xFile.addGraphicsObjectFile(s[0])
				else:  												# handle <file>@<location>
					xFile.addFile(s[0],address)
	sys.exit(0)

except Exception as err:
	print("Error : ",err)
	print("python exec.zip <command list> -o<outputfile> builds a file")
	print("\t<file>@page\t\tLoads BASIC program")
	print("\t<file>@ffff\t\tLoads Graphics Object file")
	print("\t<file>@<hex address>\tLoads arbitrary file")
	print("\trun@<hex address>\tSets the executable address")
	print("\texec\t\t\truns BASIC program")
	print("python exec.zip -d<file> dumps a file")
	sys.exit(-1)
	