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

# *******************************************************************************************
#
#									Image Objects
#
# *******************************************************************************************

class Tile16x16(object):
	def __init__(self,data):
		self.data = data
	def getSize(self):
		return 16
	def getData(self):
		return self.data
	def isSprite(self):
		return False
	def isBlank(self):
		return len([x for x in self.data if x != 0x88]) == 0

class Sprite16x16(Tile16x16):
	def isSprite(self):
		return True
	def isBlank(self):
		return len([x for x in self.data if x != 0x00]) == 0

class Sprite32x32(Sprite16x16):
	def getSize(self):
		return 32

# *******************************************************************************************
#
#						Extract image from a PIL image
#
# *******************************************************************************************

class ImageExtractor(object):
	#
	#		Start using a preloaded image with optional transparency
	#
	def __init__(self,image,isSprite,transparent):
		self.image = image.convert(mode="RGB")
		self.isSprite = isSprite
		self.transparent = transparent
		self.palette = self.getPalette()
	#
	#		Convert one sprite or tile to an array of colours, 2 pixels
	#		per byte, MSB is left hand of pair, 0 is transparent.
	#
	def extract(self,xc,yc,size):
		data = []
		for y in range(0,size):
			for x in range(0,size):
				c = self.convertPixel(xc+x,yc+y)
				if x % 2 == 0:
					data.append(c << 4)
				else:
					data[-1] += c
		assert len(data) == size*size/2
		if self.isSprite:
			return Sprite16x16(data) if size == 16 else Sprite32x32(data)
		else:
			assert size == 16
			return Tile16x16(data)
	#
	#		Convert a single pixel
	#
	def convertPixel(self,px,py):
		rgb = self.image.getpixel((px,py))
		if rgb == self.transparent:
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

# *******************************************************************************************
#
#									Image Collection
#
# *******************************************************************************************

class ImageCollection(object):
	def __init__(self):
		self.objects = []
		self.headerSize = 256
		self.total = self.headerSize
	#
	def add(self,image):
		self.objects.append(image)
		self.total += len(image.getData())
		assert self.total < 20480-16,"Too many images"
	#
	def render(self,fileName):
		data = [ 0 ] * self.headerSize
		data[0] = 1
		for i in self.objects:
			if not i.isSprite() and i.getSize() == 16:
				data[1] += 1
			if i.isSprite() and i.getSize() == 16:
				data[2] += 1
			if i.isSprite() and i.getSize() == 32:
				data[3] += 1

		for i in self.objects:
			if not i.isSprite() and i.getSize() == 16:
				data += i.getData()
		for i in self.objects:
			if i.isSprite() and i.getSize() == 16:
				data += i.getData()
		for i in self.objects:
			if i.isSprite() and i.getSize() == 32:
				data += i.getData()

		assert len(data) == self.total

		h = open(fileName,"wb")
		h.write(bytes(data))
		h.close()
	
if __name__ == "__main__":
	ix = ImageExtractor(Image.open("16.png"),(255,0,0))
	obj = ix.extract(95,0,16)
	print(obj,obj.getData())

