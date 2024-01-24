# *******************************************************************************************
# *******************************************************************************************
#
#		Name : 		unary.py
#		Purpose :	Test unary functions
#		Date :		4th December 2023
#		Author : 	Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

import os,sys,math,re,random
from testutils import *

def sign(n):
	if n == 0:
		return 0 
	return -1 if n < 0 else 1

uc = UnaryChecker()
for i in range(0,40):
	n1 = TestNumber()
	s1 = TestString()
	uc.generate(n1,"abs",abs(n1.get()))
	uc.generate(s1,"len",len(s1.get()))
	uc.generate(n1,"sgn",sign(n1.get()))
	fn1 = abs(n1.get())
	in1 = math.floor(n1.get())
	uc.generate(n1,"int",in1)
	n1 = TestNumber().absolute()
	uc.generate(n1,"sqr",math.sqrt(n1.get()),0.8)
	uc.generate(s1,"asc",ord(s1.get()[0]) if len(s1.get()) != 0 else 0)
	sc1 = TestNumberString()
	uc.generate(sc1,"val",sc1.get())

	s1 = s1.get()
	n2 = random.randint(0,len(s1)+1)
	n3 = random.randint(0,len(s1)+1)
	print('assert left$("{0}",{1}) = "{2}"'.format(s1,n2,s1[:n2]))
	print('assert right$("{0}",{1}) = "{2}"'.format(s1,n2,s1[max(0,len(s1)-n2):]))
	if n2 > 0:
		print('assert mid$("{0}",{1}) = "{2}"'.format(s1,n2,s1[n2-1:]))
		print('assert mid$("{0}",{1},{2}) = "{3}"'.format(s1,n2,n3,s1[n2-1:n2-1+n3]))
		
print("sys 65535")	