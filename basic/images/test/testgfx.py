# *******************************************************************************************
# *******************************************************************************************
#
#		Name : 		testgfx.py
#		Purpose :	Test graphics file
#		Date :		2nd January 2024
#		Author : 	Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

import sys

from gconvert import *
ic = ImageCollection()
#
# 	Tiles
#
ix = ImageExtractor(Image.open("16.png"))
o1 = ix.extract(80,32,16) 		# red with box in			
ic.add(o1)
o1 = ix.extract(96,16,16) 		# grey (mostly) box
ic.add(o1)
o1 = ix.extract(64,48,16) 		# red with diamond in
ic.add(o1)
#
#	16x16 sprites
#
ix = ImageExtractor(Image.open("16.png"),(255,0,0))
o1 = ix.extract(96,32,16) 		# rocks
ic.add(o1)
o1 = ix.extract(80,48,16)  		# rectangle on diagonal
ic.add(o1)
o1 = ix.extract(16,32,16) 		# bush
ic.add(o1)
#
#	32x32 sprites
#
o1 = ix.extract(0,0,32) 		# tree
ic.add(o1)
o1 = ix.extract(32,0,32) 		# fir tree
ic.add(o1)

ic.render("test.gfx")
	