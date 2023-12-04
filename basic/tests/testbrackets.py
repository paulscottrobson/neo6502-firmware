# *******************************************************************************************
# *******************************************************************************************
#
#		Name : 		testbrackets.py
#		Purpose :	Test parenthesis and precdence
#		Date :		26th May 2023
#		Author : 	Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

import os,sys,math,re,random
from testutils import *

# *******************************************************************************************
#
#								 Expression creator
#
# *******************************************************************************************

def CreateExpression(level):
	if level == 0:
		return str(random.randint(1,5))
	else:
		expr = ""
		for i in range(0,random.randint(2,4)):
			if i > 0:
				expr += ("+" if random.randint(0,1) == 0 else "*")
			expr += CreateExpression(level-1)
		return expr if random.randint(0,1) == 0 else "("+expr+")"

for i in range(0,140):
	x = CreateExpression(3)
	v = eval(x)
	if v < 0x800000:
		print("assert (({0})-{1}) = 0".format(x,eval(x)))
print("sys 65535")	