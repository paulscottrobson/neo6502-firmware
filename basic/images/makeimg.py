# *******************************************************************************************
# *******************************************************************************************
#
#		Name : 		makeimg.py
#		Purpose :	Convert images to a graphics data file.
#		Date :		8th January 2024
#		Author : 	Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

import sys,os

from gconvert import *
from PIL import Image

# *******************************************************************************************
#
#							Create sheet fitting specification
#
# *******************************************************************************************

def importSheet(coll,isTile,size,rows):
	yPaletteSpace = 16 															# Top of screen palette size
	tileCount = 8 if size == 32 else 16  										# tiles per row
	width = 8 + (size + 8) * tileCount  										# width of image
	height = yPaletteSpace + 8 + (size + 8) * rows

	fName = "{0}_{1}.png".format("tile" if isTile else "sprite",size)  			# access the file
	im = Image.open(fName)  	
	xt = ImageExtractor(im,not isTile,(255,0,255))   							# create an extractor.
	count = 0

	for row in range(0,rows):
		for col in range(0,tileCount):
			obj = xt.extract(col * (size+8) + 8,row * (size+8) + 8 + yPaletteSpace,size)
			if not obj.isBlank():
				count += 1
				coll.add(obj)

	print("\t{0:<7} {1}x{1} found {2}".format("Tiles" if isTile else "Sprites",size,count))

print("Image Set Creator")
collection = ImageCollection()
importSheet(collection,True,16,8)
importSheet(collection,False,16,8)
importSheet(collection,False,32,4)	
collection.render("graphics.gfx")
print("Done.")