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
#
# ***************************************************************************************

class SerialInterface(object):
	#
	#		Set up serial port at a high speed
	#
	def __init__(self,port):
		self.port = serial.Serial(port)
		self.port.baudrate = 460800
	#
	#		Transmit a command
	#
	def transmit(self,data):
		header = [len(data),sum(data) & 0xFF]  										# Header is the length of attached data + checksum
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
		print("Sent {0} bytes in {1} packets.".format(size,packets))
	#
	#		Insert the given string in the keyboard queue (use chr(13) for return)
	#
	def transmitString(self,s):
		for c in s:  																# Send all character in turn.
			self.transmit([5,ord(c)])

if __name__ == "__main__":
	si = SerialInterface('/dev/ttyUSB0')
	si.transmit([4,32,8])
	d = [x for x in open("../basic/build/tokenised.dat","rb").read(-1)]
	si.transmitDataBlock(d)
	si.transmitString("RUN"+chr(13))


