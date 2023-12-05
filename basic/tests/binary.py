# *******************************************************************************************
# *******************************************************************************************
#
#		Name : 		binary.py
#		Purpose :	Test binary functions
#		Date :		4th December 2023
#		Author : 	Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

import os,sys,math,re,random
from testutils import *

bc = BinaryChecker()
bbc = BinaryBooleanChecker()
for i in range(0,20):
	n1 = TestNumber()
	n2 = TestNumber()
	bc.generate(n1,"+",n2,n1.get()+n2.get())
	bc.generate(n1,"-",n2,n1.get()-n2.get())
	bc.generate(n1,"*",n2,n1.get()*n2.get(),1)
	if n2.get() != 0:
	 	bc.generate(n1,"/",n2,n1.get()/n2.get(),2)
	if abs(n1.get()-n2.get()) < 0.001 or random.randint(0,4) == 0:
	 	n1 = n2
	bbc.generate(n1,"<",n2,n1.get() < n2.get())
	bbc.generate(n1,"<=",n2,n1.get() <= n2.get())
	bbc.generate(n1,"=",n2,n1.get() == n2.get())
	bbc.generate(n1,">",n2,n1.get() > n2.get())
	bbc.generate(n1,">=",n2,n1.get() >= n2.get())
	bbc.generate(n1,"<>",n2,n1.get() != n2.get())
	
	i1 = TestNumber(True).absolute()
	i2 = TestNumber(True).absolute()
	bc.generate(i1," & ",i2,i1.get() & i2.get())
	bc.generate(i1," | ",i2,i1.get() | i2.get())
	bc.generate(i1," ^ ",i2,i1.get() ^ i2.get())

	i2.set(random.randint(0,13))
	bc.generate(i1," >> ",i2,i1.get() >> i2.get())
	if (i1.get() << i2.get()) < 0x400000:
		bc.generate(i1," << ",i2,i1.get() << i2.get())

print("sys 65535")	

