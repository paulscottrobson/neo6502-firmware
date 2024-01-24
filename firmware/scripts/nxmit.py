# ***************************************************************************************
# ***************************************************************************************
#
#      Name :      nxmit.py
#      Authors :   Paul Robson (paul@robsons.org.uk)
#      Date :      24th January 2024
#      Reviewed :  No
#      Purpose :   Talk to Neo6502 via the Serial Interface
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
#		4 ll hh 				Point to the address at memory address hhll
#		5 ll hh 				Jmp to hhll (assembler)
#
# ***************************************************************************************

class SerialInterface(object):
	def __init__(self,port = '/dev/ttyUSB0'):
		self.port = serial.Serial('/dev/ttyUSB0')
		self.port.baudrate = 460800

	def transmit(self,data):
		header = [len(data),sum(data) & 0xFF]
		self.port.write(bytes(header))
		self.port.write(bytes(data))
		time.sleep(0)

si = SerialInterface()
c = 0
c1 = 0
while c < 65536:
	data = []
	for n in range(0,random.randint(0,252)):		
		data.append(random.randint(0,255))
	si.transmit(data)

	c1 = c
	c += len(data)
	if c1//1024 != c//1024:
		print("Sent {0}k".format(c//1024))



