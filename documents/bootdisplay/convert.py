# *******************************************************************************************
# *******************************************************************************************
#
#		Name : 		convert.py
#		Purpose :	Test graphics file
#		Date :		7th January 2024
#		Author : 	Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

import sys,os
from PIL import Image
#
#		Format is width (bytes, e.g. actual width/8), height (pixels), byte data by row.
#		Monochrome format e.g. 8 pixels to a byte.
#
def convert(sourceFile,scale):
	src = Image.open(sourceFile)
	src = src.convert(mode="RGB")
	xSize = src.size[0] // scale
	ySize = src.size[1] // scale
	src = src.resize((xSize,ySize),resample = Image.LANCZOS)
	
	xByteSize = (xSize+7) >> 3
	xByteOffset = (xByteSize * 8 - xSize)//2

	data = [xByteSize,ySize] + [ 0 ] * (xByteSize * ySize)
	print(sourceFile,len(data))
	limit = 128
	for x in range(0,xSize):
		for y in range(0,ySize):
			if x < xSize:
				pixel = src.getpixel((x,y))
				xc = x
				if pixel[0] >= limit or pixel[1] >= limit or pixel[2] >= limit:
					data[(xc >> 3)+y*xByteSize+2] |= (0x80 >> (xc & 7))

	if False:
		for y in range(0,ySize):
			s = ""
			for x in range(0,xSize):
				isOn = data[(x >> 3)+y*xByteSize+2] & (0x80 >> (x & 7))
				s += ("*" if isOn != 0 else ".")
			print(s)

	outputFile = "logo_"+sourceFile.replace(".","_").lower()+".h"
	h = open(outputFile,"w")	
	h.write("{\n")
	h.write(",".join([str(x) for x in data]))
	h.write("\n};\n")

convert("olimex.png",8)
convert("logo.png",6)
convert("neotext.png",5)