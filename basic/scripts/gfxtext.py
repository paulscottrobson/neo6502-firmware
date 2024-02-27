# *******************************************************************************************
# *******************************************************************************************
#
#		Name : 		gfxtext.py
#		Purpose :	Convert graphics file to/from text
#		Date :		27th February 2024
#		Author : 	Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

import sys,re

# *******************************************************************************************
#
#									Convertor Class
#
# *******************************************************************************************

class GraphicsConvert(object):
	#
	#		Convert .gfx format to text format
	#
	def toText(self,source,target):
		hOutput = open(target,"w")  																	# Open file, write header
		hOutput.write("#\n#\tConversion of \"{0}\"\n#\n".format(source))
		src = [x for x in open(source,"rb").read(-1)]  													# Read graphics file
		offset1 = 256 + src[1] * 16 * 16 // 2  															# sprite 16 start here
		offset2 = offset1 + src[2] * 16 * 16 // 2  														# sprite 32 start here
		self.xlat = ".abcdefghijklmno"
		hOutput.write("\n[Tile:16x16]\n")  																# export 16x16 tiles
		for i in range(0,src[1]):
			hOutput.write("\n[{0}]\n".format(i))
			self.export(hOutput,src,256 + i * 16 * 16 // 2,16,16)
		hOutput.write("\n[Sprite:16x16]\n")  															# export 16x16 sprites
		for i in range(0,src[2]):
			hOutput.write("\n[{0}]\n".format(i))
			self.export(hOutput,src,offset1 + i * 16 * 16 // 2,16,16)  
		hOutput.write("\n[Sprite:32x32]\n")  															# export 32x32 sprites
		for i in range(0,src[3]):
			hOutput.write("\n[{0}]\n".format(i))
			self.export(hOutput,src,offset2 + i * 32 * 32 // 2,32,32)

		hOutput.close()
		return self
	#
	def export(self,h,src,p,width,height):
		while (height > 0):  																			# for each line
			height = height - 1
			for i in range(0,width >> 1):  																# for each nibble pair
				b = src[p]  																			# read it
				p += 1
				h.write(self.xlat[(b >> 4)])  															# output halves
				h.write(self.xlat[(b & 0x0F)])
			h.write("\n")
	#
	#		Convert text format to .gfx format
	#
	def toGraphics(self,source,target):
		self.src = open(source).readlines()  															# read in file
		self.src = [x.strip().upper() for x in self.src if not (x.startswith("#") or x.strip() == "")]
		self.tiles16 = []  																				# objects collections
		self.sprites16 = []
		self.sprites32 = []
		self.collections = [self.tiles16, self.sprites16,self.sprites32 ]   							# collection of collections
		self.currentCollection = 0  																	# This one we are using.
		#
		while len(self.src) != 0:  																		# while more to do 
			cmd = self.src.pop(0)
			assert cmd.startswith("[") and cmd.endswith("]"),"No command found"  
			if cmd == "[TILE:16X16]":  																# graphic type selection
				self.currentCollection = 0
			elif cmd == "[SPRITE:16X16]":
				self.currentCollection = 1
			elif cmd == "[SPRITE:32X32]":
				self.currentCollection = 2
			elif cmd == "[]":  																			# [] is define next free
				self.addDefinition(len(self.collections[self.currentCollection]))  						
			else:
				m = re.match("^\\[(\\d+)\\]$",cmd)  													# Load in to specific slot
				assert m is not None,"Bad command "+cmd
				self.addDefinition(int(m.group(1)))  													# add that one in.				
		#
		self.render = [ 0 ] * 256  																		# create header
		self.render[0] = 1
		self.render[1] = len(self.tiles16)
		self.render[2] = len(self.sprites16)
		self.render[3] = len(self.sprites32)
		for t in self.tiles16:  																		# render all tiles and sprites
			self.renderSpriteTile(t,16)
		for t in self.sprites16:
			self.renderSpriteTile(t,16)
		for t in self.sprites32:
			self.renderSpriteTile(t,32)
		#
		h = open(target,"wb")  																			# Write out the result
		h.write(bytes(self.render))
		h.close()
		return self
	#
	def addDefinition(self,n):
		while len(self.collections[self.currentCollection]) <= n:  										# Pad out to required length with Nones
			self.collections[self.currentCollection].append(None)
		assert self.collections[self.currentCollection][n] is None,"Duplicate sprite/tile"    			# Check for non duplicate	
		size = 32 if self.currentCollection == 2 else 16  												# size of sprite/tile
		self.collections[self.currentCollection][n] = self.src[:size]  									# get data for sprite
		self.src = self.src[size:]
	#
	def renderSpriteTile(self,data,size):
		for d in data:  																				# for each line of data
			for c in range(0,size >> 1):  																# for each character pair
				self.render.append(self.convert(d[c*2]) * 16 + self.convert(d[c*2+1]))  				# convert to nibble format
	#
	def convert(self,c):
		c = "@" if c == "." else c.upper()  															# must now be A-O
		assert c >= '@' and c <= 'O',"Bad graphic data"
		return ord(c) - ord('@')

if __name__ == "__main__":
	GraphicsConvert().toText("graphics.gfx","graphics.txt").toGraphics("graphics.txt","_test.gfx")
