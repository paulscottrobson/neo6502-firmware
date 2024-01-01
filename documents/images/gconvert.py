# *******************************************************************************************
# *******************************************************************************************
#
#		Name : 		gconvert.py
#		Purpose :	Extract & convert an image
#		Date :		1st January 2024
#		Author : 	Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

import sys

from PIL import Image

class ImageExtractor(object):
	#
	#		Start using a preloaded image with optional transparency
	#
	def __init__(self,image,transparent = (-1,-1,-1)):
		self.image = image.convert(mode="RGB")
		self.transparent = transparent
		self.palette = self.getPalette()
	#
	#		Convert one sprite or tile to an array of colours, 2 pixels
	#		per byte, MSB is left hand of pair, 0 is transparent.
	#
	def extract(self,xc,yc,size,isTransparent = False):
		data = []
		for y in range(0,size):
			for x in range(0,size):
				c = self.convertPixel(xc+x,yc+y,isTransparent)
				if x % 2 == 0:
					data.append(c << 4)
				else:
					data[-1] += c
		assert len(data) == size*size/2
		return data
	#
	#		Convert a single pixel
	#
	def convertPixel(self,px,py,isTransparent):
		rgb = self.image.getpixel((px,py))
		if rgb == self.transparent and isTransparent:
			return 0
		best = 99999999
		select = -1

		for i in range(1,15):
			s = self.score(rgb,self.palette[i])
			#print("\t",i,rgb,self.palette[i],s)
			if s < best:
				best = s
				select = i
		return select
	#
	#		Evaluate a pixel's comparison with the palette entry
	#
	def score(self,c1,c2):
		s = (c1[0]-c2[0])*(c1[0]-c2[0])
		s += (c1[1]-c2[1])*(c1[1]-c2[1])
		s += (c1[2]-c2[2])*(c1[2]-c2[2])
		return s
	#
	#		Palette from palette.h in firmware.
	#
	def getPalette(self):
		return [
			[0, 0, 0],
			[255, 0, 77],
			[0, 228, 54],
			[255, 236, 39],
			[29, 43, 83],
			[126, 37, 83],
			[41, 173, 255],
			[255, 241, 232],
			[0, 0, 0],
			[95, 87, 79],
			[0, 135, 81],
			[255, 163, 0],
			[171, 82, 54],
			[131, 118, 156],
			[255, 204, 170],
			[194, 195, 199]
		]

if __name__ == "__main__":
	ix = ImageExtractor(Image.open("16.png"),(255,0,0))
	data = ix.extract(95,0,16)
	print(data)