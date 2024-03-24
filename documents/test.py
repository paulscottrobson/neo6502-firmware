import serial
port = serial.Serial("/dev/ttyUSB0")
port.baudrate = 9600
for c in range(0,42):
	for i in range(0,20):
		port.write(bytearray([(c % 10)+i+97]))
	port.write(bytearray([13]))

