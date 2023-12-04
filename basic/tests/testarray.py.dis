# *******************************************************************************************
# *******************************************************************************************
#
#		Name : 		testarray.py
#		Purpose :	Array testing script
#		Date :		1st July 2023
#		Author : 	Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

import os,sys,math,re,random
from testutils import *

class Array(object):
	def __init__(self):
		self.name = ""
		for i in range(0,random.randint(1,5)):
			if (i & 1) == 0:
				self.name += chr(random.randint(0,25)+97)
			else:
				self.name += chr(random.randint(0,9)+48)
		self.isString = random.randint(0,1) == 0
		if self.isString:
			self.name += "$"
		if random.randint(0,1) == 0:
			self.size = [random.randint(1,8)]
		else:
			self.size = [random.randint(1,5),random.randint(1,5)]
		self.elements = {}
	#
	def getName(self):
		return self.name
	def getDimension(self):
		print("dim {0}({1})".format(self.name,",".join([str(x) for x in self.size])))
	#
	def assign(self):
		x = [random.randint(0,i) for i in self.size]
		m = self.mapping(x)
		self.elements[m] = TestString() if self.isString else TestNumber()
		print("{0}({1}) = {2}".format(self.name,",".join([str(i) for i in x]),self.elements[m].render()))
	#
	def mapping(self,x):
		if len(x) == 1:
			return x[0]
		else:
			return x[0] * 100 + x[1]
	#
	def check(self):
		if len(self.size) == 1:
			for n1 in range(0,self.size[0]+1):
				self.validate([n1])
		else:
			for n1 in range(0,self.size[0]+1):
				for n2 in range(0,self.size[1]+1):
					self.validate([n1,n2])

	#
	def validate(self,x):
		m = self.mapping(x)
		if m in self.elements:
			print("assert {0}({1}) = {2}".format(self.name,",".join([str(i) for i in x]),self.elements[m].render()))

arrayList = []
arrayNames = {}
arrayCount = 20
while len(arrayList) < arrayCount:
	na = Array()			
	if na.getName() not in arrayNames:
		arrayList.append(na)
		arrayNames[na.getName()] = True

for a in arrayList:
	a.getDimension()		
for i in range(0,len(arrayList)*10):
	arrayList[random.randint(0,len(arrayList)-1)].assign()
for a in arrayList:
	a.check()
print("sys 65535")	