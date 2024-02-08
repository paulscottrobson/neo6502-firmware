# *******************************************************************************************
# *******************************************************************************************
#
#		Name : 		tokeniser.py
#		Purpose :	Tokeniser worker
#		Date :		24th November 2023
#		Author : 	Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

from tokens import *
import re

# *******************************************************************************************
#
#							Identifier storage class
#
# *******************************************************************************************

class IdentifierStore(object):
	def __init__(self):
		self.store = [ 0x1 ] 																# one page.
		self.identifiers = {}
	#
	def render(self):
		reqLength = self.store[0] * 256  													# Actual size
		render = [x for x in self.store] 													# make a copy
		while len(render) < reqLength:  													# Pad out with zeor.
			render.append(0)
		return render
	#
	def get(self,name):
		name = name.upper()
		return self.identifiers[name] if name in self.identifiers else None
	#
	def add(self,name):
		name = name.upper()
		bAddress = len(self.store)
		assert name not in self.identifiers
		self.identifiers[name] = len(self.store)+1 											# Points to data, offset 1 in record.
		isString = name.endswith("$") or name.endswith("$(") 								# Work out type byte
		isArray = name.endswith("(")
		self.store.append(len(name)+6)														# Offset byte
		self.store += [0,0,0,0] 															# default value
		ctrl = 0x80 if isString else 0x00
		ctrl = ctrl + 0x10 if isArray else ctrl
		self.store.append(ctrl) 															# control byte.
		b = [ord(x) for x in name] 															# work out name
		b[-1] |= 0x80
		self.store += b																		# name
		aAddress = len(self.store)
		if (bAddress & 0x80) == 0 and (aAddress & 0x80) != 0:  								# creating crossed the middle of the page
			self.store[0] += 1 																# Another page. 

		return self
	#
	def dump(self):
		data = self.render()
		pos = 1 
		while data[pos] != 0:
			p1 = pos + 6
			name = ""
			done = False
			while not done:
				name += chr(data[p1] & 0x7F)
				done = (data[p1] & 0x80) != 0
				p1 += 1
			print("Record ${0:04x} : ${1:02x} {2}{3}".format(pos,data[pos+5],name.lower(),"" if (data[pos+1] & 0xC0) == 0 else "$"))
			pos += data[pos]

# *******************************************************************************************
#
#							Tokeniser worker class
#
# *******************************************************************************************

class Tokeniser(object):
	def __init__(self,identStore):
		self.ts = TokenSet()
		self.iStore = identStore
	#
	def tokenise(self,s):
		s = s.strip()
		self.code = []
		while s != "":
			s = self.tokeniseOne(s).strip()
		return self.code
	#
	def tokeniseOne(self,s):
		#
		#		Numbers, compiled in base 64
		#
		if s[0] >= "0" and s[0] <= "9":
			m = re.match("(\\d+)\\s*(.*)$",s)
			self.renderConstant(int(m.group(1)))
			s = m.group(2)
			if s.startswith("."):
				m = re.match('\\.(\\d+)\\s*(.*)',s)
				digits = ([int(x) for x in m.group(1)] + [0xF])
				digits = digits if len(digits) % 2 == 0 else digits + [0xF]
				self.code.append(self.getTokenID("!!dec"))
				self.code.append(len(digits) >> 1)
				while len(digits) > 0:
					self.code.append(digits[0]*16+digits[1])
					digits = digits[2:]
				return m.group(2)
			return s
		#
		#		Hexadecimal numbers also in base 64
		#
		if s[0] == '$':
			m = re.match("\\$([0-9A-Fa-f]+)\\s*(.*)$",s)
			self.code.append(self.getTokenID("$"))
			self.renderConstant(int(m.group(1),16))
			return m.group(2)
		#
		#		Quoted string : [!!str] [length] [characters]
		#
		if s[0] == '"':
			m = re.match('\\"(.*?)\\"\\s*(.*)',s)
			self.code.append(self.getTokenID("!!str"))
			self.code.append(len(m.group(1)))
			self.code += [ord(c) for c in m.group(1)]
			return m.group(2)
		#
		#		Decimals [!!dec] [length] [digits packed in BCD, ending in $F, max of 8 digits]
		#
		#
		#		Comment
		#
		if s[0] == "'":
			s = s[1:].strip()
			self.code.append(self.getTokenID("'"))
			if s != "":
				s = s.replace('"','')
				self.code.append(self.getTokenID("!!str"))
				self.code.append(len(s))
				self.code += [ord(c) for c in s]
			return ""
		#
		#		Identifier or Token
		#
		if s[0].upper() >= "A" and s[0].upper() <= "Z":
			m = re.match("^([A-Za-z0-9\\_\\.]+\\$?\\(?)\\s*(.*)$",s)
			t = self.ts.getByName(m.group(1))
			if t is not None:
				id = t.getID()
				if id >= 0x100:
					self.code.append(self.getTokenID("!!sh"+str(id >> 8)))
				self.code.append(id & 0xFF)
			else:
				if self.iStore.get(m.group(1)) is None:
					self.iStore.add(m.group(1))
				id = self.iStore.get(m.group(1))
				self.code.append(id >> 8)
				self.code.append(id & 0xFF)
			return m.group(2)
		#
		#		Punctuation
		#
		if len(s) >= 2:
			id = self.ts.getByName(s[:2])
			if id is not None:
				self.code.append(id.getID())
				return s[2:]
		#
		id = self.ts.getByName(s[0])
		assert id is not None
		self.code.append(id.getID())
		return s[1:]
	#
	def getTokenID(self,name):
		return self.ts.getByName(name).getID()
	#
	def renderConstant(self,n):
		if n >= 64:
			self.renderConstant(n >> 6)
		self.code.append(0x40|(n & 0x3F))
	#
	def test(self,s):
		code = self.tokenise(s)
		print("{0}\n\t{1}".format(s,",".join(["{0:02x}".format(n) for n in code])))

if __name__ == "__main__":
	iStore = IdentifierStore()
	iStore.add("name$").add("a.number")
	tk = Tokeniser(iStore)
	tk.test("12 300 $2A $43")
	tk.test(' "" "Hello" ')
	tk.test(".1 1.4082 4.251")
	tk.test("gosub return rnd( left$( while")
	tk.test("name$ a.number fre.d")
	tk.test("case A Y$189E4-42204.1 d9j0m4")
	tk.test(".hello ")
#	tk.test("+")
	iStore.dump()
# 		identifiers
# 		punctuation
