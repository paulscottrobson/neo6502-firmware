# ***************************************************************************************
# ***************************************************************************************
#
#      Name :      sweet16.py
#      Authors :   Paul Robson (paul@robsons.org.uk)
#      Date :      10th April 2024
#      Reviewed :  No
#      Purpose :   Sweet 16 emulation code generator.
#
# ***************************************************************************************
# ***************************************************************************************

import sys,os,re

def process(s,n):
	s = s.replace("$R",str(n & 0x0F))
	return s 

mnemonics = [ None ] * 256
code = [ "" ] * 256
currentFirst = None
currentLast = None

for f in sys.argv[1:]:
	for s in [x.rstrip() for x in open(f).readlines() if not x.startswith("#") and x.strip() != ""]:
		if (s[0] >= '0' and s[0] <= '9') or (s[0] >= 'A' and s[0] <= 'Z'):
			m = re.match('^([0-9A-F\\?]+)\\s*\\"(.*)\\"\\s*$',s)
			assert m is not None,"Bad line "+s
			opcode = m.group(1)
			if opcode[1] == '?':
				currentFirst = int(opcode[0],16) << 4
				currentLast = currentFirst+15
				for i in range(currentFirst,currentLast+1):
					assert mnemonics[i] is None,"Duplicate"					
					s = m.group(2).replace("\t"," ").split(" ")
					s[0] = (s[0]+"    ")[:4]
					mnemonics[i] = process(" ".join(s),i)
			else:
				currentFirst = int(opcode,16)
				currentLast = currentFirst
				assert mnemonics[currentFirst] is None,"Duplicate"					
				s = m.group(2).replace("\t"," ").split(" ")
				s[0] = (s[0]+"    ")[:4]
				mnemonics[currentFirst] = process(" ".join(s),i)
		else:
			assert currentFirst is not None,"Code outside definition"			
			for i in range(currentFirst,currentLast+1):
				code[i] = code[i] + s.strip() 


print("//\n//\tThis code is automatically generated.\n//")
print("#ifdef SW16_OPCODES")
for i in range(0,256):
	if mnemonics[i] is not None:
		m = process(mnemonics[i],i)
		print("case 0x{0:02x}: // ${0:02x} {0:3d} {1}".format(i,m))
		print("\t{0};break;\n".format(process(code[i],i)))
print("#endif")

print()

s = ['"'+x.lower()+'"' if x is not None else '"?"' for x in mnemonics]
print("#ifdef SW16_MNEMONICS")
print("static const char *sw16Mnemonics[256] = {{ {0} }};\n".format(",".join(s)))
print("#endif")
