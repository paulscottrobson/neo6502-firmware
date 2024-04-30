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

class AssemblerError(Exception):
	pass

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
		if True:
			print("{0:04x} : {1:02x}".format(addr,b))
	
	def WriteList(self,addr,byteList):
		for b in byteList:
			self.writeByte(addr,b)
			addr += 1

if __name__ == "__main__":
	ev = Evaluator()
	m = Memory()
	print(ev.evaluate("R2+500",True))
	print(ev.evaluate("R1+$4FF",True))

