# *******************************************************************************************
# *******************************************************************************************
#
#		Name : 		swasm.py
#		Purpose :	Sweet 16 Assembler
#		Date :		30th April 2024
#		Author : 	Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

import os,re,sys

class AssemblerError(Exception):
	pass

AssemblerError.__LINE__ = 0
AssemblerError.__FILE__ = 0

# *******************************************************************************************
#
#									Evaluator class
#
# *******************************************************************************************

class Evaluator(object):
	def __init__(self):
		self.identifiers = {}
		for i in range(0,16):
			self.define("R{0:x}".format(i),i)
			self.define("R{0}".format(i),i)
	
	def evaluate(self,expr,pass2):
		v = 0
		try:
			ex = expr.lower().replace("$","0x")
			v = int(eval(ex,self.identifiers))
		except NameError:
			if pass2:
				raise AssemblerError("Unknown identifier in "+expr)
			v = None
		except SyntaxError:
			raise AssemblerError("Bad expression "+expr)
		return v

	def define(self,ident,value):
		ident = ident.strip().lower()
		if ident in self.identifiers and self.identifiers[ident] != value:
			raise AssemblerError("Identifier "+ident+" redefined.")
		self.identifiers[ident] = value

# *******************************************************************************************
#
#										Memory Store
#
# *******************************************************************************************

class Memory(object):
	def __init__(self):
		self.memory = [ 0 ] * 0x10000
		self.lowMemory = None
		self.highMemory = None

	def writeByte(self,addr,b):
		assert addr >= 0 and addr < 0x10000 and b >= 0 and b < 256
		if self.lowMemory is None:
			self.lowMemory = addr
			self.highMemory = addr
		self.memory[addr] = b
		self.lowMemory = min(self.lowMemory,addr)
		self.highMemory = max(self.highMemory,addr)
		if False:
			print("{0:04x} : {1:02x}".format(addr,b))
	
	def writeList(self,addr,byteList):
		for b in byteList:
			self.writeByte(addr,b)
			addr += 1

	def writeMemory(self,fileName):
		if self.lowMemory is not None:
			h = open(fileName,"wb")
			h.write(bytes(self.memory[self.lowMemory:self.highMemory+1]))
			h.close()

# *******************************************************************************************
#
#										Assembler
#
# *******************************************************************************************

class Assembler(object):
	def __init__(self,memory,evaluator):
		self.memory = memory 
		self.evaluator = evaluator
		self.pc = None
		self.apass = None
		self.group0 = {	"rtn":0x00,"bk":0x0A,"rs":0x0B }
		self.group1 = { 
			"ld":0x20,"st":0x30, "ld@":0x40,"st@":0x50,"ldd@":0x60,"std@":0x70,				
			"pop@":0x80,"stp@":0x90,"add":0xA0,"sub":0xB0,"popd@":0xC0,
			"cpr":0xD0,"inr":0xE0,"dcr":0xF0
		}
		self.group2 = {
			"br":0x01,"bnc":0x02,"bc":0x03,"bp":0x04,"bm":0x05,"bz":0x06,
			"bnz":0x07,"bm1":0x08,"bnm1":0x09,"bs":0x0C
		}
		
	def newPass(self,apass):
		self.apass = apass
		self.pc = None
		AssemblerError.__LINE__ = 0
		AssemblerError.__FILE__ = ""

	def assemble(self,l,listing = None):
		self.current = []
		if self.apass == 2 and listing is None:
			listing = sys.stdout
		s = l if l.find(";") < 0 else l[:l.find(";")]
		s = s.strip()
		if s != "":
			m = re.match('^([a-z1]+\\s?\\@?)(.*)$',s)
			opcode = s.split(" ")[0].lower().strip()
			opcode = "" if m is None else m.group(1).replace(" ","")
			operand = "" if m is None else m.group(2).strip()

			if s.endswith(":"):
				self.checkInCode()
				self.evaluator.define(s[:-1],self.pc)
			elif s in self.group0:
				self.current.append(self.group0[s])
			elif opcode in self.group1:
				self.current.append(self.group1[opcode]+self.evaluateRegister(operand))
			elif opcode in self.group2:
				self.relativeBranch(self.group2[opcode],operand,1)
			elif opcode == "set":
				operand = operand.split(",")
				if len(operand) != 2:
					raise AssemblerError("Syntax error "+l)
				self.current.append(0x10+self.evaluateRegister(operand[0]))
				v = self.evaluate(operand[1])
				self.current += [v & 0xFF,(v >> 8) & 0xFF]
			elif opcode == "bsl":
				self.relativeBranch(0x0D,operand,2)
			else:
				self.pseudoOperation(s)
			if listing is not None:
				b = " ".join(["{0:02x}".format(n) for n in self.current])
				listing.write("{0:04x} : {1:20} : {3}{2}\n".format(self.pc,b[:20],s,"" if s.endswith(":") else "  "))
			if len(self.current) > 0:
				self.memory.writeList(self.pc,self.current)
				self.pc += len(self.current)

	def relativeBranch(self,opcode,addr,size):
		self.current.append(opcode)
		offset = self.evaluate(addr) - (self.pc + size + 1) 
		offset = 0 if self.apass == 1 else offset
		if size == 1:
			if offset < -128 or offset > 127:
				raise AssemblerError("Bad short offset "+addr)
			self.current.append(offset & 0xFF)
		else:
			self.current += [offset & 0xFF,(offset >> 8) & 0xFF]
	def checkInCode(self):
		if self.pc is None:
			raise AssemblerError("No code origin set")

	def pseudoOperation(self,s):
		if s.startswith("org"):
			self.pc = self.evaluate(s[3:],True)
			if self.pc < 0 or self.pc >= 0x10000:
				raise AssemblerError("Bad Origin "+s)
		elif s.startswith("byte"):
			self.dataOperation(s[4:],1)
		elif s.startswith("word"):
			self.dataOperation(s[4:],2)
		elif s.startswith("text"):
			self.textOperation(s[4:])
		else:
			raise AssemblerError("Syntax Error "+s)

	def dataOperation(self,data,size):
		for b in data.split(","):
			n = self.evaluate(b)
			n = 0 if n is None else n
			self.current.append(n & 0xFF)
			if size == 2:
				self.checkInCode()
				self.current.append((n >> 8) & 0xFF)

	def textOperation(self,data):
		for b in [x.strip() for x in data.split(",")]:
			ok = (b[0] == '"' or b[0] == "'") and b[0] == b[-1]
			if not ok:
				raise AssemblerError("Bad text string "+data)
			self.current += [ord(x) for x in b[1:-1]]

	def evaluate(self,e,check = None):
		if check is None:
			check = (self.apass == 2)
		return self.evaluator.evaluate(e,check)		

	def evaluateRegister(self,e):
		reg = self.evaluate(e,True)
		if reg < 0 or reg > 15:
			raise AssemblerError("Bad register "+e)
		return reg

if __name__ == "__main__":
	m = Memory()
	asm = Assembler(m,Evaluator())
	for p in range(1,3):
		asm.newPass(p)
		asm.assemble("; comment")
		asm.assemble("")
		asm.assemble(" org $C000")
		asm.assemble("start:")
		asm.assemble(" byte 42,$2B")
		asm.assemble(" word 32767,-2,start")
		asm.assemble(" text 'Hello',\"World\"")
		asm.assemble(" rs")
		asm.assemble(" popd @r3")
		asm.assemble(" dcr re")
		asm.assemble(" set r5,1027")
		asm.assemble(" bs start")
		asm.assemble(" bsl start")
	m.writeMemory("out.bin")


#
#		Wrapping
#		Branch long
#
#		