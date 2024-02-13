# *******************************************************************************************
# *******************************************************************************************
#
#		Name : 		create_blanks.py
#		Purpose :	Create blank sprite & tile sheets
#		Date :		8th January 2024
#		Author : 	Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

import sys,os

from gconvert import *
from PIL import Image,ImageDraw

# *******************************************************************************************
#
#							Convert palette entry to triplet
#
# *******************************************************************************************

def getPalette(pal,n):
	return (pal[n][0],pal[n][1],pal[n][2])

# *******************************************************************************************
#
#							Create sheet fitting specification
#
# *******************************************************************************************

def createSheet(isTile,size,rows):
	yPaletteSpace = 16 															# Top of screen palette size
	tileCount = 8 if size == 32 else 16  										# tiles per row
	width = 8 + (size + 8) * tileCount  										# width of image
	height = yPaletteSpace + 8 + (size + 8) * rows
	im = Image.new("RGB",(width,height),color = (64,64,64)) 					# create it
	palette = ImageExtractor(im).getPalette()	 								# get the default palette.
	draw = ImageDraw.Draw(im)
	for i in range(0,16):  														# draw the palette colours.
		w = width // 16
		x = w * i + (width - w*16) // 2
		draw.rectangle([x+1,1,x+w-1,yPaletteSpace-2],fill = getPalette(palette,i),outline = (255,255,255),width = 1)

	filler = (255,255,255)  													# transparency
	for col in range(0,tileCount):  											# draw the work boxes.
		for row in range(0,rows):
			x = col * (size+8) + 8
			y = row * (size+8) + 8 + yPaletteSpace
			draw.rectangle([x-1,y-1,x+size,y+size],fill = filler,outline = (255,255,255),width = 1)
			
	fName = "{0}_{1}.png".format("tile" if isTile else "sprite",size)

	if os.path.isfile(fName):  													# do not automatically overwrite
		print("\tFile "+fName+" already exists. Delete manually.")
	else:
		print("\tCreating {0} images {1}".format(fName,rows*tileCount))
		im.save(fName)

print("Blank sprite/tile sheet creator.")
createSheet(True,16,8)
createSheet(False,16,8)
createSheet(False,32,4)	
