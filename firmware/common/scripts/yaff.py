# ***************************************************************************************
# ***************************************************************************************
#
#      Name :      yaff.py
#      Authors :   Paul Robson (paul@robsons.org.uk)
#      Date :      16th March 2024
#      Reviewed :  No
#      Purpose :   Yaff exporter.
#
# ***************************************************************************************
# ***************************************************************************************

class Character(object):
	def __init__(self,code):
		self.code = code
		self.binary = [0] * 8

	def writeFontLine(self,line,data):
		self.binary[line] = data

	def export(self,dCode):
		data = ",".join(["0x{0:02x}".format(n) for n in self.binary])
		print("\t{0}, // ${1:02x} {2}".format(data,dCode,chr(dCode) if dCode != 0x5C else '\\'))

class CharacterCollection(object):
	def __init__(self):
		self.characters = {}
		self.currentCharacter = None 
		self.currentLine = 0
		self.blankChar = Character(-1)

	def loadYAFFFile(self,fileName):
		for s in open(fileName).readlines():
			#print("[{0}]".format(s.strip()))
			if s.startswith("u+") or s.startswith("0x"):
				s = s.strip()
				charCode = int(s[2:-1],16)
				self.characters[charCode] = Character(charCode)
				self.currentCharacter = self.characters[charCode]
				self.currentLine = 0

			if s.startswith("    "):
				s = s.strip()
				b = int(s.replace(".","0").replace("@","1"),2)				
				self.currentCharacter.writeFontLine(self.currentLine,b)
				self.currentLine += 1

		return self 

	def export(self,c):
		if c in self.characters:
			self.characters[c].export(c)
		else:
			self.blankChar.export(c)


cc = CharacterCollection()
cc.loadYAFFFile("msx-international.yaff")		
for i in range(32,128):
	cc.export(i)
