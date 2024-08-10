# *******************************************************************************************
# *******************************************************************************************
#
#		Name : 		listbasic.py
#		Purpose :	List a BASIC program
#		Date :		2nd January 2024
#		Author : 	Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

from tokens import *
import re,sys 

# *******************************************************************************************
#
#								Program Listing Class
#
# *******************************************************************************************

class ProgramLister(object):
	def __init__(self,fileName,showLineNumbers):
		self.bin = bytearray(open(fileName,"rb").read(-1))
		self.ts = TokenSet()
		self.dollar = self.ts.getByName("$").getID()
		self.showLineNumbers = showLineNumbers

	def list(self):
		code = self.bin[0] << 8 
		while self.bin[code] != 0:
			self.listLine(code)
			code += self.bin[code]

	def listLine(self,lineStart):
		self.text = "{0} ".format(self.bin[lineStart+1]+self.bin[lineStart+2] * 256) if self.showLineNumbers else ""
		p = lineStart + 3
		while self.bin[p] != self.ts.getByName("!!END").getID():
			p = self.listOneElement(p)
		print(self.text)

	def listOneElement(self,p):
		n = self.bin[p]

		if n >= 0x00 and n < 0x20:
			va = (n << 8)+self.bin[p+1]+5
			p += 2
			name = ""
			while self.bin[va] < 0x80:
				name += chr(self.bin[va])
				va += 1
			name += chr(self.bin[va] & 0x7F)
			self.append(name.lower())

		elif n == self.ts.getByName("!!STR").getID():
			self.append('"'+"".join([chr(self.bin[p+2+c]) for c in range(0,self.bin[p+1])])+'"')
			p = p + self.bin[p+1] + 2

		elif n == self.ts.getByName("!!DEC").getID():
			self.append('.'+"".join([self.decode(self.bin[p+2+c]) for c in range(0,self.bin[p+1])]))
			p = p + self.bin[p+1] + 2

		elif n >= 0x40 and n < 0x80:
			v = 0 
			while self.bin[p] >= 0x40 and self.bin[p] < 0x80:
				v = (v << 6) + self.bin[p] - 0x40
				p += 1
			self.append(str(v))

		elif n == self.dollar:
			self.append(" $")
			p += 1
			v = 0 
			while self.bin[p] >= 0x40 and self.bin[p] < 0x80:
				v = (v << 6) + self.bin[p] - 0x40
				p += 1
			self.append("{0:x}".format(v))

		elif n >= 0x80 or (n >= 0x20 and n < 0x40):
			p += 1
			if n == self.ts.getByName("!!SH1").getID():
				n = self.bin[p] + 0x100
				p += 1
			if n == self.ts.getByName("!!SH2").getID():
				n = self.bin[p] + 0x200
				p += 1
			s = self.ts.getByID(n).getName()
			self.append(s)

		else:
			self.text += "[{0:02x}]".format(self.bin[p])
			p += 1

		return p

	def append(self,s):
		if self.text != "" and self.type(self.text[-1]) == self.type(s[0]):
			self.text += " "
		self.text += s

	def type(self,c):
		c = c.upper()
		if c != ' ':
			c = 'I' if (c >= '0' and c <= '9') or (c >= 'A' and c <= 'Z') or c == '_' else 'N'
		return c
	def decode(self,d):
		return "{0}{1}".format("" if (d & 240) == 240 else d >> 4,"" if (d & 15) == 15 else (d & 15))

showLineNumbers = True
if len(sys.argv) == 1:
	print("listbasic.zip <options> <tokenised files>")
	print("\t-l\t\tSuppress line numbers.")

for f in sys.argv[1:]:
	if f == "-l":
		showLineNumbers = False
	else:
		ProgramLister(f,showLineNumbers).list()
