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

def compare(start,size):
	global errors
	for i in range(0,size):
		if ram[i+base+start] != reqd[i+start]:
			print("\tAt ${2:04x} Program ${0:02x} {0:3} should be ${1:02x} {1:3}".format(ram[i+base+start],reqd[i+start],i+start))
			errors = errors + 1

errors = 0
ram = [x for x in open("memory.dump","rb").read(-1)]
reqd = [x for x in open("build/token_test.dat","rb").read(-1)]
base = ram[0] << 8
vPages = ram[base]


print("Variable pages {0}".format(vPages))
eVar = 1
while reqd[eVar] != 0:
	eVar += reqd[eVar]
print("Comparing variables $0000-${0:04x}".format(eVar+1))
compare(0,eVar+1)
print("Comparing tokenised code ${0:04x}-${1:04x}".format(vPages*256,len(reqd)))
compare(vPages * 256,len(reqd) - vPages * 256)
if errors != 0:
	print("**** Errors {0} ****".format(errors))
	sys.exit(-1)
print("Successful.")

