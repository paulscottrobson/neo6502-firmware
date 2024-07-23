# *******************************************************************************************
# *******************************************************************************************
#
#		Name : 		makebasic.py
#		Purpose :	Build a BASIC program
#		Date :		24th November 2023
#		Author : 	Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

from tokeniser import *
from tokens import *
import re,sys 

# *******************************************************************************************
#
#									Program Builder Class
#
# *******************************************************************************************

class Program(object):
	def __init__(self):
		self.nextLine = 100
		self.lineStep = 10
		self.code = []
		self.store = IdentifierStore()
		self.store.add("A")
		self.store.add("O")
		self.store.add("P")
		self.store.add("X")
		self.store.add("Y")
		self.ts = TokenSet()
		self.tw = Tokeniser(self.store)
		self.identifiers = {}
		self.libraryMode = False
	#
	#		Add the contents of the file, stripping // comments
	#
	def addFile(self,fileName):
		for s in open(fileName).readlines():
			s = s.strip()
			if s.startswith("#"):
				self.command(s[1:])
				s = ""
			s = s.strip()
			s = self.processIdentifiers(s)
			if s != "":
				number = None 
				if s[0] >= "0" and s[0] <= "9":
					m = re.match("^(\\d+)(.*)$",s)
					number = int(m.group(1))
					s = m.group(2)
				self.addLine(number,s)
	#
	#		Handle #commands.
	#
	def command(self,c):
		if c.startswith("define"):
			c = c[6:].strip()
			n = c.find(" ")
			self.identifiers[c[:n]] = c[n+1:].strip()
		elif c == "library":
			self.libraryMode = True
		elif c == "nolibrary":
			self.libraryMode = False
			self.nextLine = 1000
		else:
			assert False,"Bad #command '#"+c+"'"

	#
	#		Process identifiers, making define substitutions.
	#				
	def processIdentifiers(self,s):
		src = re.split("([A-Za-z][A-Za-z0-9\\._]*)",s)
		for i in range(0,len(src)):
			if src[i] in self.identifiers:
				src[i] = self.identifiers[src[i]]				
		return "".join(src)
	#
	#		Add a line with an optional line number
	#
	def addLine(self,number = None,text = ""):
		if text.strip() != "":
			if number is not None:
				self.nextLine = number
			lineNo = 0 if self.libraryMode else self.nextLine
			#print(lineNo,text)
			line = [0,lineNo & 0xFF,lineNo >> 8]
			line += self.tw.tokenise(text)
			line.append(self.ts.getByName("!!end").getID())
			line[0] = len(line)
			#print(line)
			self.code += line
			self.nextLine += self.lineStep

		return self
	#
	#		Make program a library
	#
	def makelibrary(self):
		p = 0
		while p != len(self.code):
			self.code[p+1] = 0
			self.code[p+2] = 0
			p += self.code[p]
	#
	#		Save the resulting tokenised code.
	#
	def render(self,fileName):
		h = open(output,"wb")
		h.write(bytes(self.store.render()+self.code+[0]))
		h.close()

output = "basic.tok"
prog = Program()
for f in sys.argv[1:]:
	if f.startswith("-o"):
		output = f[2:]
	elif f == "library":
		prog.makelibrary()
	else:
		prog.addFile(f)
prog.render(output)