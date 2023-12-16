# *******************************************************************************************
# *******************************************************************************************
#
#		Name : 		testprogram.py
#		Purpose :	Compare test program results.
#		Date :		16th December 2023
#		Author : 	Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

import os,sys,math,re,random

ram = [x for x in open("memory.dump","rb").read(-1)]
reqd = [x for x in open("build/token_test.dat","rb").read(-1)]
base = ram[0] << 8

errors = 0
for i in range(0,len(reqd)):
	if ram[i+base] != reqd[i]:
		print("Program ${0:02x} {0:3} should be ${1:02x} {1:3}".format(ram[i+base],reqd[i]))
		errors += 1

assert errors == 0,"Errors "+str(errors)

