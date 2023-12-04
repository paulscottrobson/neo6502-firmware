# *******************************************************************************************
# *******************************************************************************************
#
#		Name : 		testutils.py
#		Purpose :	Test utility class
#		Date :		4th December 2023
#		Author : 	Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

import os,sys,math,re,random

class TestNumber(object):
	def __init__(self,forceInteger = False):
		self.isInteger = (random.randint(0,1) == 0) or forceInteger
		if self.isInteger:
			self.number = random.randint(-1000,1000)
		else:
			self.number = random.randint(-100000,100000) / 100
	def absolute(self):
		self.number = abs(self.number)
		return self
	def get(self):
		return self.number 
	def set(self,n):
		self.number = n
	def render(self):
		return str(self.number) if self.isInteger else str(round(self.number,2))

class TestString(object):
	def __init__(self):
		self.text = "".join([chr(random.randint(65,90)) for x in range(0,random.randint(0,10))])
	def get(self):
		return self.text
	def render(self):
		return '"'+self.text+'"'

class TestHexNumber(TestNumber):
	def render(self):
		return '"{0:x}"'.format(self.get())

class TestNumberString(TestNumber):
	def render(self):
		return '"'+TestNumber.render(self)+'"'

class BinaryChecker(object):
	def generate(self,n1,op,n2,result,errorPC = 0.0002):
		expr = "({0}{1}{2})".format(n1.render(),op,n2.render())
		rerror = round(abs(result/100*errorPC),3)
		if errorPC != 0 and rerror == 0:
			rerror = 0.001
		result = "abs({0}-{1}) <= {2}".format(expr,round(result,4),rerror)
		print("assert "+result)

class BinaryBooleanChecker(BinaryChecker):
	def generate(self,n1,op,n2,result,errorPC = 0.0001):
		result = -1 if result else 0
		BinaryChecker.generate(self,n1,op,n2,result,errorPC)

class UnaryChecker(object):
	def generate(self,n1,op,result,errorPC = 0.0001):
		expr = "({1}({0}){2})".format(n1.render(),op,")" if op.find("(") > 0 else "")
		rerror = round(abs(result/100*errorPC),3)
		if errorPC != 0 and rerror == 0:
			rerror = 0.001
		result = "abs({0}-{1}) <= {2}".format(expr,round(result,4),rerror)
		print("assert "+result)

random.seed()
