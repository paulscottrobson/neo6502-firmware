print chr$(12)
repeat
print chr$(20)
for i = 0 to 127
	print key(i),
next
print time()
until 0