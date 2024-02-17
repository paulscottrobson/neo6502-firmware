# *******************************************************************************************
# *******************************************************************************************
#
#		Name : 		makeexec.py
#		Purpose :	Build (or list) an executable
#		Date :		17th February 2024
#		Author : 	Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

import os,re,sys 

# *******************************************************************************************
#
#								Executable class
#
# *******************************************************************************************

class Executable(object):
	def __init__(self):
		self.new()
	#
	#		Create a new executable
	#
	def new(self):
		self.code = [ 0x03,ord('N'),ord('E'),ord('O')]  							# Magic number $03 $4E $45 $4F
		self.code += [ 0, 0 ]  														# Requires at least this minimum/maximum version.
		self.lastControlByte = None  												# so we can fix up continuations.
		return self
	#
	#		Add a new file.
	#
	def addFile(self,fileName,loadAddress):
		if self.lastControlByte is not None:  										# is this a continuation ?
			self.code[self.lastControlByte] |= 0x80  								# set the continuation byte
		self.lastControlByte = len(self.code)  										# in case we want to make a continuation next time.
		self.code.append(0x00)  													# control byte.
		self.code.append(loadAddress & 0xFF)  										# load address
		self.code.append(loadAddress >> 8)
		#
		h = open(fileName,"rb")  													# load in file.
		data = [x for x in h.read(-1)]
		h.close()
		#
		self.code.append(len(data) & 0xFF)  										# size of data.
		self.code.append(len(data) >> 8)
		#
		for c in [ord(c) for c in fileName]:										# ASCIIZ comment / filename etc.
			self.code.append(c)
		self.code.append(0) 	
		#
		self.code += data  															# actual data
		return self
	#
	# 		Write file
	#
	def writeFile(self,fileName):
		h = open(fileName,"wb")
		h.write(bytes(self.code))
		h.close()
	#
	#		Output file
	#
	def dumpFile(self,fileName):
		h = open(fileName,"rb")  													# read in file
		data = [x for x in h.read(-1)]
		print("File : {0} ".format(fileName))
		if data[0] != 0x03 or data[1] != 0x4E or data[2] != 0x45 or data[3] != 0x4F:# check if executable
			print("Not a neo6502 executable")
			return self 
		print("Firmware: v{0}.{1}.0".format(data[4],data[5]))  						# min firmware version
		data = data[6:]   															# strip header
		blockCount = 0
		continueDecode = True
		while continueDecode:  														# while block left
			blockCount += 1
			continueDecode = (data[0] & 0x80) != 0  								# block follows.
			print("\nBlock {0}:".format(blockCount))
			loadAddress = data[1] + data[2] * 256  									# get load and size
			dataSize = data[3] + data[4] * 256
			print("\tLoad to   : ${0:04x} ({0})".format(loadAddress))
			print("\tData size : ${0:04x} ({0})".format(dataSize))
			data = data[5:]  														# extract comment / name
			s = ""
			while data[0] != 0:
				s = s + chr(data.pop(0))
			data = data[1:]
			print("\tComment   : {0}".format(s if s != "" else "(none)"))
			data  = data[dataSize:]  												# Remove the actual data
		return self 

Executable.GRAPHIC_OBJECT_MEMORY = 0xFFFF
Executable.LOAD_TO_PAGE = 0xFFFD

e = Executable().new()
e.addFile("storage/frogger.gfx",Executable.GRAPHIC_OBJECT_MEMORY)
e.addFile("storage/frogger.bas",Executable.LOAD_TO_PAGE)
e.writeFile("storage/frogger.neo")
e.dumpFile("storage/frogger.neo")