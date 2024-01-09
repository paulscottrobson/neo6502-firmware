import math 

class Triangle(object):
	def __init__(self,angle = 45):
		self.display = ["................................"] * 16
		rad = math.radians(angle)
		self.sin = math.sin(rad)
		self.cos = math.cos(rad)
		x = -5
		while x <= 5:
			self.line(x,5,0,-7)
			x += 0.03

	def line(self,x1,y1,x2,y2):
		p = 0
		while p < 1:
			self.plot(x1+(x2-x1)*p,y1+(y2-y1)*p)
			p += 0.03

	def plot(self,xc,yc):
		x = xc*self.cos-yc*self.sin 
		y = yc*self.cos+xc*self.sin
		if x >= -7 and x <= 7 and y >= -7 and y <= 7:
			x = (x + 7) * 2
			y = y + 7
			x = round(x)
			y = round(y)
			s = "XX"
			self.display[y] = self.display[y][:x] + s + self.display[y][x+2:]

	def render(self):
		return "\n".join(self.display)

print(Triangle(45).render())
print(Triangle(0).render())