# ***************************************************************************************
# ***************************************************************************************
#
#      Name :      nxmit.py
#      Authors :   Paul Robson (paul@robsons.org.uk)
#      Date :      24th January 2024
#      Reviewed :  No
#      Purpose :   Talk to Neo6502 via the Serial Interface
# 				   You can use this to automate some projects as a library.
#
# ***************************************************************************************
# ***************************************************************************************

import sys,os,re,random
import serial,time

# ***************************************************************************************
#
#										Serial Interface
#										================
#
#		0 						Detach serial interface
# 		1 <data> 				Copy the length-1 bytes of data inc target address
# 		2 ll hh 				Point to memory address hhll
#		3 ll hh 				Point to graphics address hhll
#		4 ll hh 				Point to the address at memory address hhll#
#		5 c  					Insert character in keyboard buffer
#		6 ll hh ll hh 			Set start and size for save.
# 		7 len name 				Save file.
#
# ***************************************************************************************

class SerialInterface(object):
	#
	#		Set up serial port at a high speed
	#
	def __init__(self,port):
		self.port = serial.Serial(port)
		self.port.baudrate = 230400
		self.breakTranmission = 0 													# 0 checksum, 1 insufficient data
	#
	#		Transmit a command
	#
	def transmit(self,data):
		header = [len(data),sum(data) & 0xFF]  										# Header is the length of attached data + checksum
		if self.breakTranmission == 1:
			header[0] = (header[0]+1) & 0xFF
		if self.breakTranmission == 2:
			data = data[0]
		self.port.write(bytes(header))   											# Write header
		self.port.write(bytes(data))  												# Write data block
		#time.sleep(0)
	#
	#		Transmit a single block of data to whatever the current write address has been set to
	#
	def transmitDataBlock(self,data):
		packets = 0  																# Tracking information
		size = len(data)
		while len(data) > 0:  														# While more to send
			count = min(240,len(data))  											# Take the first 240 at most
			self.transmit([1]+data[:count])  										# Append command 1, data transmit and send it
			data = data[count:]  													# Trim from data
			packets += 1  															# Bump packet count.
		print("\tSent {0} bytes in {1} packets.".format(size,packets))
	#
	#		Insert the given string in the keyboard queue (use ! for return)
	#
	def transmitString(self,s):
		s = s.replace("!",chr(13))
		for c in s:  																# Send all character in turn.
			self.transmit([5,ord(c)])
	#
	#		Send a file to 6502 memory.
	#
	def transmit6502File(self,fileName,loadAddress,loadIndirect = False):
		self.transmit([4 if loadIndirect else 2,   									# 4 is address indirect, 2 is address
										loadAddress & 0xFF,loadAddress >> 8])
		self._transmitFile(fileName)
	#
	#		Send a BASIC program *MUST* be tokenised
	#
	def transmitBASICCode(self,fileName):
		self.transmit6502File(fileName,0x820,True)  								# In BASIC $820/1 holds PAGE.
	#
	#		Load a file to graphics memory
	#
	def transmitGraphicFile(self,fileName):
		self.transmit([3,0,0])  													# 3 sends to graphic memory.
		self._transmitFile(fileName)
	#
	#		Transmit a file.
	#
	def _transmitFile(self,file):
		print("Sending file {0}".format(file))
		self.transmitDataBlock([x for x in open(file,"rb").read(-1)])
	#
	#		End a transmission.
	#
	def endTransmit(self):
		self.transmit([0]);
	#
	#		Copy a file.
	#
	def copyFile(self,fileName):
		h = open(fileName,"rb")
		size = len(bytearray(h.read(-1)))
		h.close()
		loadAddress = 0xE000-size
		print("{0} {1}".format(loadAddress,size))
		self.transmit6502File(fileName,loadAddress)
		self.setSaveAddress(loadAddress,size)
		self.saveFile(fileName.split(os.sep)[-1].lower())
	#
	#		Set the start and length 
	#
	def setSaveAddress(self,startAddr,length): 										# 6 sets start and length
		self.transmit([6,startAddr & 0xFF,startAddr >> 8,length & 0xFF,length >> 8])
	#
	#		Save a file
	#
	def saveFile(self,fileName):
		print(len(fileName))		
		self.transmit([7,len(fileName)]+[ord(x) for x in fileName])

# ***************************************************************************************
#
#						A simple command line driven application
#
# ***************************************************************************************

if __name__ == "__main__":
	if len(sys.argv) < 2:
		print("python nxmit.zip <port> <cmd list>")
		print("\t<port> is COM1 or /dev/ttyUSB0 or similar.")
		print("\tCommands supported are :")
		print("\t\tb:<file>\t\tloads <file> as BASIC")
		print("\t\tf:<file>@<hex>\tLoads <file> into CPU Memory at address <hex>")
		print("\t\tg:<file>\t\tloads <file> into Graphic Memory")
		print("\t\tk:<text>\t\tPush text into keyboard buffer. Use ! for return")
		print("\t\ts:<file>\t\tSave <file> on hardware external storage")
		sys.exit(1)

	serial = SerialInterface(sys.argv[1])
	for cmd in sys.argv[2:]:
		if cmd.startswith("b:"):
			serial.transmitBASICCode(cmd[2:])
		elif cmd.startswith("g:"):
			serial.transmitGraphicFile(cmd[2:])
		elif cmd.startswith("f:"):
			cmd = cmd[2:].split("@")
			serial.transmit6502File(cmd[0],int(cmd[1],16),False)
		elif cmd.startswith("k:"):
			serial.transmitString(cmd[2:])
		elif cmd.startswith("s:"):
			serial.copyFile(cmd[2:])
		else:
			print("Unknown command "+cmd)
			sys.exit(1)
	serial.endTransmit();



