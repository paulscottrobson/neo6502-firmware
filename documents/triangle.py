import math 
from PIL import Image,ImageDraw

class Triangle(object):
	def __init__(self,angle = 45):
		self.image = Image.new(mode='RGB',size=(16,16),color="red")
		draw = ImageDraw.Draw(self.image)
		draw.polygon((8,0,13,14,3,14),fill=(255,255,255))
		if angle != 0:
			self.image = self.image.rotate(-angle,fillcolor="red")
		self.image.save("triangle.png")

Triangle(45)