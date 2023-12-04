# *******************************************************************************************
# *******************************************************************************************
#
#		Name : 		teststring.py
#		Purpose :	Test string operations
#		Date :		26th May 2023
#		Author : 	Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

import os,sys,math,re,random
from testutils import *

bc = BinaryChecker()
bbc = BinaryBooleanChecker()
uc = UnaryChecker()

for i in range(0,40):
	s1 = TestString()
	s2 = TestString()
	if random.randint(0,4) == 0:
		s1 = s2

	bbc.generate(s1,"<",s2,s1.get() < s2.get())
	bbc.generate(s1,"<=",s2,s1.get() <= s2.get())
	bbc.generate(s1,"=",s2,s1.get() == s2.get())
	bbc.generate(s1,">",s2,s1.get() > s2.get())
	bbc.generate(s1,">=",s2,s1.get() >= s2.get())
	bbc.generate(s1,"<>",s2,s1.get() != s2.get())
	
	print('assert "{0}"+"{1}" = "{0}{1}"'.format(s1.get(),s2.get()))

	n1 = TestNumber(True)
	n1.set(random.randint(64,95))
	uc.generate(n1,"asc(chr$",n1.get())

	n1 = TestNumber()
	uc.generate(n1,"val(str$",n1.get(),0.5)

	n1 = TestNumber(True).absolute()
	uc.generate(n1,"dec(hex$",n1.get())	
print("sys 65535")	